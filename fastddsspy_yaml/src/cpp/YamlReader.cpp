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

#include <ddspipe_yaml/YamlReader.hpp>

#include <fastddsspy_participants/configuration/SpyParticipantConfiguration.hpp>

#include <fastddsspy_yaml/yaml_configuration_tags.hpp>

namespace eprosima {
namespace ddspipe {
namespace yaml {

template <>
fastddsspy::participants::SpyParticipantConfiguration
YamlReader::get<fastddsspy::participants::SpyParticipantConfiguration>(
        const Yaml& yml,
        const YamlReaderVersion version)
{
    fastddsspy::participants::SpyParticipantConfiguration conf;

    YamlReader::fill<participants::ParticipantConfiguration>(conf, yml, version);

    return conf;
}

} /* namespace yaml */
} /* namespace ddspipe */
} /* namespace eprosima */
