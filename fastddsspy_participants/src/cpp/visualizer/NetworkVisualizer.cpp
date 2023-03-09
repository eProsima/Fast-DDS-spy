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

#include <fastddsspy_participants/visualizer/NetworkVisualizer.hpp>

namespace eprosima {
namespace spy {
namespace participants {

void NetworkVisualizer::print_participants(
        std::ostream& target /* = std::cout */) const noexcept
{
    std::shared_lock<ParticipantInfoDatabase> _(participant_database_);

    target << "  Participants:\n";
    for (const auto& participant : participant_database_)
    {
        target << "    - " << participant.second.name << " [" << participant.first << "]\n";
    }
    target << std::endl;
}

void NetworkVisualizer::print_datareaders(
        std::ostream& target /* = std::cout */) const noexcept
{
    std::shared_lock<EndpointInfoDatabase> _(endpoint_database_);

    target << "  DataReaders:\n";
    for (const auto& endpoint : endpoint_database_)
    {
        if (endpoint.second.is_reader())
        {
            target << "    - " << endpoint.second.topic.topic_name() << " [" << endpoint.first << "]\n";
        }
    }
    target << std::endl;
}

void NetworkVisualizer::print_datawriters(
        std::ostream& target /* = std::cout */) const noexcept
{
    std::shared_lock<EndpointInfoDatabase> _(endpoint_database_);

    target << "  DataWriters:\n";
    for (const auto& endpoint : endpoint_database_)
    {
        if (endpoint.second.is_writer())
        {
            target << "    - " << endpoint.second.topic.topic_name() << " [" << endpoint.first << "]\n";
        }
    }
    target << std::endl;
}

void NetworkVisualizer::new_participant_info(const ParticipantInfo& info) noexcept
{
    std::unique_lock<ParticipantInfoDatabase> _(participant_database_);

    // First check if participant already in database
    auto it = participant_database_.find(info.guid);

    // If already exist, check if it must be removed
    // If not exists and active, add it to the database
    if (it != participant_database_.end() && !info.active)
    {
        participant_database_.erase(it);
    }
    else if (it == participant_database_.end() && info.active)
    {
        participant_database_.insert({info.guid, info});
    }
}

void NetworkVisualizer::new_endpoint_info(const EndpointInfo& info) noexcept
{
    std::unique_lock<EndpointInfoDatabase> _(endpoint_database_);

    // First check if endpoint already in database
    auto it = endpoint_database_.find(info.guid);

    // If already exist, check if it must be removed
    // If not exists and active, add it to the database
    if (it != endpoint_database_.end() && !info.active)
    {
        endpoint_database_.erase(it);
    }
    else if (it == endpoint_database_.end() && info.active)
    {
        endpoint_database_.insert({info.guid, info});
    }
}

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
