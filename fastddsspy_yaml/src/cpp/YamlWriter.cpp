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

#include <fastddsspy_yaml/library/library_dll.h>
#include <fastddsspy_yaml/yaml_configuration_tags.hpp>
#include <fastddsspy_yaml/YamlWriter.hpp>
#include <nlohmann/json.hpp>

namespace eprosima {
namespace ddspipe {
namespace yaml {

static bool is_primitive_array(
        const nlohmann::json& j)
{
    if (!j.is_array() || j.empty())
    {
        return false;
    }
    for (const auto& element : j)
    {
        if (!element.is_primitive())
        {
            return false;
        }
    }
    return true;
}

static YAML::Node json_to_yaml(
        const nlohmann::json& j)
{
    YAML::Node node;
    if (j.is_null())
    {
        // Return empty/null node
        return node;
    }
    else if (j.is_boolean())
    {
        node = j.get<bool>();
    }
    else if (j.is_number_integer())
    {
        node = j.get<int64_t>();
    }
    else if (j.is_number_unsigned())
    {
        node = j.get<uint64_t>();
    }
    else if (j.is_number_float())
    {
        node = j.get<double>();
    }
    else if (j.is_string())
    {
        node = j.get<std::string>();
    }
    else if (j.is_array())
    {
        if (is_primitive_array(j))
        {
            YAML::Node array_node(YAML::NodeType::Sequence);
            for (const auto& element : j)
            {
                array_node.push_back(json_to_yaml(element));
            }
            array_node.SetStyle(YAML::EmitterStyle::Flow);
            return array_node;
        }
        else
        {
            // Use block style for arrays containing objects
            node = YAML::Node(YAML::NodeType::Sequence);
            for (const auto& element : j)
            {
                node.push_back(json_to_yaml(element));
            }
        }
    }
    else if (j.is_object())
    {
        node = YAML::Node(YAML::NodeType::Map);
        for (auto it = j.begin(); it != j.end(); ++it)
        {
            node[it.key()] = json_to_yaml(it.value());
        }
    }

    return node;
}

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
    // TODO use cpp_utils seconds_to_timestamp when merged
    utils::Formatter f;
    f
        << utils::timestamp_to_string(
        std::chrono::time_point<std::chrono::system_clock>(
            std::chrono::seconds(
                value.timestamp.seconds())),
        "%Y/%m/%d %H:%M:%S");
    set(yml, f.to_string());
}

template <>
void set(
        Yaml& yml,
        const SimpleParticipantData& value)
{
    set_in_tag(yml, "name", value.name);
    set_in_tag(yml, "guid", value.guid);
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
    set_in_tag(yml, "name", value.name);
    set_in_tag(yml, "guid", value.guid);
    set_in_tag(yml, "datawriters", value.writers);
    set_in_tag(yml, "datareaders", value.readers);
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
        const SimpleEndpointData& value)
{
    set_in_tag(yml, "guid", value.guid);
    set_in_tag(yml, "participant", value.participant_name);
    set_in_tag(yml, "topic", value.topic);
}

template <>
void set(
        Yaml& yml,
        const ComplexEndpointData::ExtendedTopic& value)
{
    set_in_tag(yml, "name", value.topic_name);
    set_in_tag(yml, "type", value.topic_type);
    set_in_tag(yml, "partitions", value.partition);
}

template <>
void set(
        Yaml& yml,
        const ComplexEndpointData::QoS& value)
{
    if (value.durability == ddspipe::core::types::DurabilityKind::VOLATILE)
    {
        set_in_tag(yml, "durability", std::string("volatile"));
    }
    else
    {
        set_in_tag(yml, "durability", std::string("transient-local"));
    }

    if (value.reliability == ddspipe::core::types::ReliabilityKind::BEST_EFFORT)
    {
        set_in_tag(yml, "reliability", std::string("best-effort"));
    }
    else
    {
        set_in_tag(yml, "reliability", std::string("reliable"));
    }
}

template <>
void set(
        Yaml& yml,
        const ComplexEndpointData& value)
{
    set_in_tag(yml, "guid", value.guid);
    set_in_tag(yml, "participant", value.participant_name);
    set_in_tag(yml, "topic", value.topic);
    set_in_tag(yml, "qos", value.qos);
}

template <>
void set(
        Yaml& yml,
        const SimpleTopicData::Rate& value)
{
    utils::Formatter f;
    f << value.rate << " " << value.unit;
    set(yml, f.to_string());
}

template <>
void set(
        Yaml& yml,
        const SimpleTopicData& value,
        bool is_compact)
{
    if (is_compact)
    {
        std::string compact_format =
                value.name + " (" + value.type + ") (" +
                std::to_string(value.datawriters) + "|" +
                std::to_string(value.datareaders) + ") [" +
                std::to_string(value.rate.rate) + " " + value.rate.unit + "]";

        set_in_tag(yml, "topic", compact_format);
    }
    else
    {
        set_in_tag(yml, "name", value.name);
        set_in_tag(yml, "type", value.type);
        set_in_tag(yml, "datawriters", value.datawriters);
        set_in_tag(yml, "datareaders", value.datareaders);
        set_in_tag(yml, "rate", value.rate);
    }
}

template <>
void set(
        Yaml& yml,
        const ComplexTopicData::Endpoint& value)
{
    std::string guid_and_partition = "";
    std::ostringstream guid_ss;
    guid_ss << value.guid;
    guid_and_partition = guid_ss.str() + " [" +
            (value.partition == "" ? "\"\"": value.partition) + "]";

    set(yml, guid_and_partition);
}

template <>
void set(
        Yaml& yml,
        const ComplexTopicData& value)
{
    set_in_tag(yml, "name", value.name);
    set_in_tag(yml, "type", value.type);
    set_in_tag(yml, "datawriters", value.datawriters);
    set_in_tag(yml, "datareaders", value.datareaders);
    set_in_tag(yml, "rate", value.rate);
    set_in_tag(yml, "dynamic_type_discovered", value.discovered);
}

template <>
void set(
        Yaml& yml,
        const DdsDataData& value)
{
    set_in_tag(yml, "topic", value.topic);
    set_in_tag(yml, "writer", value.writer);
    set_in_tag(yml, "partitions", value.partitions);
    set_in_tag(yml, "timestamp", value.timestamp);
}

template <>
void set(
        Yaml& yml,
        const TopicKeysData& value,
        bool is_compact)
{
    set_in_tag(yml, "topic", value.topic_name);
    if (value.key_fields.empty())
    {
        yml["keys"] = YAML::Node(YAML::NodeType:: Sequence);
    }
    else
    {
        set_in_tag(yml, "keys", value.key_fields);
    }
    if (!is_compact)
    {
        if (value.instances.empty())
        {
            yml["instances"] = YAML::Node(YAML::NodeType:: Sequence);
        }
        else
        {
            YAML::Node instances_node;
            for (const auto& instance_json_str : value.instances)
            {
                try
                {
                    nlohmann::json j = nlohmann::json::parse(instance_json_str);
                    YAML::Node yaml_instance = json_to_yaml(j);
                    instances_node.push_back(yaml_instance);
                }
                catch (const std::exception& e)
                {
                    instances_node.push_back(instance_json_str);
                }
            }
            yml["instances"] = instances_node;
        }
    }
    yml["instance_count"] = value.instance_count;
}

} /* namespace yaml */
} /* namespace ddspipe */
} /* namespace eprosima */
