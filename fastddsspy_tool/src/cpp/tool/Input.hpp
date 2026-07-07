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

#pragma once

#include <iostream>
#include <istream>
#include <ostream>

#include <cpp_utils/user_interface/CommandReader.hpp>

#include "Command.hpp"

namespace eprosima {
namespace spy {

class Input
{
public:

    explicit Input(
            bool prompt_enabled = true,
            std::istream& source = std::cin,
            std::ostream& output = std::cout);

    utils::Command<CommandValue> wait_next_command();

    void wait_something();

    utils::Command<CommandValue> parse_as_command(
            const std::vector<std::string>& args) const;

private:

    utils::Command<CommandValue> parse_line_(
            const std::string& line) const;

    std::vector<std::string> join_quoted_strings_(
            const std::vector<std::string>& input) const;

    bool prompt_enabled_;
    std::istream& source_;
    std::ostream& output_;

};

} /* namespace spy */
} /* namespace eprosima */
