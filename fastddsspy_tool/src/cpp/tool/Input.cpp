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

#include <iostream>

#include <cpp_utils/utils.hpp>

#include "Input.hpp"

namespace eprosima {
namespace spy {

Input::Input(
        bool prompt_enabled,
        std::istream& source,
        std::ostream& output)
    : prompt_enabled_(prompt_enabled)
    , source_(source)
    , output_(output)
{
    // Do nothing
}

utils::Command<CommandValue> Input::wait_next_command()
{
    if (prompt_enabled_)
    {
        output_ << std::endl << "\033[1;36m" << "Insert a command for Fast DDS Spy:\n>> " << "\033[0m" << std::flush;
    }

    std::string line;
    if (!std::getline(source_, line))
    {
        utils::Command<CommandValue> command;
        command.arguments = {"exit"};
        command.command = CommandValue::exit;
        return command;
    }

    return parse_line_(line);
}

void Input::wait_something()
{
    std::string _;
    std::getline(source_, _);
}

utils::Command<CommandValue> Input::parse_as_command(
        const std::vector<std::string>& args) const
{
    utils::Command<CommandValue> command;
    command.arguments = args;
    if (command.arguments.empty())
    {
        command.arguments = {""};
    }

    auto res = CommandBuilder::get_instance()->string_to_enumeration(command.arguments[0], command.command);
    if (!res)
    {
        command.command = CommandValue::error_input;
    }

    return command;
}

utils::Command<CommandValue> Input::parse_line_(
        const std::string& line) const
{
    return parse_as_command(join_quoted_strings_(utils::split_string(line, " ")));
}

std::vector<std::string> Input::join_quoted_strings_(
        const std::vector<std::string>& input) const
{
    std::vector<std::string> result;

    for (size_t i = 0; i < input.size(); ++i)
    {
        if (!input[i].empty() && input[i].front() == '"')
        {
            std::string joined = input[i];

            while (i + 1 < input.size() &&
                    (joined.empty() || joined.back() != '"'))
            {
                joined += " " + input[++i];
            }

            const int trailing_quote_chars = joined.back() == '"' ? 2 : 1;
            result.push_back(joined.substr(1, joined.size() - trailing_quote_chars));
        }
        else
        {
            result.push_back(input[i]);
        }
    }

    return result;
}

} /* namespace spy */
} /* namespace eprosima */
