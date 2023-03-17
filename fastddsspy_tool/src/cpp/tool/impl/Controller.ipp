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

#include <ddspipe_yaml/YamlWriter.hpp>

namespace eprosima {
namespace spy {

/*
 * Auxiliary function that join the functionality that is repeated for participants, readers and writers.
 */

template <typename SimpleF, typename VerboseF, typename specificF>
void Controller::dds_entity_command__(
        const std::vector<std::string>& arguments,
        SimpleF simple_function,
        VerboseF verbose_function,
        specificF specific_function,
        const char* entity_name) noexcept
{
    Yaml yml;
    // Size cannot be 0
    if (arguments.size() == 1)
    {
        // all participants simple
        ddspipe::yaml::set(yml, simple_function(*model_));
    }
    else if (verbose_argument(arguments[1]))
    {
        // verbose
        ddspipe::yaml::set(yml, verbose_function(*model_));
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
            auto data = specific_function(*model_, guid);
            if (!data.guid.is_valid())
            {
                view_.show_error(STR_ENTRY
                    << arguments[1]
                    << " does not match with any known " << entity_name << ".");
                return;
            }
            ddspipe::yaml::set(yml, data);
        }
    }
    view_.show(yml);
}

} /* namespace spy */
} /* namespace eprosima */
