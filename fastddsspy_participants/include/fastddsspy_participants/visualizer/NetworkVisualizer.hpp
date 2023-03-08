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

#include <cpp_utils/types/Atomicable.hpp>

#include <ddspipe_core/dynamic/DiscoveryDatabase.hpp>

#include <fastddsspy_participants/types/ParticipantInfo.hpp>

namespace eprosima {
namespace spy {
namespace participants {

/**
 * TODO
 */
class NetworkVisualizer
{
public:
    void print_participants(
            std::ostream& target = std::cout) const noexcept;

    void print_datareaders(
            std::ostream& target = std::cout) const noexcept;

    void print_datawriters(
            std::ostream& target = std::cout) const noexcept;

    void print_topics(
            std::ostream& target = std::cout) const noexcept;

    void new_participant_info(const ParticipantInfo& info) noexcept;

protected:

    using ParticipantInfoDatabase = utils::Atomicable<std::map<ddspipe::core::types::Guid, ParticipantInfo>>;

    ParticipantInfoDatabase participant_database_;

    std::shared_ptr<ddspipe::core::DiscoveryDatabase> discovery_database_;
};

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
