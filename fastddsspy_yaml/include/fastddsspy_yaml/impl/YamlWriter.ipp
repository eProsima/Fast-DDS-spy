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

#include <chrono>

#include <cpp_utils/Formatter.hpp>
#include <cpp_utils/time/time_utils.hpp>

#include <ddspipe_yaml/YamlWriter.hpp>

#include <fastddsspy_participants/configuration/SpyParticipantConfiguration.hpp>
#include <fastddsspy_participants/visualization/parser_data.hpp>

#include <fastddsspy_yaml/library/library_dll.h>
#include <fastddsspy_yaml/yaml_configuration_tags.hpp>

namespace eprosima {
namespace ddspipe {
namespace yaml {

using namespace eprosima::spy::participants;

template <>
FASTDDSSPY_YAML_DllAPI
void set(
        Yaml& yml,
        const ddspipe::core::types::Guid& value);

template <>
FASTDDSSPY_YAML_DllAPI
void set(
        Yaml& yml,
        const TimestampData& value);

template <>
FASTDDSSPY_YAML_DllAPI
void set(
        Yaml& yml,
        const SimpleParticipantData& value);

template <>
FASTDDSSPY_YAML_DllAPI
void set(
        Yaml& yml,
        const ComplexParticipantData::Endpoint& value);

template <>
FASTDDSSPY_YAML_DllAPI
void set(
        Yaml& yml,
        const ComplexParticipantData& value);

template <>
FASTDDSSPY_YAML_DllAPI
void set(
        Yaml& yml,
        const SimpleEndpointData::Topic& value);

template <>
FASTDDSSPY_YAML_DllAPI
void set(
        Yaml& yml,
        const SimpleEndpointData& value);

template <>
FASTDDSSPY_YAML_DllAPI
void set(
        Yaml& yml,
        const ComplexEndpointData::ExtendedTopic& value);

template <>
FASTDDSSPY_YAML_DllAPI
void set(
        Yaml& yml,
        const ComplexEndpointData::QoS& value);

template <>
FASTDDSSPY_YAML_DllAPI
void set(
        Yaml& yml,
        const ComplexEndpointData& value);

template <>
FASTDDSSPY_YAML_DllAPI
void set(
        Yaml& yml,
        const SimpleTopicData::Rate& value);

template <>
FASTDDSSPY_YAML_DllAPI
void set(
        Yaml& yml,
        const SimpleTopicData& value,
        bool is_compact);

template <>
FASTDDSSPY_YAML_DllAPI
void set(
        Yaml& yml,
        const ComplexTopicData::Endpoint& value);

template <>
FASTDDSSPY_YAML_DllAPI
void set(
        Yaml& yml,
        const ComplexTopicData& value);

template <>
FASTDDSSPY_YAML_DllAPI
void set(
        Yaml& yml,
        const DdsDataData& value);

} /* namespace yaml */
} /* namespace ddspipe */
} /* namespace eprosima */
