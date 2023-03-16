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

#pragma once

#include <ddspipe_core/types/dds/Guid.hpp>
#include <ddspipe_core/types/dds/TopicQoS.hpp>
#include <ddspipe_core/types/dds/Payload.hpp>

namespace eprosima {
namespace spy {
namespace participants {

struct TimestampData
{
    ddspipe::core::types::DataTime timestamp;
};

struct SimpleParticipantData
{
    std::string name;
    ddspipe::core::types::Guid guid;
};

struct ComplexParticipantData : SimpleParticipantData
{
    struct Endpoint
    {
        std::string topic_name;
        std::string topic_type;
        int number;
    };

    std::vector<Endpoint> writers;
    std::vector<Endpoint> readers;
};

struct SimpleEndpointData
{
    struct Topic
    {
        std::string topic_name;
        std::string topic_type;
    };

    ddspipe::core::types::Guid guid;
    std::string participant_name;
    Topic topic;
};

struct ComplexEndpointData
{
    struct ExtendedTopic
    {
        std::string topic_name;
        std::string topic_type;
    };

    struct QoS
    {
        std::string durability;
        std::string reliability;
    };

    ddspipe::core::types::Guid guid;
    std::string participant_name;
    ExtendedTopic topic;
    QoS qos;
};

struct SimpleTopicData
{
    struct Rate
    {
        float rate;
        std::string unit;
    };

    std::string name;
    std::string type;
    int datawriters;
    int datareaders;
    Rate rate;
};

struct ComplexTopicData
{
    struct Endpoint
    {
        ddspipe::core::types::Guid guid;
    };

    std::string name;
    std::string type;
    std::vector<Endpoint> datawriters;
    std::vector<Endpoint> datareaders;
    SimpleTopicData::Rate rate;
    bool discovered;
};

struct DdsDataData
{
    SimpleEndpointData::Topic topic;
    ddspipe::core::types::Guid writer;
    TimestampData timestamp;
};

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
