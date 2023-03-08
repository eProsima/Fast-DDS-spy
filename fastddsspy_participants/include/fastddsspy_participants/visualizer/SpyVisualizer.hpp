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
#include <fastddsspy_participants/participant/DataVisualizer.hpp>

namespace eprosima {
namespace spy {
namespace core {

/**
 * TODO
 */
class SpyVisualizer : public DataVisualizer
{
public:
    void print_participants() const noexcept;

    void print_datareaders() const noexcept;

    void print_datawriters() const noexcept;

    void print_topics() const noexcept;

    void new_participant_info(const ParticipantInfo& info) noexcept;

protected:

    using ParticipantInfoDatabase = utils::Atomicable<std::map<ddspipe::core::Guid, ParticipantInfo>>;

    ParticipantInfoDatabase participant_database_;

    std::shared_ptr<ddspipe::core::DiscoveryDatabase> discovery_database_;
};

} /* namespace core */
} /* namespace spy */
} /* namespace eprosima */
