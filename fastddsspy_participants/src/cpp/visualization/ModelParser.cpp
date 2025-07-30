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
        eprosima::spy::participants::EndpointInfoData>& endpoint,
        bool ros2_types = false) noexcept
{
    // Check if this topic has already endpoints added
    auto it = already_endpoints_index.find(endpoint.second.info.topic.m_topic_name);
    if (it == already_endpoints_index.end())
    {
        // If first for this topic, add new topic
        already_endpoints_index[endpoint.second.info.topic.m_topic_name] = endpoints.size();
        endpoints.push_back({
                        ros2_types ? utils::demangle_if_ros_topic(
                            endpoint.second.info.topic.m_topic_name) : endpoint.second.info.topic.m_topic_name,
                        ros2_types ? utils::demangle_if_ros_type(
                            endpoint.second.info.topic.type_name) : endpoint.second.info.topic.type_name,
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
        if (endpoint.second.info.guid.guid_prefix() == prefix)
        {
            if (endpoint.second.info.is_reader())
            {
                add_endpoint_to_vector(already_endpoints_index_readers, result.readers, endpoint,
                        model.get_ros2_types());
            }
            else if (endpoint.second.info.is_writer())
            {
                add_endpoint_to_vector(already_endpoints_index_writers, result.writers, endpoint,
                        model.get_ros2_types());
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
            model.get_ros2_types() ? utils::demangle_if_ros_topic(endpoint.topic.m_topic_name) : endpoint.topic.
                    m_topic_name,
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
    result.topic.topic_name =
            model.get_ros2_types() ? utils::demangle_if_ros_topic(endpoint.topic.m_topic_name) : endpoint.
                    topic.m_topic_name;
    result.topic.topic_type =
            model.get_ros2_types() ? utils::demangle_if_ros_type(endpoint.topic.type_name) : endpoint.topic.
                    type_name;
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
        if (endpoint.second.info.active && endpoint.second.info.kind == kind)
        {
            result.push_back(fill_simple_endpoint(model, endpoint.second.info));
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
        if (endpoint.second.info.active && endpoint.first == guid && endpoint.second.info.kind == kind)
        {
            fill_complex_endpoint(model, result, endpoint.second.info);

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
        if (endpoint.second.info.active && endpoint.second.info.is_writer())
        {
            result.push_back(writers(model, endpoint.second.info.guid));
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
        if (endpoint.second.info.active && endpoint.second.info.is_reader())
        {
            result.push_back(readers(model, endpoint.second.info.guid));
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

std::set<eprosima::ddspipe::core::types::DdsTopic> ModelParser::get_topics(
        const SpyModel& model,
        const ddspipe::core::types::WildcardDdsFilterTopic& filter_topic) noexcept
{
    std::set<eprosima::ddspipe::core::types::DdsTopic> result;
    for (const auto& endpoint : model.endpoint_database_)
    {
        if (endpoint.second.info.active && filter_topic.matches(endpoint.second.info.topic))
        {
            result.insert(endpoint.second.info.topic);
        }
    }
    return result;
}

SimpleTopicData ModelParser::simple_topic_data(
        const SpyModel& model,
        const ddspipe::core::types::DdsTopic& topic) noexcept
{
    SimpleTopicData result;

    int datawriters = 0;
    int datareaders = 0;
    for (const auto& endpoint : model.endpoint_database_)
    {
        if (endpoint.second.info.active && endpoint.second.info.topic.m_topic_name == topic.m_topic_name)
        {
            if (endpoint.second.info.is_reader())
            {
                datareaders++;
            }
            if (endpoint.second.info.is_writer())
            {
                datawriters++;
            }
        }
    }

    result.name = model.get_ros2_types() ? utils::demangle_if_ros_topic(topic.m_topic_name) : topic.m_topic_name;
    result.type = model.get_ros2_types() ? utils::demangle_if_ros_type(topic.type_name) : topic.type_name;
    result.datawriters = datawriters;
    result.datareaders = datareaders;
    result.rate.rate = model.get_topic_rate(topic);
    result.rate.unit = "Hz";

    return result;
}

ComplexTopicData ModelParser::complex_topic_data(
        const SpyModel& model,
        const ddspipe::core::types::DdsTopic& topic) noexcept
{
    ComplexTopicData result;

    // Topic found, fill its information
    result.name = model.get_ros2_types() ? utils::demangle_if_ros_topic(topic.m_topic_name) : topic.m_topic_name;
    result.type = model.get_ros2_types() ? utils::demangle_if_ros_type(topic.type_name) : topic.type_name;
    result.discovered = model.is_topic_type_discovered(topic);
    result.rate.rate = model.get_topic_rate(topic);
    result.rate.unit = "Hz";

    for (const auto& it : model.endpoint_database_)
    {
        if (it.second.info.active && topic.m_topic_name == it.second.info.topic.m_topic_name)
        {
            if (it.second.info.is_reader())
            {
                result.datareaders.push_back({it.first});
            }
            if (it.second.info.is_writer())
            {
                result.datawriters.push_back({it.first});
            }
        }
    }

    return result;
}

std::vector<SimpleTopicData> ModelParser::topics(
        const SpyModel& model,
        const ddspipe::core::types::WildcardDdsFilterTopic& filter_topic) noexcept
{
    std::vector<SimpleTopicData> result;

    std::set<eprosima::ddspipe::core::types::DdsTopic> endpoints_topics = get_topics(model, filter_topic);
    for (const auto& topic : endpoints_topics)
    {
        result.push_back(simple_topic_data(model, topic));
    }

    return result;
}

std::vector<ComplexTopicData> ModelParser::topics_verbose(
        const SpyModel& model,
        const ddspipe::core::types::WildcardDdsFilterTopic& filter_topic) noexcept
{
    std::vector<ComplexTopicData> result;

    std::set<eprosima::ddspipe::core::types::DdsTopic> topics = get_topics(model, filter_topic);
    for (const auto& topic : topics)
    {
        result.push_back(complex_topic_data(model, topic));
    }

    return result;
}

std::string ModelParser::topics_type_idl(
        const SpyModel& model,
        const ddspipe::core::types::WildcardDdsFilterTopic& filter_topic) noexcept
{
    std::set<eprosima::ddspipe::core::types::DdsTopic> topics = get_topics(model, filter_topic);
    for (const auto& topic : topics)
    {
        for (const auto& endpoint : model.endpoint_database_)
        {
            if (endpoint.second.info.active && endpoint.second.info.topic == topic)
            {
                return endpoint.second.type_idl;
            }
        }
    }
    return "";

}

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
