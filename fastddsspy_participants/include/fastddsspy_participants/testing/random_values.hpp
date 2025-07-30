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

#include <ddspipe_core/testing/random_values.hpp>
#include <fastddsspy_participants/model/SpyModel.hpp>
#include <fastddsspy_participants/visualization/parser_data.hpp>

namespace eprosima {
namespace spy {
namespace participants {

ddspipe::core::types::Guid random_guid_same_prefix(
        unsigned int seed = 1);

void random_participant_info(
        spy::participants::ParticipantInfo& participant_data,
        bool active = true,
        unsigned int seed = 1);

void random_endpoint_info(
        spy::participants::EndpointInfoData& endpoint_data,
        ddspipe::core::types::EndpointKind kind,
        bool active = true,
        unsigned int seed = 1,
        ddspipe::core::types::DdsTopic topic = ddspipe::core::testing::random_dds_topic());

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
