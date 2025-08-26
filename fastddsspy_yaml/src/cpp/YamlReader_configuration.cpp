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

/**
 * @file YamlReader_configuration.cpp
 *
 * File to redefine the validate function for the types that are different in the Fast-DDS Spy.
 */

#include <set>

#include <ddspipe_core/types/dds/TopicQoS.hpp>

#include <ddspipe_yaml/yaml_configuration_tags.hpp>
#include <ddspipe_yaml/YamlValidator.hpp>

namespace eprosima {
namespace ddspipe {
namespace yaml {

template <>
DDSPIPE_YAML_DllAPI
bool YamlValidator::validate<core::types::TopicQoS>(
        const Yaml& yml,
        const YamlReaderVersion& /* version */)
{
    // The QOS_MAX_TX_RATE_TAG is not a valid TopicQoS
    static const std::set<TagType> tags{
        QOS_TRANSIENT_TAG,
        QOS_RELIABLE_TAG,
        QOS_OWNERSHIP_TAG,
        QOS_PARTITION_TAG,
        QOS_HISTORY_DEPTH_TAG,
        QOS_KEYED_TAG,
        QOS_MAX_RX_RATE_TAG,
        QOS_DOWNSAMPLING_TAG};

    return YamlValidator::validate_tags(yml, tags);
}

} /* namespace yaml */
} /* namespace ddspipe */
} /* namespace eprosima */
