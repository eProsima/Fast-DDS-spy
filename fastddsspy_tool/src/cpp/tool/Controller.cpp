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

#include <fastrtps/types/DynamicType.h>
#include <fastrtps/types/DynamicPubSubType.h>
#include <fastrtps/types/DynamicData.h>
#include <fastrtps/types/DynamicDataFactory.h>
#include <fastrtps/types/DynamicDataHelper.hpp>

#include <cpp_utils/user_interface/CommandReader.hpp>
#include <cpp_utils/macros/custom_enumeration.hpp>

#include <ddspipe_yaml/YamlWriter.hpp>

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
{
    // Do nothing
}

void Controller::run()
{
    utils::Command<CommandValue> command;
    command.command = CommandValue::participant;
    while (command.command != CommandValue::exit)
    {
        command = input_.wait_next_command();

        switch (command.command)
        {
            case CommandValue::participant:
                participants_command(command.arguments);
                break;

            // TODO add other cases

            case CommandValue::print:
                print_command(command.arguments);
                break;

            default:
                break;
        }
    }
}

utils::ReturnCode Controller::reload_allowed_topics(
        const std::shared_ptr<ddspipe::core::AllowedTopicList>& allowed_topics)
{
    return backend_.reload_allowed_topics(allowed_topics);
}

fastrtps::types::DynamicData_ptr Controller::get_dynamic_data(
        const fastrtps::types::DynamicType_ptr& dyn_type,
        const ddspipe::core::types::RtpsPayloadData& data) noexcept
{
    // TODO this should not be done, but dyn types API is like it is.
    auto& data_no_const = const_cast<ddspipe::core::types::RtpsPayloadData&>(data);

    // Create PubSub Type
    fastrtps::types::DynamicPubSubType pubsub_type(dyn_type);
    fastrtps::types::DynamicData_ptr dyn_data(fastrtps::types::DynamicDataFactory::get_instance()->create_data(dyn_type));

    pubsub_type.deserialize(&data_no_const.payload, dyn_data.get());

    return dyn_data;
}

void Controller::data_stream_callback_(
        const ddspipe::core::types::DdsTopic& topic,
        const fastrtps::types::DynamicType_ptr& dyn_type,
        const ddspipe::core::types::RtpsPayloadData& data)
{
    // Get deserializad data
    auto dyn_data = get_dynamic_data(dyn_type, data);

    // TODO this does not make much sense as print does not allow to choose target
    // change in dyn types to be able to print it in view
    view_.show("---");
    fastrtps::types::DynamicDataHelper::print(dyn_data.get());
    view_.show("---\n");
}

void Controller::data_stream_callback_verbose_(
        const ddspipe::core::types::DdsTopic& topic,
        const fastrtps::types::DynamicType_ptr& dyn_type,
        const ddspipe::core::types::RtpsPayloadData& data)
{
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
    auto dyn_data = get_dynamic_data(dyn_type, data);

    // Print data
    view_.show("data:\n---");
    fastrtps::types::DynamicDataHelper::print(dyn_data.get());
    view_.show("---\n");
}

bool Controller::verbose_argument(
        const std::string& argument) const noexcept
{
    return (
        (argument == "verbose")
        || (argument == "v")
        || (argument == "-v")
        || (argument == "--v")
        || (argument == "V"));
}

bool Controller::all_argument(
        const std::string& argument) const noexcept
{
    return (
        (argument == "all")
        || (argument == "a"));
}

void Controller::participants_command(
        const std::vector<std::string>& arguments) noexcept
{
    Yaml yml;
    // Size cannot be 0
    if (arguments.size() == 1)
    {
        // all participants simple
        ddspipe::yaml::set(yml, participants::ModelParser::participants(*model_));
    }
    else if (verbose_argument(arguments[1]))
    {
        // verbose
        ddspipe::yaml::set(yml, participants::ModelParser::participants_verbose(*model_));
    }
    else
    {
        // guid given to read participant
        ddspipe::core::types::Guid guid(arguments[1]);
        if (!guid.is_valid())
        {
            view_.show_error(STR_ENTRY
                    << arguments[1]
                    << " is not a valid GUID. Use format <xx.xx.xx.xx.xx.xx.xx.xx.xx.xx.xx.xx|xx.xx.xx.xx>");
            return;
        }
        else
        {
            auto data = participants::ModelParser::participants(*model_, guid);
            if (!data.guid.is_valid())
            {
                view_.show_error(STR_ENTRY
                        << arguments[1]
                        << " does not match with a known participant.");
                return;
            }
            ddspipe::yaml::set(yml, data);
        }
    }
    view_.show(yml);
}

void Controller::print_command(
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
    if (all_argument(arguments[1]))
    {
        bool activated = model_->activate_all(
            std::make_shared<participants::DataStreamer::CallbackType>(
                [this](
                    const ddspipe::core::types::DdsTopic& topic,
                    const fastrtps::types::DynamicType_ptr& dyn_type,
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

    // Print topi   c
    else
    {
        ddspipe::core::types::DdsTopic topic;
        bool topic_exist = model_->get_topic(arguments[1], topic);
        if (!topic_exist)
        {
            view_.show_error(STR_ENTRY
                    << "Topic <"
                    << arguments[1]
                    << "> does not exist.");
            return;
        }

        bool topic_available = model_->is_topic_type_discovered(topic);
        if (!topic_available)
        {
            view_.show_error(STR_ENTRY
                    << "Topic Type <"
                    << topic.type_name
                    << "> has not been discovered, and thus cannot print its data.");
            return;
        }

        // Check if verbose is set
        bool verbose = false;
        if (arguments.size() >= 3)
        {
            verbose = verbose_argument(arguments[2]);
        }

        std::shared_ptr<participants::DataStreamer::CallbackType> callback;

        if (verbose)
        {
            callback = std::make_shared<participants::DataStreamer::CallbackType>(
                [this](
                    const ddspipe::core::types::DdsTopic& topic,
                    const fastrtps::types::DynamicType_ptr& dyn_type,
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
                    const fastrtps::types::DynamicType_ptr& dyn_type,
                    const ddspipe::core::types::RtpsPayloadData& data
                    )
                {
                    data_stream_callback_(topic, dyn_type, data);
                });
        }

        // Must activate data streamer with the required callback
        bool activated = model_->activate(
            topic,
            callback);

        if (!activated)
        {
            view_.show_error(STR_ENTRY
                    << "Error showing data for topic <"
                    << topic.topic_name()
                    << ".");
            return;
        }
    }

    // Wait for other command to stop printing topics
    input_.wait_something();
    model_->deactivate();
}

} /* namespace spy */
} /* namespace eprosima */
