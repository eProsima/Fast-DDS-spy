// Copyright 2023 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file main.cpp
 *
 */

#include <cpp_utils/event/FileWatcherHandler.hpp>
#include <cpp_utils/event/MultipleEventHandler.hpp>
#include <cpp_utils/event/PeriodicEventHandler.hpp>
#include <cpp_utils/event/SignalEventHandler.hpp>
#include <cpp_utils/exception/ConfigurationException.hpp>
#include <cpp_utils/exception/InitializationException.hpp>
#include <cpp_utils/logging/StdLogConsumer.hpp>
#include <cpp_utils/ReturnCode.hpp>
#include <cpp_utils/thread_pool/pool/SlotThreadPool.hpp>
#include <cpp_utils/time/time_utils.hpp>
#include <cpp_utils/types/Fuzzy.hpp>
#include <cpp_utils/utils.hpp>

#include <ddspipe_core/core/DdsPipe.hpp>
#include <ddspipe_core/dynamic/ParticipantsDatabase.hpp>
#include <ddspipe_core/dynamic/DiscoveryDatabase.hpp>
#include <ddspipe_core/efficiency/payload/FastPayloadPool.hpp>
#include <ddspipe_core/logging/DdsLogConsumer.hpp>

#include <ddspipe_participants/participant/dynamic_types/SchemaParticipant.hpp>
#include <ddspipe_participants/participant/dynamic_types/DynTypesParticipant.hpp>
#include <ddspipe_participants/xml/XmlHandler.hpp>

#include <fastddsspy_yaml/YamlReaderConfiguration.hpp>
#include <fastddsspy_yaml/CommandlineArgsSpy.hpp>

#include "user_interface/constants.hpp"
#include "user_interface/arguments_configuration.hpp"
#include "user_interface/ProcessReturnCode.hpp"
#include "tool/Controller.hpp"

