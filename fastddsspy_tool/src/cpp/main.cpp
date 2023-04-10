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
#include <cpp_utils/logging/CustomStdLogConsumer.hpp>
#include <cpp_utils/ReturnCode.hpp>
#include <cpp_utils/time/time_utils.hpp>
#include <cpp_utils/utils.hpp>
#include <cpp_utils/thread_pool/pool/SlotThreadPool.hpp>

#include <ddspipe_core/core/DdsPipe.hpp>
#include <ddspipe_core/dynamic/ParticipantsDatabase.hpp>
#include <ddspipe_core/dynamic/DiscoveryDatabase.hpp>
#include <ddspipe_core/efficiency/payload/FastPayloadPool.hpp>

#include <ddspipe_participants/participant/dynamic_types/SchemaParticipant.hpp>
#include <ddspipe_participants/participant/dynamic_types/DynTypesParticipant.hpp>

#include <fastddsspy_yaml/YamlReaderConfiguration.hpp>

#include "user_interface/constants.hpp"
#include "user_interface/arguments_configuration.hpp"
#include "user_interface/ProcessReturnCode.hpp"
#include "tool/Controller.hpp"

int main(
        int argc,
        char** argv)
{
    // Configuration File path
    std::string file_path = "";

    // Reload time
    eprosima::utils::Duration_ms reload_time = 0;

    // Debug options
    std::string log_filter = "(DDSPIPE|FASTDDSSPY)";
    eprosima::fastdds::dds::Log::Kind log_verbosity = eprosima::fastdds::dds::Log::Kind::Warning;

    // One shot command
    std::vector<std::string> one_shot_command;

    // Parse arguments
    eprosima::spy::ProcessReturnCode arg_parse_result =
            eprosima::spy::parse_arguments(
        argc,
        argv,
        file_path,
        reload_time,
        log_filter,
        log_verbosity,
        one_shot_command);

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

    // Debug
    {
        // Remove every consumer
        eprosima::utils::Log::ClearConsumers();

        // Activate log with verbosity, as this will avoid running log thread with not desired kind
        eprosima::utils::Log::SetVerbosity(log_verbosity);

        eprosima::utils::Log::RegisterConsumer(
            std::make_unique<eprosima::utils::CustomStdLogConsumer>(log_filter, log_verbosity));

        // NOTE:
        // It will not filter any log, so Fast DDS logs will be visible unless Fast DDS is compiled
        // in non debug or with LOG_NO_INFO=ON.
        // This is the easiest way to allow to see Warnings and Errors from Fast DDS.
        // Change it when Log Module is independent and with more extensive API.
        // eprosima::utils::Log::SetCategoryFilter(std::regex("(ddspipe|FOXGLOVEWS)"));
    }

    // Check file is in args, else get the default file
    if (file_path == "")
    {
        file_path = eprosima::spy::DEFAULT_CONFIGURATION_FILE_NAME;

        logInfo(
            FASTDDSSPY_TOOL,
            "Not configuration file given, try to use default file " << file_path << ".");
    }

    // Check file exists and it is readable
    // NOTE: this check is redundant with option parse arg check
    if (!is_file_accessible(file_path.c_str(), eprosima::utils::FileAccessMode::read))
    {
        logInfo(
            FASTDDSSPY_TOOL,
            "File '" << file_path << "' does not exist or it is not accessible. Using default configuration.");
        file_path = "";
    }

    // Encapsulating execution in block to erase all memory correctly before closing process
    try
    {
        /////
        // Fast DDS Spy Initialization

        // Default configuration. Load it from file if file exists
        eprosima::spy::yaml::Configuration configuration;

        if (file_path != "")
        {
            logInfo(
                FASTDDSSPY_TOOL,
                "Loading configuration from file '" << file_path << "' .");
            configuration = eprosima::spy::yaml::Configuration(file_path);
        }

        // Create the Spy
        eprosima::spy::Controller spy(configuration);

        /////
        // File Watcher Handler

        // Callback will reload configuration and pass it to ddspipe
        // WARNING: it is needed to pass file_path, as FileWatcher only retrieves file_name
        std::function<void(std::string)> filewatcher_callback =
                [&spy, file_path]
                    (std::string file_name)
                {
                    logInfo(
                        FASTDDSSPY_TOOL,
                        "FileWatcher notified changes in file " << file_name << ". Reloading configuration");

                    try
                    {
                        eprosima::spy::yaml::Configuration new_configuration(file_path);
                        spy.reload_allowed_topics(
                            std::make_shared<eprosima::ddspipe::core::AllowedTopicList>(
                                new_configuration.allowlist,
                                new_configuration.blocklist
                                )
                            );
                    }
                    catch (const std::exception& e)
                    {
                        logWarning(FASTDDSSPY_TOOL,
                                "Error reloading configuration file " << file_name << " with error: " << e.what());
                    }
                };

        // Creating FileWatcher event handler
        std::unique_ptr<eprosima::utils::event::FileWatcherHandler> file_watcher_handler =
                std::make_unique<eprosima::utils::event::FileWatcherHandler>(filewatcher_callback, file_path);

        /////
        // Periodic Handler for reload configuration in periodic time

        // It must be a ptr, so the object is only created when required by a specific configuration
        std::unique_ptr<eprosima::utils::event::PeriodicEventHandler> periodic_handler;

        // If reload time is higher than 0, create a periodic event to reload configuration
        if (reload_time > 0)
        {
            // Callback will reload configuration and pass it to ddspipe
            std::function<void()> periodic_callback =
                    [&spy, file_path]
                        ()
                    {
                        logInfo(
                            FASTDDSSPY_TOOL,
                            "Periodic Timer raised. Reloading configuration from file " << file_path << ".");

                        try
                        {
                            eprosima::spy::yaml::Configuration new_configuration(file_path);
                            spy.reload_allowed_topics(
                                std::make_shared<eprosima::ddspipe::core::AllowedTopicList>(
                                    new_configuration.allowlist,
                                    new_configuration.blocklist
                                    )
                                );
                        }
                        catch (const std::exception& e)
                        {
                            logWarning(FASTDDSSPY_TOOL,
                                    "Error reloading configuration file " << file_path << " with error: " << e.what());
                        }
                    };

            periodic_handler = std::make_unique<eprosima::utils::event::PeriodicEventHandler>(periodic_callback,
                            reload_time);
        }

        // If one-shot is required, do one shot run. Otherwise run along
        if (one_shot_command.empty())
        {
            spy.run();
        }
        else
        {
            spy.one_shot_run(one_shot_command);
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
        logError(FASTDDSSPY_TOOL,
                "Error Loading Fast DDS Spy Configuration from file " << file_path <<
                ". Error message:\n " <<
                e.what());
        return static_cast<int>(eprosima::spy::ProcessReturnCode::execution_failed);
    }
    catch (const eprosima::utils::InitializationException& e)
    {
        logError(FASTDDSSPY_TOOL,
                "Error Initializing Fast DDS Spy. Error message:\n " <<
                e.what());
        return static_cast<int>(eprosima::spy::ProcessReturnCode::execution_failed);
    }

    // Force print every log before closing
    eprosima::utils::Log::Flush();

    return static_cast<int>(eprosima::spy::ProcessReturnCode::success);
}
