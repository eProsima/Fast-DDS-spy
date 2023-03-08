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

#include <ddspipe_core/interface/IRoutingData.hpp>
#include <ddspipe_core/types/topic/TopicInternalTypeDiscriminator.hpp>
#include <ddspipe_core/types/topic/dds/DdsTopic.hpp>

#include <fastddsspy_participants/types/ParticipantInfo.hpp>

namespace eprosima {
namespace spy {
namespace core {

/**
 * TODO
 */
struct ParticipantInfoData : public core::IRoutingData
{

    virtual types::TopicInternalTypeDiscriminator internal_type_discriminator() const noexcept override;

    //! Info of the participant
    ParticipantInfo info{};
};

DdsTopic participant_info_topic();

DDSPIPE_CORE_DllAPI
bool is_participant_info_topic(
        const ITopic& topic);

constexpr const char* PARTICIPANT_INFO_TOPIC_NAME = "__internal__/participant_info";
constexpr const char* PARTICIPANT_INFO_DATA_TYPE_NAME = "__internal__::participant_info";

const core::types::TopicInternalTypeDiscriminator INTERNAL_TOPIC_TYPE_PARTICIPANTS_INFO = "participant::type::v0";

} /* namespace core */
} /* namespace spy */
} /* namespace eprosima */
