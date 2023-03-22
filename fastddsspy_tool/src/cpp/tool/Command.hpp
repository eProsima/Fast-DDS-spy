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

#include <cpp_utils/macros/custom_enumeration.hpp>
#include <cpp_utils/enum/EnumBuilder.hpp>

namespace eprosima {
namespace spy {

ENUMERATION_BUILDER(
    CommandValue,
    empty_command,
    help,
    exit,
    version,
    participant,
    datawriter,
    datareader,
    topic,
    print,
    error_input
    );

eProsima_ENUMERATION_BUILDER(
    CommandBuilder,
    CommandValue,
        {
            { CommandValue::empty_command COMMA {""}} COMMA
            { CommandValue::help COMMA {"help" COMMA "h" COMMA "H" COMMA "man"}} COMMA
            { CommandValue::exit COMMA {"quit" COMMA "q" COMMA "Q" COMMA "quit()" COMMA "exit" COMMA "x" COMMA "exit()"}} COMMA
            { CommandValue::version COMMA {"version" COMMA "v" COMMA "V"}} COMMA
            { CommandValue::participant COMMA {"participant" COMMA "participants" COMMA "p" COMMA "P"}} COMMA
            { CommandValue::datawriter COMMA {"datawriter" COMMA "datawriters" COMMA "w" COMMA "W" COMMA "writer" COMMA "writers" COMMA "publication" COMMA "publications"}} COMMA
            { CommandValue::datareader COMMA {"datareader" COMMA "datareaders" COMMA "r" COMMA "R" COMMA "reader" COMMA "readers" COMMA "subscription" COMMA "subscriptions"}} COMMA
            { CommandValue::topic COMMA {"topic"  COMMA "topics" COMMA "t" COMMA "T"}} COMMA
            { CommandValue::print COMMA {"print" COMMA "show" COMMA "s" COMMA "S"}} COMMA
        }
    );

} /* namespace spy */
} /* namespace eprosima */
