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

} /* namespace spy */
} /* namespace eprosima */
