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

#include <fastdds/dds/xtypes/dynamic_types/DynamicType.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicPubSubType.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicData.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicDataFactory.hpp>
#include <fastdds/dds/xtypes/utils.hpp>

#include <cpp_utils/user_interface/CommandReader.hpp>
#include <cpp_utils/macros/custom_enumeration.hpp>

#include <ddspipe_core/types/topic/filter/WildcardDdsFilterTopic.hpp>
#include <ddspipe_yaml/YamlWriter.hpp>

#include <fastddsspy_participants/library/config.h>
#include <fastddsspy_participants/model/SpyModel.hpp>
#include <fastddsspy_participants/visualization/ModelParser.hpp>

#include <fastddsspy_yaml/YamlReaderConfiguration.hpp>

#include "Controller.hpp"
#include "Command.hpp"

namespace eprosima {
namespace spy {

Controller::Controller(
        const yaml::Configuration& configuration)
    : backend_(configuration)
    , model_(backend_.model())
    , configuration_(configuration)
{
    // Do nothing
}

void Controller::run()
{
    view_.print_initial();
    utils::Command<CommandValue> command;
    command.command = CommandValue::participant;
    while (command.command != CommandValue::exit)
    {
        command = input_.wait_next_command();
        run_command_(command);
    }
}

void Controller::one_shot_run(
        const std::vector<std::string>& args)
{
    utils::sleep_for(configuration_.one_shot_wait_time_ms);
    run_command_(input_.parse_as_command(args));
}

utils::ReturnCode Controller::reload_configuration(
        yaml::Configuration& new_configuration)
{
    return backend_.reload_configuration(new_configuration);
}

void Controller::run_command_(
        const utils::Command<CommandValue>& command)
{
    switch (command.command)
    {
        case CommandValue::participant:
            participants_command_(command.arguments);
            break;

        case CommandValue::datareader:
            readers_command_(command.arguments);
            break;

        case CommandValue::datawriter:
            writers_command_(command.arguments);
            break;

        case CommandValue::topic:
            topics_command_(command.arguments);
            break;

        case CommandValue::print:
            print_command_(command.arguments);
            break;

        case CommandValue::version:
            version_command_(command.arguments);
            break;

        case CommandValue::help:
            help_command_(command.arguments);
            break;

        case CommandValue::error_input:
            error_command_(command.arguments);
            break;

        default:
            break;
    }
}

fastdds::dds::DynamicData::_ref_type Controller::get_dynamic_data_(
        const fastdds::dds::DynamicType::_ref_type& dyn_type,
        const ddspipe::core::types::RtpsPayloadData& data) noexcept
{
    // TODO fast this should not be done, but dyn types API is like it is.
    auto& data_no_const = const_cast<ddspipe::core::types::RtpsPayloadData&>(data);

    // Create PubSub Type
    fastdds::dds::DynamicPubSubType pubsub_type(dyn_type);
    fastdds::dds::DynamicData::_ref_type dyn_data(fastdds::dds::DynamicDataFactory::get_instance()->create_data(
                dyn_type));

    pubsub_type.deserialize(data_no_const.payload, &dyn_data);

    return dyn_data;
}

void Controller::data_stream_callback_(
        const ddspipe::core::types::DdsTopic& topic,
        const fastdds::dds::DynamicType::_ref_type& dyn_type,
        const ddspipe::core::types::RtpsPayloadData& data)
{
    // Block entrance so prints does not collapse
    std::lock_guard<std::mutex> _(view_mutex_);

    // Get deserializad data
    auto dyn_data = get_dynamic_data_(dyn_type, data);

    // TODO fast this does not make much sense as dynamictypes::print does not allow to choose target
    // change in dyn types to be able to print it in view
    view_.show("---");
    std::stringstream ss;
    ss << std::setw(4);
    if (fastdds::dds::RETCODE_OK !=
            fastdds::dds::json_serialize(dyn_data, fastdds::dds::DynamicDataJsonFormat::EPROSIMA, ss))
    {
        EPROSIMA_LOG_WARNING(FASTDDSSPY_CONTROLLER,
                "Not able to serialize data of topic " << topic.topic_name() << " into JSON format.");
        return;
    }
    std::cout << ss.str() << std::endl;
    view_.show("---\n");
}

void Controller::data_stream_callback_verbose_(
        const ddspipe::core::types::DdsTopic& topic,
        const fastdds::dds::DynamicType::_ref_type& dyn_type,
        const ddspipe::core::types::RtpsPayloadData& data)
{
    // Block entrance so prints does not collapse
    std::lock_guard<std::mutex> _(view_mutex_);

    // Prepare info data
    participants::DdsDataData data_info{
        {topic.m_topic_name, topic.type_name},
        data.source_guid,
        data.source_timestamp
    };

    // Write yaml with info
    Yaml yml;
    ddspipe::yaml::set(yml, data_info);
    view_.show(yml);

    // Get deserializad data
    auto dyn_data = get_dynamic_data_(dyn_type, data);

    // Print data
    view_.show("data:\n---");
    std::stringstream ss;
    ss << std::setw(4);
    if (fastdds::dds::RETCODE_OK !=
            fastdds::dds::json_serialize(dyn_data, fastdds::dds::DynamicDataJsonFormat::EPROSIMA, ss))
    {
        EPROSIMA_LOG_WARNING(FASTDDSSPY_CONTROLLER,
                "Not able to serialize data of topic " << topic.topic_name() << " into JSON format.");
        return;
    }
    std::cout << ss.str() << std::endl;
    view_.show("---\n");
}

bool Controller::compact_argument_(
        const std::string& argument) const noexcept
{
    return (
        (argument == "compact")
        || (argument == "c")
        || (argument == "-c")
        || (argument == "--c")
        || (argument == "C"));
}

bool Controller::verbose_argument_(
        const std::string& argument) const noexcept
{
    return (
        (argument == "verbose")
        || (argument == "v")
        || (argument == "-v")
        || (argument == "--v")
        || (argument == "V"));
}

bool Controller::all_argument_(
        const std::string& argument) const noexcept
{
    return (
        (argument == "all")
        || (argument == "a")
        || (argument == "-a")
        || (argument == "--a")
        || (argument == "A"));
}

void Controller::participants_command_(
        const std::vector<std::string>& arguments) noexcept
{
    dds_entity_command__(
        arguments,
        [](const participants::SpyModel& model)
        {
            return participants::ModelParser::participants(model);
        },
        [](const participants::SpyModel& model)
        {
            return participants::ModelParser::participants_verbose(model);
        },
        [](const participants::SpyModel& model, const ddspipe::core::types::Guid& guid)
        {
            return participants::ModelParser::participants(model, guid);
        },
        "participant"
        );
}

void Controller::writers_command_(
        const std::vector<std::string>& arguments) noexcept
{
    dds_entity_command__(
        arguments,
        [](const participants::SpyModel& model)
        {
            return participants::ModelParser::writers(model);
        },
        [](const participants::SpyModel& model)
        {
            return participants::ModelParser::writers_verbose(model);
        },
        [](const participants::SpyModel& model, const ddspipe::core::types::Guid& guid)
        {
            return participants::ModelParser::writers(model, guid);
        },
        "writer"
        );
}

void Controller::readers_command_(
        const std::vector<std::string>& arguments) noexcept
{
    dds_entity_command__(
        arguments,
        [](const participants::SpyModel& model)
        {
            return participants::ModelParser::readers(model);
        },
        [](const participants::SpyModel& model)
        {
            return participants::ModelParser::readers_verbose(model);
        },
        [](const participants::SpyModel& model, const ddspipe::core::types::Guid& guid)
        {
            return participants::ModelParser::readers(model, guid);
        },
        "reader"
        );
}

void Controller::topics_command_(
        const std::vector<std::string>& arguments) noexcept
{
    Yaml yml;

    // Handle 'topics' command without arguments
    if (arguments.size() == 1)
    {
        // all participants simple
        ddspipe::yaml::set(yml, participants::ModelParser::topics(*model_), false);
    }
    else
    {
        const std::string& option = arguments[1];

        if (compact_argument_(option))
        {
            // Handle 'topics compact'
            ddspipe::yaml::set(yml, participants::ModelParser::topics(*model_), true);
        }
        else if (verbose_argument_(option))
        {
            // Handle 'topics verbose'
            ddspipe::yaml::set(yml, participants::ModelParser::topics_verbose(*model_));
        }
        else
        {
            // Handle 'topics <name>'
            ddspipe::core::types::WildcardDdsFilterTopic filter_topic;
            filter_topic.topic_name = option;
            auto data = participants::ModelParser::topics_verbose(*model_, filter_topic);
            if (data.empty())
            {
                view_.show_error(STR_ENTRY
                        << "<"
                        << arguments[1]
                        << "> topic does not exist in the DDS network.");
                return;
            }
            ddspipe::yaml::set_collection(yml, data);
        }
    }

    view_.show(yml);
}

void Controller::print_command_(
        const std::vector<std::string>& arguments) noexcept
{
    // Check the number of arguments is correct
    if (arguments.size() < 2)
    {
        view_.show_error(STR_ENTRY
                << "Command <"
                << arguments[0]
                << "> requires at least one argument.");
        return;
    }

    // Print all data
    if (all_argument_(arguments[1]))
    {
        bool activated = model_->activate_all(
            std::make_shared<participants::DataStreamer::CallbackType>(
                [this](
                    const ddspipe::core::types::DdsTopic& topic,
                    const fastdds::dds::DynamicType::_ref_type& dyn_type,
                    const ddspipe::core::types::RtpsPayloadData& data
                    )
                {
                    data_stream_callback_verbose_(topic, dyn_type, data);
                }));

        if (!activated)
        {
            view_.show_error(STR_ENTRY
                    << "Error printing all topics.");
            return;
        }

    }

    // Print topic
    else
    {
        std::string topic_name = arguments[1];

        ddspipe::core::types::WildcardDdsFilterTopic filter_topic;
        filter_topic.topic_name = topic_name;

        std::set<eprosima::ddspipe::core::types::DdsTopic> topics = participants::ModelParser::get_topics(*model_, filter_topic);
        if (topics.empty())
        {
            view_.show_error(STR_ENTRY
                    << "Topic <"
                    << arguments[1]
                    << "> does not exist.");
            return;
        }

        bool topic_available = false;
        for (const auto& topic : topics)
        {
            topic_available = model_->is_topic_type_discovered(topic);
            if (topic_available)
            {
                break;
            }
        }

        if (!topic_available)
        {
            view_.show_error(STR_ENTRY
                    << "Topic Type <"
                    << topic_name
                    << "> has not been discovered, and thus cannot print its data.");
            return;
        }

        // Check if verbose is set
        bool verbose = false;
        if (arguments.size() >= 3)
        {
            verbose = verbose_argument_(arguments[2]);
        }

        std::shared_ptr<participants::DataStreamer::CallbackType> callback;

        if (verbose)
        {
            callback = std::make_shared<participants::DataStreamer::CallbackType>(
                [this](
                    const ddspipe::core::types::DdsTopic& topic,
                    const fastdds::dds::DynamicType::_ref_type& dyn_type,
                    const ddspipe::core::types::RtpsPayloadData& data
                    )
                {
                    data_stream_callback_verbose_(topic, dyn_type, data);
                });
        }
        else
        {
            callback = std::make_shared<participants::DataStreamer::CallbackType>(
                [this](
                    const ddspipe::core::types::DdsTopic& topic,
                    const fastdds::dds::DynamicType::_ref_type& dyn_type,
                    const ddspipe::core::types::RtpsPayloadData& data
                    )
                {
                    data_stream_callback_(topic, dyn_type, data);
                });
        }

        // Must activate data streamer with the required callback
        bool activated = model_->activate(
            filter_topic,
            callback);

        if (!activated)
        {
            view_.show_error(STR_ENTRY
                    << "Error showing data for topic <"
                    << topic_name
                    << ".");
            return;
        }
    }

    // Wait for other command to stop printing topics
    input_.wait_something();
    model_->deactivate();
}

void Controller::version_command_(
        const std::vector<std::string>& arguments) noexcept
{
    view_.show(STR_ENTRY
            << "Fast DDS Spy "
            << FASTDDSSPY_PARTICIPANTS_VERSION_STRING
            << "\ncommit hash: "
            << FASTDDSSPY_PARTICIPANTS_COMMIT_HASH);
}

void Controller::help_command_(
        const std::vector<std::string>& arguments) noexcept
{
    view_.show(STR_ENTRY
            << "Fast DDS Spy is an interactive CLI that allow to instrospect DDS networks.\n"
            << "Each command shows data related with the network in Yaml format.\n"
            << "Commands available and the information they show:\n"
            << "\thelp                   : this help.\n"
            << "\tversion                : tool version.\n"
            << "\tquit                   : exit interactive CLI and close program.\n"
            << "\tparticipants           : DomainParticipants discovered in the network.\n"
            << "\tparticipants verbose   : verbose information about DomainParticipants discovered in the network.\n"
            << "\tparticipants <Guid>    : verbose information related with a specific DomainParticipant.\n"
            << "\twriters                : DataWriters discovered in the network.\n"
            << "\twriters verbose        : verbose information about DataWriters discovered in the network.\n"
            << "\twriters <Guid>         : verbose information related with a specific DataWriter.\n"
            << "\treader                 : DataReaders discovered in the network.\n"
            << "\treader verbose         : verbose information about DataReaders discovered in the network.\n"
            << "\treader <Guid>          : verbose information related with a specific DataReader.\n"
            << "\ttopics                 : Topics discovered in the network.\n"
            << "\ttopics verbose         : verbose information about Topics discovered in the network.\n"
            << "\ttopics <name>          : verbose information related with a specific Topic.\n"
            << "\tshow <name>            : data of a specific Topic (Data Type must be discovered).\n"
            << "\tshow <name> verbose    : data with additional source info of a specific Topic.\n"
            << "\tshow all               : verbose data of all topics (only those whose Data Type is discovered).\n"
            << "\n"
            << "Notes and comments:\n"
            << "\tTo exit from data printing, press enter.\n"
            << "\tEach command is accessible by using its first letter (h/v/q/p/w/r/t/s).\n"
            << "\n"
            << "For more information about these commands and formats, please refer to the documentation:\n"
            << "https://fast-dds-spy.readthedocs.io/en/latest/\n"
            );
}

void Controller::error_command_(
        const std::vector<std::string>& arguments) noexcept
{
    view_.show_error(STR_ENTRY
            << "<" << arguments[0] << "> is not a known command. "
            << "Use <help> command to see valid commands and arguments.");
}

} /* namespace spy */
} /* namespace eprosima */
