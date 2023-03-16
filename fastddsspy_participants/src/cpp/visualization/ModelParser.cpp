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

std::vector<SimpleParticipantData> participants(const SpyModel& model)
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

std::vector<ComplexParticipantData> participants_verbose(const SpyModel& model)
{
    std::vector<ComplexParticipantData> result;

    // Get all endpoints divided by guid prefix, then by topic name, and then readers / writers
    std::map<
        ddspipe::core::types::GuidPrefix,
        std::map<
            std::pair<
                std::string,
                std::string>,
            std::pair<
                int,
                int>>>
                    endpoints;

    for (const auto& endpoint : model.endpoint_database_)
    {
        // This endpoint info
        auto prefix = endpoint.second.guid.guid_prefix();

        // Check if guid prefix already exist, if not create it
        auto it_endp = endpoints.find(prefix);
        if (it_endp == endpoints.end())
        {
            it_endp = endpoints.emplace(
                prefix,
                std::map<std::pair<std::string,std::string>,std::pair<int,int>>()).first;
        }

        // Check if topic already exist, if not create it
        std::pair<std::string, std::string> topic_name_and_type = {
            endpoint.second.topic.m_topic_name,
            endpoint.second.topic.type_name};
        auto it_topic = it_endp->second.find(topic_name_and_type);
        if (it_topic == it_endp->second.end())
        {
            it_topic = it_endp->second.emplace(
                topic_name_and_type,
                std::pair<int,int>()).first;
        }

        // Add a new endpoint to the counter
        if (endpoint.second.is_reader())
        {
            it_topic->second.first++;
        }
        else if (endpoint.second.is_writer())
        {
            it_topic->second.second++;
        }
    }

    // From endpoint information, create Participant info
    for (const auto& it : model.participant_database_)
    {
        // Get only active participants
        if (it.second.active)
        {
            ComplexParticipantData data;
            data.guid = it.second.guid;
            data.name = it.second.name;

            // Look for the associated endpoints
            auto it = endpoints.find(data.guid.guid_prefix());
            if (it != endpoints.end())
            {
                for (const auto& topics : it->second)
                {
                    // Add readers if any
                    if (topics.second.first)
                    {
                        data.readers.push_back({
                            topics.first.first,
                            topics.first.second,
                            topics.second.first
                        });
                    }

                    // Add writers if any
                    if (topics.second.second)
                    {
                        data.readers.push_back({
                            topics.first.first,
                            topics.first.second,
                            topics.second.second
                        });
                    }
                }
            }
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
        const std::pair<const eprosima::ddspipe::core::types::Guid, eprosima::spy::participants::EndpointInfo>& endpoint)
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

ComplexParticipantData participants(const SpyModel& model, const ddspipe::core::types::Guid& guid)
{
    ComplexParticipantData result;
    result.guid = guid;

    // Look for participant name
    for (const auto& it : model.participant_database_)
    {
        if (it.first == guid)
        {
            result.name = it.second.name;
            break;
        }
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
            else if(endpoint.second.is_writer())
            {
                add_endpoint_to_vector(already_endpoints_index_writers, result.writers, endpoint);
            }
        }
    }

    return result;
}


std::vector<SimpleEndpointData> writers(const SpyModel& model)
{
    std::vector<SimpleEndpointData> result;
    return result;
}

std::vector<ComplexEndpointData> writers_verbose(const SpyModel& model)
{
    std::vector<ComplexEndpointData> result;
    return result;
}

ComplexEndpointData writers(const SpyModel& model, const ddspipe::core::types::Guid& guid)
{
    ComplexEndpointData result;
    return result;
}


std::vector<SimpleEndpointData> readers(const SpyModel& model)
{
    std::vector<SimpleEndpointData> result;
    return result;
}

std::vector<ComplexEndpointData> readers_verbose(const SpyModel& model)
{
    std::vector<ComplexEndpointData> result;
    return result;
}

ComplexEndpointData readers(const SpyModel& model, const ddspipe::core::types::Guid& guid)
{
    ComplexEndpointData result;
    return result;
}


std::vector<SimpleTopicData> topics(const SpyModel& model)
{
    std::vector<SimpleTopicData> result;
    return result;
}

std::vector<ComplexTopicData> topics_verbose(const SpyModel& model)
{
    std::vector<ComplexTopicData> result;
    return result;
}

ComplexTopicData topics(const SpyModel& model, const std::string& topic_name)
{
    ComplexTopicData result;
    return result;
}


} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
