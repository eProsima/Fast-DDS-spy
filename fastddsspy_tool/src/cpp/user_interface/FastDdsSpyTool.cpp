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

#include <cpp_utils/user_interface/CommandReader.hpp>
#include <cpp_utils/macros/custom_enumeration.hpp>

#include <fastddsspy_participants/visualizer/SpyVisualizer.hpp>

#include <fastddsspy_yaml/YamlReaderConfiguration.hpp>

#include "FastDdsSpyTool.hpp"
#include "Command.hpp"

namespace eprosima {
namespace spy {
namespace ui {

FastDdsSpyTool::FastDdsSpyTool(const yaml::Configuration& configuration)
    : configuration_(configuration)
    , payload_pool_(std::make_shared<ddspipe::core::FastPayloadPool>())
    , discovery_database_(std::make_shared<ddspipe::core::DiscoveryDatabase>())
    , allowed_topics_(
        std::make_shared<ddspipe::core::AllowedTopicList>(
            configuration.allowlist,
            configuration.blocklist))
    , thread_pool_(
        std::make_shared<utils::SlotThreadPool>(
            configuration.n_threads))
    , participant_database_(std::make_shared<ddspipe::core::ParticipantsDatabase>())
    , spy_visualizer_(
        std::make_shared<participants::SpyVisualizer>(discovery_database_))
    , dds_participant_(
        std::make_shared<participants::SpyDdsParticipant>(
            configuration.simple_configuration,
            payload_pool_,
            discovery_database_))
    , spy_participant_(
        std::make_shared<participants::SpyParticipant>(
            configuration.spy_configuration,
            payload_pool_,
            discovery_database_,
            spy_visualizer_))
{
    // Check configuration
    utils::Formatter error_msg;
    if (!configuration_.is_valid(error_msg))
    {
        throw utils::ConfigurationException(error_msg);
    }

    // Init Dds Participant
    dds_participant_->init();

    // Populate Participant Database
    participant_database_->add_participant(
        dds_participant_->id(),
        dds_participant_
    );
    participant_database_->add_participant(
        spy_participant_->id(),
        spy_participant_
    );

    // Create and initialize Pipe
    pipe_ = std::make_unique<ddspipe::core::DdsPipe>(
        allowed_topics_,
        discovery_database_,
        payload_pool_,
        participant_database_,
        thread_pool_,
        configuration.builtin_topics
    );

    pipe_->enable();
}

FastDdsSpyTool::~FastDdsSpyTool()
{
    pipe_->disable();
}

void FastDdsSpyTool::run()
{
    utils::CommandReader<CommandValue> reader(
        utils::EnumBuilder<CommandValue>({
            {CommandValue::participant, {"participant", "participants"}},
            {CommandValue::datawriter, {"datawriter", "datawriters"}},
            {CommandValue::datareader, {"datareader", "datareaders"}},
            {CommandValue::topic, {"topic", "topics"}},
            {CommandValue::print, {"print"}},
            {CommandValue::exit, {"exit", "quit", "q", ""}},
            {CommandValue::help, {"help", "man", "h"}},
        })
    );

    utils::Command<CommandValue> command;
    command.command = CommandValue::participant;
    while (command.command != CommandValue::exit)
    {
        logUser(FASTDDSSPY_TOOL, "Insert a command for Fast DDS Spy:");
        auto res = reader.read_next_command(command);

        if (!res)
        {
            logUser(FASTDDSSPY_TOOL, "! Command <" << command.arguments[0] << "> not supported.");
            continue;
        }

        switch (command.command)
        {
        case CommandValue::participant:
            spy_visualizer_->print_participants();
            break;

        case CommandValue::datareader:
            spy_visualizer_->print_datareaders();
            break;

        case CommandValue::datawriter:
            spy_visualizer_->print_datawriters();
            break;

        case CommandValue::topic:
            spy_visualizer_->print_topics();
            break;

        case CommandValue::print:

            // Check the number of arguments is correct
            if (command.arguments.size() != 2)
            {
                logUser(FASTDDSSPY_TOOL, "! Command <" << command.arguments[0] << "> requires exactly one argument.");
            }
            else
            {
                printing_data_(reader, command.arguments[1]);
            }
            break;

        case CommandValue::help:
            print_help_();
            break;

        default:
            break;
        }
    }
}

utils::ReturnCode FastDdsSpyTool::reload_allowed_topics(
        const std::shared_ptr<ddspipe::core::AllowedTopicList>& allowed_topics)
{
    return pipe_->reload_allowed_topics(allowed_topics_);
}

bool FastDdsSpyTool::search_topic_(
        const std::string& topic_name,
        ddspipe::core::types::DdsTopic& topic)
{
    // TODO IMPORTANT
    return false;
}

void FastDdsSpyTool::printing_data_(
        utils::CommandReader<CommandValue>& reader,
        const std::string& topic_name)
{
    ddspipe::core::types::DdsTopic topic;
    auto topic_discovered = search_topic_(topic_name, topic);

    if (!topic_discovered)
    {
        logUser(FASTDDSSPY_TOOL, "! Topic <" << topic_name << "> is not in the network.");
        return;
    }

    // Topic discovered, try activate printing data
    auto printing = spy_visualizer_->activate(topic);

    if (!printing)
    {
        logUser(FASTDDSSPY_TOOL, "! Type <" << topic.type_name << "> is not discovered.")
        return;
    }

    // Already printing data, stop until a new line is written
    logUser(FASTDDSSPY_TOOL, "Printing data for topic <" << topic_name << ">. Presh enter to stop.");
    utils::Command<CommandValue> _;
    reader.read_next_command(_);

    spy_visualizer_->deactivate();
}

void FastDdsSpyTool::print_help_()
{
    // TODO IMPORTANT
    logUser(FASTDDSSPY_TOOL, "<help> still in progress.");
}

} /* namespace ui */
} /* namespace spy */
} /* namespace eprosima */
