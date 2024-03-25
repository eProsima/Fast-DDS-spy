// Copyright 2024 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

#include <fastddsspy_yaml/CommandlineArgsSpy.hpp>

namespace eprosima {
namespace spy {
namespace yaml {

CommandlineArgsSpy::CommandlineArgsSpy()
{
    log_filter[utils::VerbosityKind::Info].set_value("FASTDDSSPY", utils::FuzzyLevelValues::fuzzy_level_default);
    log_filter[utils::VerbosityKind::Warning].set_value("FASTDDSSPY",
            utils::FuzzyLevelValues::fuzzy_level_default);
    log_filter[utils::VerbosityKind::Error].set_value("", utils::FuzzyLevelValues::fuzzy_level_default);
}

bool CommandlineArgsSpy::is_valid(
        utils::Formatter& error_msg) const noexcept
{
    return ddspipe::core::CommandlineArgs::is_valid(error_msg);
}

} /* namespace yaml */
} /* namespace spy */
} /* namespace eprosima */
