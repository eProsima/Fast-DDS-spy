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

#include <cpp_utils/user_interface/CommandReader.hpp>

#include "Input.hpp"

namespace eprosima {
namespace spy {

Input::Input()
    : reader_(*CommandBuilder::get_instance())
{
    // Do nothing
}

utils::Command<CommandValue> Input::wait_next_command()
{
    utils::Command<CommandValue> command;
    std::cout << std::endl << "\033[1;36m" << "Insert a command for Fast DDS Spy:\n>> " << "\033[0m" << std::flush;

    auto res = reader_.read_next_command(command);

    if (!res)
    {
        command.command = CommandValue::error_input;
    }
    return command;
}

void Input::wait_something()
{
    utils::Command<CommandValue> _;
    reader_.read_next_command(_);
}

} /* namespace spy */
} /* namespace eprosima */
