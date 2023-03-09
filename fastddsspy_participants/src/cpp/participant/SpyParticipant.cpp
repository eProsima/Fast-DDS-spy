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

#include <fastddsspy_participants/participant/SpyParticipant.hpp>
#include <fastddsspy_participants/types/ParticipantInfoData.hpp>

namespace eprosima {
namespace spy {
namespace participants {

SpyParticipant::SpyParticipant(
        const std::shared_ptr<ddspipe::participants::ParticipantConfiguration>& participant_configuration,
        const std::shared_ptr<ddspipe::core::PayloadPool>& payload_pool,
        const std::shared_ptr<ddspipe::core::DiscoveryDatabase>& discovery_database,
        const std::shared_ptr<SpyVisualizer>& visualizer)
    : ddspipe::participants::SchemaParticipant(participant_configuration, payload_pool, discovery_database, visualizer)
    , visualizer_(visualizer)
{
    // TODO: study why couldn't do with bind and try it, better than create a lambda
    auto callback = [this](ddspipe::core::IRoutingData& data){ this->new_participant_info_(data); };
    participants_writer_ = std::make_shared<InternalWriter>(
        participant_configuration->id,
        callback);

    // Simulate that there is a reader of participants to force this track creation
    discovery_database_->add_endpoint(
        simulate_endpoint_(participant_info_topic())
        );
}

std::shared_ptr<ddspipe::core::IWriter> SpyParticipant::create_writer(
        const ddspipe::core::ITopic& topic)
{
    if (is_participant_info_topic(topic))
    {
        return participants_writer_;
    }
    else
    {
        return ddspipe::participants::SchemaParticipant::create_writer(topic);
    }
}

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
