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

void random_participant_info(
        spy::participants::ParticipantInfo& participant_data,
        bool active /* = true */,
        unsigned int seed /* = 0 */)
{
    participant_data.name = ddspipe::core::testing::random_participant_id();
    participant_data.active = active;
    participant_data.guid = ddspipe::core::testing::random_guid(seed);
}

void random_endpoint_info(
        spy::participants::EndpointInfo& endpoint_data,
        ddspipe::core::types::EndpointKind kind,
        bool active /* = true */,
        unsigned int seed /* = 0 */,
        ddspipe::core::types::DdsTopic topic /* = ddspipe::core::testing::random_dds_topic(rand() % 15) */)
{
    endpoint_data.active = active;
    endpoint_data.kind = kind;
    endpoint_data.guid = ddspipe::core::testing::random_guid(seed);
    endpoint_data.topic = topic;
    endpoint_data.discoverer_participant_id = ddspipe::core::testing::random_participant_id(seed);
}

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
