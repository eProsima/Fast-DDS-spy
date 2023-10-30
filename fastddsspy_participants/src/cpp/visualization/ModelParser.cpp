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
// limitations under the License\.

#include <utility>

#include <cpp_utils/ros2_mangling.hpp>

#include <fastddsspy_participants/visualization/ModelParser.hpp>

namespace eprosima {
namespace spy {
namespace participants {

std::vector<SimpleParticipantData> ModelParser::participants(
        const SpyModel& model) noexcept
{
    std::vector<SimpleParticipantData> result;
    for (const auto& it : model.participant_database_)
    {
        // Get only active participants
        if (it.second.active)
        {
            result.push_back({it.second.name, it.second.guid});
        }
    }
    return result;
}

std::vector<ComplexParticipantData> ModelParser::participants_verbose(
        const SpyModel& model) noexcept
{
    std::vector<ComplexParticipantData> result;

    for (const auto& it : model.participant_database_)
    {
        // Get only active participants
        if (it.second.active)
        {
            result.push_back(participants(model, it.second.guid));
        }
    }

    return result;
}

/*
 * This is an auxiliary function that is only used in participants(SpyModel, Guid) to
 * not duplicate this functionality between readers and writers.
 */
void add_endpoint_to_vector(
        std::map<std::string, int>& already_endpoints_index,
        std::vector<ComplexParticipantData::Endpoint>& endpoints,
        const std::pair<const eprosima::ddspipe::core::types::Guid,
        eprosima::spy::participants::EndpointInfo>& endpoint,
        bool ros2_types = false) noexcept
{
    // Check if this topic has already endpoints added
    auto it = already_endpoints_index.find(endpoint.second.topic.m_topic_name);
    if (it == already_endpoints_index.end())
    {
        // If first for this topic, add new topic
        already_endpoints_index[endpoint.second.topic.m_topic_name] = endpoints.size();
        endpoints.push_back({
                        endpoint.second.topic.m_topic_name,
                        ros2_types ? utils::demangle_if_ros_type(endpoint.second.topic.type_name) : endpoint.second.topic.type_name,
                        1
                    });
    }
    else
    {
        // If topic already exist, add value in such topic
        endpoints[it->second].number++;
    }
}

ComplexParticipantData ModelParser::participants(
        const SpyModel& model,
        const ddspipe::core::types::Guid& guid) noexcept
{
    ComplexParticipantData result;

    // Look for participant name
    for (const auto& it : model.participant_database_)
    {
        if (it.first == guid)
        {
            result.guid = guid;
            result.name = it.second.name;
            break;
        }
    }

    // If Participant does not exist, stop here
    if (!result.guid.is_valid())
    {
        return result;
    }

    // Get all endpoints with same guid prefix from database and fill writers readers information
    auto prefix = result.guid.guid_prefix();
    std::map<std::string, int> already_endpoints_index_writers;
    std::map<std::string, int> already_endpoints_index_readers;

    for (const auto& endpoint : model.endpoint_database_)
    {
        // Get only for this participant endpoints
        if (endpoint.second.guid.guid_prefix() == prefix)
        {
            if (endpoint.second.is_reader())
            {
                add_endpoint_to_vector(already_endpoints_index_readers, result.readers, endpoint, model.get_ros2_types());
            }
            else if (endpoint.second.is_writer())
            {
                add_endpoint_to_vector(already_endpoints_index_writers, result.writers, endpoint, model.get_ros2_types());
            }
        }
    }

    return result;
}

std::string get_participant_name(
        const SpyModel& model,
        const ddspipe::core::types::Guid guid) noexcept
{
    // Look for participant name
    for (const auto& participant : model.participant_database_)
    {
        // Get only active participants
        if (participant.second.active && participant.first.guid_prefix() == guid.guid_prefix())
        {
            return participant.second.name;
        }
    }
    return "";
}

SimpleEndpointData fill_simple_endpoint(
        const SpyModel& model,
        const spy::participants::EndpointInfo& endpoint) noexcept
{
    std::string participant_name = get_participant_name(model, endpoint.guid);

    return {
        endpoint.guid,
        participant_name,
        {
            endpoint.topic.m_topic_name,
            model.get_ros2_types() ? utils::demangle_if_ros_type(endpoint.topic.type_name) : endpoint.topic.type_name
        }
    };
}

void fill_complex_endpoint(
        const SpyModel& model,
        ComplexEndpointData& result,
        const spy::participants::EndpointInfo& endpoint) noexcept
{
    result.participant_name = get_participant_name(model, endpoint.guid);

    result.guid = endpoint.guid;
    result.topic.topic_name = endpoint.topic.m_topic_name;
    result.topic.topic_type = model.get_ros2_types() ? utils::demangle_if_ros_type(endpoint.topic.type_name) : endpoint.topic.type_name;
    result.qos.durability = endpoint.topic.topic_qos.durability_qos;
    result.qos.reliability = endpoint.topic.topic_qos.reliability_qos;
}

void set_endpoint_simple_information(
        const SpyModel& model,
        std::vector<SimpleEndpointData>& result,
        const ddspipe::core::types::EndpointKind kind) noexcept
{
    for (const auto& endpoint : model.endpoint_database_)
    {
        if (endpoint.second.active && endpoint.second.kind == kind)
        {
            result.push_back(fill_simple_endpoint(model, endpoint.second));
        }
    }
}

void set_endpoint_complex_information(
        const SpyModel& model,
        ComplexEndpointData& result,
        const ddspipe::core::types::EndpointKind kind,
        const eprosima::ddspipe::core::types::Guid guid) noexcept
{
    for (const auto& endpoint : model.endpoint_database_)
    {
        if (endpoint.second.active && endpoint.first == guid && endpoint.second.kind == kind)
        {
            fill_complex_endpoint(model, result, endpoint.second);

            break;
        }
    }
}

std::vector<SimpleEndpointData> ModelParser::writers(
        const SpyModel& model) noexcept
{
    std::vector<SimpleEndpointData> result;

    set_endpoint_simple_information(model, result, eprosima::ddspipe::core::types::EndpointKind::writer);

    return result;
}

std::vector<ComplexEndpointData> ModelParser::writers_verbose(
        const SpyModel& model) noexcept
{
    std::vector<ComplexEndpointData> result;

    for (const auto& endpoint : model.endpoint_database_)
    {
        if (endpoint.second.active && endpoint.second.is_writer())
        {
            result.push_back(writers(model, endpoint.second.guid));
        }
    }

    return result;
}

ComplexEndpointData ModelParser::writers(
        const SpyModel& model,
        const ddspipe::core::types::Guid& guid) noexcept
{
    ComplexEndpointData result;

    set_endpoint_complex_information(model, result, eprosima::ddspipe::core::types::EndpointKind::writer, guid);

    return result;
}

std::vector<SimpleEndpointData> ModelParser::readers(
        const SpyModel& model) noexcept
{
    std::vector<SimpleEndpointData> result;

    set_endpoint_simple_information(model, result, eprosima::ddspipe::core::types::EndpointKind::reader);

    return result;
}

std::vector<ComplexEndpointData> ModelParser::readers_verbose(
        const SpyModel& model) noexcept
{
    std::vector<ComplexEndpointData> result;

    for (const auto& endpoint : model.endpoint_database_)
    {
        if (endpoint.second.active && endpoint.second.is_reader())
        {
            result.push_back(readers(model, endpoint.second.guid));
        }
    }

    return result;
}

ComplexEndpointData ModelParser::readers(
        const SpyModel& model,
        const ddspipe::core::types::Guid& guid) noexcept
{
    ComplexEndpointData result;

    set_endpoint_complex_information(model, result, eprosima::ddspipe::core::types::EndpointKind::reader, guid);

    return result;
}

/*
 * This is an auxiliary function that is used to get topics endpoint database
 */
std::set<eprosima::ddspipe::core::types::DdsTopic> get_topics(
        const SpyModel& model) noexcept
{
    std::set<eprosima::ddspipe::core::types::DdsTopic> result;
    for (const auto& endpoint : model.endpoint_database_)
    {
        if (endpoint.second.active)
        {
            result.insert(endpoint.second.topic);
        }
    }
    return result;
}

ddspipe::core::types::DdsTopic ModelParser::get_topic(
        const SpyModel& model,
        std::string topic_name) noexcept
{

    for (const auto& endpoint : model.endpoint_database_)
    {
        if (endpoint.second.active && endpoint.second.topic.m_topic_name == topic_name)
        {
            return endpoint.second.topic;
        }
    }
    ddspipe::core::types::DdsTopic topic;
    return topic;
}

std::vector<SimpleTopicData> ModelParser::topics(
        const SpyModel& model) noexcept
{
    std::vector<SimpleTopicData> result;

    std::set<eprosima::ddspipe::core::types::DdsTopic> endpoints_topics = get_topics(model);
    for (const auto& topic : endpoints_topics)
    {
        int datawriters = 0;
        int datareaders = 0;
        for (const auto& endpoint : model.endpoint_database_)
        {
            if (endpoint.second.active && endpoint.second.topic.m_topic_name == topic.m_topic_name)
            {
                if (endpoint.second.is_reader())
                {
                    datareaders++;
                }
                if (endpoint.second.is_writer())
                {
                    datawriters++;
                }
            }
        }
        result.push_back({
                        topic.m_topic_name,
                        model.get_ros2_types() ? utils::demangle_if_ros_type(topic.type_name) : topic.type_name,
                        datawriters,
                        datareaders,
                        {
                            model.get_topic_rate(topic),
                            "Hz"
                        }
                    });
    }

    return result;
}

std::vector<ComplexTopicData> ModelParser::topics_verbose(
        const SpyModel& model) noexcept
{
    std::vector<ComplexTopicData> result;

    std::set<eprosima::ddspipe::core::types::DdsTopic> endpoints_topics = get_topics(model);
    for (const auto& topic : endpoints_topics)
    {
        result.push_back(topics(model, topic.m_topic_name));
    }

    return result;
}

ComplexTopicData ModelParser::topics(
        const SpyModel& model,
        const std::string& topic_name) noexcept
{
    ComplexTopicData result;

    // Lets check if topic exists
    auto topic = get_topic(model, topic_name);

    // If topic not found, return an empty Daa
    if (topic.m_topic_name != topic_name)
    {
        result.name = "";
        return result;
    }

    // Topic found, fill its information
    result.name = topic.m_topic_name;
    result.type = model.get_ros2_types() ? utils::demangle_if_ros_type(topic.type_name) : topic.type_name;
    result.discovered = model.is_topic_type_discovered(topic);
    result.rate.rate = model.get_topic_rate(topic);
    result.rate.unit = "Hz";

    for (const auto& it : model.endpoint_database_)
    {
        if (it.second.active && topic.m_topic_name == it.second.topic.m_topic_name)
        {
            if (it.second.is_reader())
            {
                result.datareaders.push_back({it.first});
            }
            if (it.second.is_writer())
            {
                result.datawriters.push_back({it.first});
            }
        }
    }

    return result;
}

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