int main(
        int argc,
        char** argv)
{
    // Enable ANSI colors for windows
    eprosima::utils::enable_ansi_colors();
    // Initialize CommandlineArgs
    eprosima::spy::yaml::CommandlineArgsSpy commandline_args;

    // Parse arguments
    eprosima::spy::ProcessReturnCode arg_parse_result =
            eprosima::spy::parse_arguments(argc, argv, commandline_args);

    if (arg_parse_result == eprosima::spy::ProcessReturnCode::help_argument)
    {
        return static_cast<int>(eprosima::spy::ProcessReturnCode::success);
    }
    else if (arg_parse_result == eprosima::spy::ProcessReturnCode::version_argument)
    {
        return static_cast<int>(eprosima::spy::ProcessReturnCode::success);
    }
    else if (arg_parse_result != eprosima::spy::ProcessReturnCode::success)
    {
        return static_cast<int>(arg_parse_result);
    }

    // Check file is in args, else get the default file
    if (commandline_args.file_path == "")
    {
        commandline_args.file_path = eprosima::spy::DEFAULT_CONFIGURATION_FILE_NAME;

        EPROSIMA_LOG_INFO(
            FASTDDSSPY_TOOL,
            "Not configuration file given, try to use default file " << commandline_args.file_path << ".");
    }

    // Check file exists and it is readable
    // NOTE: this check is redundant with option parse arg check
    if (!is_file_accessible(commandline_args.file_path.c_str(), eprosima::utils::FileAccessMode::read))
    {
        EPROSIMA_LOG_INFO(
            FASTDDSSPY_TOOL,
            "File '" << commandline_args.file_path <<
                "' does not exist or it is not accessible. Using default configuration.");
        commandline_args.file_path = "";
    }

    // Encapsulating execution in block to erase all memory correctly before closing process
    try
    {
        /////
        // Fast DDS Spy Initialization

        // Default configuration. Load it from file if file exists
        if (commandline_args.file_path != "")
        {
            EPROSIMA_LOG_INFO(
                FASTDDSSPY_TOOL,
                "Loading configuration from file '" << commandline_args.file_path << "' .");
        }

        eprosima::spy::yaml::Configuration configuration = eprosima::spy::yaml::Configuration(
            commandline_args.file_path, &commandline_args);

        // Debug
        {
            const auto log_configuration = configuration.ddspipe_configuration.log_configuration;

            // Remove every consumer
            eprosima::utils::Log::ClearConsumers();

            // Activate log with verbosity, as this will avoid running log thread with not desired kind
            eprosima::utils::Log::SetVerbosity(configuration.ddspipe_configuration.log_configuration.verbosity);

            // Stdout Log Consumer
            if (log_configuration.stdout_enable)
            {
                eprosima::utils::Log::RegisterConsumer(
                    std::make_unique<eprosima::utils::StdLogConsumer>(&log_configuration));
            }

            // DDS Log Consumer
            if (log_configuration.publish.enable)
            {
                eprosima::utils::Log::RegisterConsumer(
                    std::make_unique<eprosima::ddspipe::core::DdsLogConsumer>(&log_configuration));
            }

            // NOTE:
            // It will not filter any log, so Fast DDS logs will be visible unless Fast DDS is compiled
            // in non debug or with LOG_NO_INFO=ON.
            // This is the easiest way to allow to see Warnings and Errors from Fast DDS.
            // Change it when Log Module is independent and with more extensive API.
            // eprosima::utils::Log::SetCategoryFilter(std::regex("(ddspipe|FASTDDSSPY)"));
        }

        // Load XML profiles
        eprosima::ddspipe::participants::XmlHandler::load_xml(configuration.xml_configuration);

        // Create the Spy
        eprosima::spy::Controller spy(configuration);

        // Update partitions filter from yaml
        spy.set_partition_filter(configuration.dds_configuration->allowed_partition_list);
        // Update topic filter from yaml
        spy.set_content_topic_filter(configuration.dds_configuration->content_topic_filter_dict);

        /////
        // File Watcher Handler

        // Callback will reload configuration and pass it to ddspipe
        // WARNING: it is needed to pass file_path, as FileWatcher only retrieves file_name
        std::function<void(std::string)> filewatcher_callback =
                [&spy, commandline_args]
                (std::string file_name)
                {
                    EPROSIMA_LOG_INFO(
                        FASTDDSSPY_TOOL,
                        "FileWatcher notified changes in file " << file_name << ". Reloading configuration");

                    try
                    {
                        eprosima::spy::yaml::Configuration new_configuration(commandline_args.file_path);
                        spy.reload_configuration(new_configuration);
                    }
                    catch (const std::exception& e)
                    {
                        EPROSIMA_LOG_WARNING(FASTDDSSPY_TOOL,
                                "Error reloading configuration file " << file_name << " with error: " << e.what());
                    }
                };

        // Creating FileWatcher event handler
        std::unique_ptr<eprosima::utils::event::FileWatcherHandler> file_watcher_handler =
                std::make_unique<eprosima::utils::event::FileWatcherHandler>(filewatcher_callback,
                        commandline_args.file_path);

        /////
        // Periodic Handler for reload configuration in periodic time

        // It must be a ptr, so the object is only created when required by a specific configuration
        std::unique_ptr<eprosima::utils::event::PeriodicEventHandler> periodic_handler;

        // If reload time is higher than 0, create a periodic event to reload configuration
        if (commandline_args.reload_time > 0)
        {
            // Callback will reload configuration and pass it to ddspipe
            std::function<void()> periodic_callback =
                    [&spy, commandline_args]
                    ()
                    {
                        EPROSIMA_LOG_INFO(
                            FASTDDSSPY_TOOL,
                            "Periodic Timer raised. Reloading configuration from file " << commandline_args.file_path <<
                                ".");

                        try
                        {
                            eprosima::spy::yaml::Configuration new_configuration(commandline_args.file_path);
                            spy.reload_configuration(new_configuration);
                        }
                        catch (const std::exception& e)
                        {
                            EPROSIMA_LOG_WARNING(FASTDDSSPY_TOOL,
                                    "Error reloading configuration file " << commandline_args.file_path <<
                                    " with error: " <<
                                    e.what());
                        }
                    };

            periodic_handler = std::make_unique<eprosima::utils::event::PeriodicEventHandler>(periodic_callback,
                            commandline_args.reload_time);
        }

        // If one-shot is required, do one shot run. Otherwise run along
        if (commandline_args.one_shot_command.empty())
        {
            spy.run();
        }
        else
        {
            spy.one_shot_run(commandline_args.one_shot_command);
        }

        // Before stopping the Fast DDS Spy erase event handlers that reload configuration
        if (periodic_handler)
        {
            periodic_handler.reset();
        }

        if (file_watcher_handler)
        {
            file_watcher_handler.reset();
        }
    }
    catch (const eprosima::utils::ConfigurationException& e)
    {
        EPROSIMA_LOG_ERROR(FASTDDSSPY_TOOL,
                "Error Loading Fast DDS Spy Configuration from file " << commandline_args.file_path <<
                ". Error message:\n " <<
                e.what());
        return static_cast<int>(eprosima::spy::ProcessReturnCode::execution_failed);
    }
    catch (const eprosima::utils::InitializationException& e)
    {
        EPROSIMA_LOG_ERROR(FASTDDSSPY_TOOL,
                "Error Initializing Fast DDS Spy. Error message:\n " <<
                e.what());
        return static_cast<int>(eprosima::spy::ProcessReturnCode::execution_failed);
    }

    // Force print every log before closing
    eprosima::utils::Log::Flush();

    // Delete the consumers before closing
    eprosima::utils::Log::ClearConsumers();

    return static_cast<int>(eprosima::spy::ProcessReturnCode::success);
}
