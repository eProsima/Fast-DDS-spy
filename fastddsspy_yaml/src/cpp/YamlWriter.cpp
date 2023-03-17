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

#include <chrono>

#include <cpp_utils/Formatter.hpp>
#include <cpp_utils/time/time_utils.hpp>

#include <ddspipe_yaml/YamlWriter.hpp>

#include <fastddsspy_participants/configuration/SpyParticipantConfiguration.hpp>
#include <fastddsspy_participants/visualization/parser_data.hpp>

#include <fastddsspy_yaml/yaml_configuration_tags.hpp>

namespace eprosima {
namespace ddspipe {
namespace yaml {

// TODO : maybe the methods here should use const tags to write a yaml.

using namespace eprosima::spy::participants;

template <>
void set(
        Yaml& yml,
        const ddspipe::core::types::Guid& value)
{
    set(yml, utils::generic_to_string(value));
}

template <>
void set(
        Yaml& yml,
        const TimestampData& value)
{
    utils::Formatter f;
    f << value.timestamp.seconds();
    set(yml, f.to_string());

    // TODO convert seconds to timestamp
    // set(yml, utils::timestamp_to_string(
    //         std::chrono::time_point_cast<std::chrono::seconds>(
    //             std::chrono::duration_cast<std::chrono::seconds>(
    //                 value.timestamp.seconds()))));
}

template <>
void set(
        Yaml& yml,
        const SimpleParticipantData& value)
{
    set(yml, "name", value.name);
    set(yml, "guid", value.guid);
}

template <>
void set(
        Yaml& yml,
        const ComplexParticipantData::Endpoint& value)
{
    utils::Formatter f;
    f << value.topic_name << " [" << value.topic_type << "] (" << value.number << ")";
    set(yml, f.to_string());
}

template <>
void set(
        Yaml& yml,
        const ComplexParticipantData& value)
{
    set(yml, "name", value.name);
    set(yml, "guid", value.guid);
    set(yml, "datawriters", value.writers);
    set(yml, "datareaders", value.readers);
}

template <>
void set(
        Yaml& yml,
        const SimpleEndpointData& value)
{
    // TODO
}

template <>
void set(
        Yaml& yml,
        const SimpleEndpointData::Topic& value)
{
    utils::Formatter f;
    f << value.topic_name << " [" << value.topic_type << "]";
    set(yml, f.to_string());
}

template <>
void set(
        Yaml& yml,
        const ComplexEndpointData& value)
{
    // TODO
}

template <>
void set(
        Yaml& yml,
        const ComplexEndpointData::ExtendedTopic& value)
{
    // TODO
}

template <>
void set(
        Yaml& yml,
        const ComplexEndpointData::QoS& value)
{
    // TODO
}

template <>
void set(
        Yaml& yml,
        const SimpleTopicData& value)
{
    // TODO
}

template <>
void set(
        Yaml& yml,
        const SimpleTopicData::Rate& value)
{
    // TODO
}

template <>
void set(
        Yaml& yml,
        const ComplexTopicData& value)
{
    // TODO
}

template <>
void set(
        Yaml& yml,
        const ComplexTopicData::Endpoint& value)
{
    // TODO
}

template <>
void set(
        Yaml& yml,
        const DdsDataData& value)
{
    set(yml, "topic", value.topic);
    set(yml, "writer", value.writer);
    set(yml, "timestamp", value.timestamp);
}

} /* namespace yaml */
} /* namespace ddspipe */
} /* namespace eprosima */
