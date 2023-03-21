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

#include <fastddsspy_participants/visualization/ModelParser.hpp>

namespace eprosima {
namespace spy {
namespace participants {

std::vector<SimpleParticipantData> ModelParser::participants(
        const SpyModel& model)
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
        const SpyModel& model)
{
    std::vector<ComplexParticipantData> result;

    for (const auto& it : model.participant_database_)
    {
        result.push_back(participants(model, it.second.guid));
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
        eprosima::spy::participants::EndpointInfo>& endpoint)
{
    // Check if this topic has already endpoints added
    auto it = already_endpoints_index.find(endpoint.second.topic.m_topic_name);
    if (it == already_endpoints_index.end())
    {
        // If first for this topic, add new topic
        already_endpoints_index[endpoint.second.topic.m_topic_name] = endpoints.size();
        endpoints.push_back({
                        endpoint.second.topic.m_topic_name,
                        endpoint.second.topic.type_name,
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
        const ddspipe::core::types::Guid& guid)
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
                add_endpoint_to_vector(already_endpoints_index_readers, result.readers, endpoint);
            }
            else if (endpoint.second.is_writer())
            {
                add_endpoint_to_vector(already_endpoints_index_writers, result.writers, endpoint);
            }
        }
    }

    return result;
}

std::vector<SimpleEndpointData> writers(
        const SpyModel& model)
{
    std::vector<SimpleEndpointData> result;
    return result;
}

std::vector<ComplexEndpointData> writers_verbose(
        const SpyModel& model)
{
    std::vector<ComplexEndpointData> result;
    return result;
}

ComplexEndpointData writers(
        const SpyModel& model,
        const ddspipe::core::types::Guid& guid)
{
    ComplexEndpointData result;
    return result;
}

std::vector<SimpleEndpointData> readers(
        const SpyModel& model)
{
    std::vector<SimpleEndpointData> result;
    return result;
}

std::vector<ComplexEndpointData> readers_verbose(
        const SpyModel& model)
{
    std::vector<ComplexEndpointData> result;
    return result;
}

ComplexEndpointData readers(
        const SpyModel& model,
        const ddspipe::core::types::Guid& guid)
{
    ComplexEndpointData result;
    return result;
}

std::vector<SimpleTopicData> topics(
        const SpyModel& model)
{
    std::vector<SimpleTopicData> result;
    return result;
}

std::vector<ComplexTopicData> topics_verbose(
        const SpyModel& model)
{
    std::vector<ComplexTopicData> result;
    return result;
}

ComplexTopicData topics(
        const SpyModel& model,
        const std::string& topic_name)
{
    ComplexTopicData result;
    return result;
}

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
