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

#include <fastddsspy_participants/testing/random_values.hpp>

namespace eprosima {
namespace spy {
namespace participants {

ddspipe::core::types::Guid random_guid_same_prefix(
        unsigned int seed /* = 1 */)
{
    ddspipe::core::types::Guid guid;
    guid.entityId.value[3] = static_cast<eprosima::fastdds::rtps::octet>(seed);
    guid.guidPrefix.value[0] = 0x01;
    guid.guidPrefix.value[1] = 0x0f;
    return guid;
}

void random_participant_info(
        spy::participants::ParticipantInfo& participant_data,
        bool active /* = true */,
        unsigned int seed /* = 0 */)
{
    participant_data.name = ddspipe::core::testing::random_participant_id(seed);
    participant_data.active = active;
    participant_data.guid = random_guid_same_prefix(seed);
}

void random_endpoint_info(
        spy::participants::EndpointInfoData& endpoint_data,
        ddspipe::core::types::EndpointKind kind,
        bool active /* = true */,
        unsigned int seed /* = 0 */,
        ddspipe::core::types::DdsTopic topic /* = ddspipe::core::testing::random_dds_topic() */)
{
    endpoint_data.info.active = active;
    endpoint_data.info.kind = kind;
    endpoint_data.info.guid = random_guid_same_prefix(seed);
    endpoint_data.info.topic = topic;
    endpoint_data.info.discoverer_participant_id = ddspipe::core::testing::random_participant_id(seed);
    endpoint_data.type_idl = "type_idl";
    // add empty partition
    endpoint_data.info.specific_partitions = std::map<std::string, std::string>();
    std::ostringstream ss;
    ss << endpoint_data.info.guid;
    endpoint_data.info.specific_partitions[ss.str()] = ""; // empty partition
}

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
