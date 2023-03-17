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

#include <ddspipe_yaml/Yaml.hpp>

#include "View.hpp"

namespace eprosima {
namespace spy {

void View::show(const std::string& value)
{
    show(value.c_str());
}

void View::show(const char* value)
{
    std::cout << value << std::endl;
}

template <>
void View::show(
        const Yaml& value)
{
    std::cout << value << std::endl;
}

template <>
void View::show_error(
        const std::string& value)
{
    std::cout << "\033[1;31m" << value << "\033[0m" << std::endl;
}

template <>
void View::show_error(
        const utils::Formatter& value)
{
    show_error(value.to_string());
}

} /* namespace spy */
} /* namespace eprosima */
