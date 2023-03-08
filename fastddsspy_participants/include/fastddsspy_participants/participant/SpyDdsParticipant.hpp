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

#pragma once

#include <ddspipe_participants/participant/dynamic_types/DynTypesParticipant.hpp>

#include <fastddsspy_participants/types/ParticipantInfo.hpp>

namespace eprosima {
namespace spy {
namespace participants {

/**
 * TODO
 */
class SpyDdsParticipant : public ddspipe::participants::DynTypesParticipant
{
public:

    SpyDdsParticipant(
            std::shared_ptr<ddspipe::participants::SimpleParticipantConfiguration> participant_configuration,
            std::shared_ptr<ddspipe::core::PayloadPool> payload_pool,
            std::shared_ptr<ddspipe::core::DiscoveryDatabase> discovery_database);

    ~SpyDdsParticipant();

    //! Override create_reader_() IParticipant method
    std::shared_ptr<core::IReader> create_reader(
            const core::ITopic& topic) override;

    virtual void on_participant_discovery(
            fastdds::dds::DomainParticipant* participant,
            fastrtps::rtps::ParticipantDiscoveryInfo&& info) override;

protected:

    void internal_notify_participant_discovered_(
            const ParticipantInfo& participant_discovered);

    //! Participants Internal Reader
    std::shared_ptr<ddspipe::participants::InternalReader> participants_reader_;
};

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
