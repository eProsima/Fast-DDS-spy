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

#include <ddspipe_participants/participant/rtps/CommonParticipant.hpp>

#include <fastddsspy_participants/participant/SpyParticipant.hpp>
#include <fastddsspy_participants/types/ParticipantInfo.hpp>
#include <fastddsspy_participants/types/EndpointInfo.hpp>

namespace eprosima {
namespace spy {
namespace participants {

SpyParticipant::SpyParticipant(
        const std::shared_ptr<ddspipe::participants::ParticipantConfiguration>& participant_configuration,
        const std::shared_ptr<ddspipe::core::PayloadPool>& payload_pool,
        const std::shared_ptr<ddspipe::core::DiscoveryDatabase>& discovery_database,
        const std::shared_ptr<SpyModel>& model)
    : ddspipe::participants::SchemaParticipant(participant_configuration, payload_pool, discovery_database, model)
    , model_(model)
{
    // TODO: study why couldn't do with bind and try it, better than create a lambda
    auto participant_callback = [this](ddspipe::core::IRoutingData& data)
            {
                return this->new_participant_info_(data);
            };
    participants_writer_ = std::make_shared<InternalWriter>(
        participant_configuration->id,
        participant_callback);

    auto endpoint_callback = [this](ddspipe::core::IRoutingData& data)
            {
                return this->new_endpoint_info_(data);
            };
    endpoints_writer_ = std::make_shared<InternalWriter>(
        participant_configuration->id,
        endpoint_callback);

    // Simulate that there is a reader of participants to force this track creation
    discovery_database_->add_endpoint(
        ddspipe::participants::rtps::CommonParticipant::simulate_endpoint(participant_info_topic(), this->id())
        );

    // Simulate that there is a reader of endpoints to force this track creation
    discovery_database_->add_endpoint(
        ddspipe::participants::rtps::CommonParticipant::simulate_endpoint(endpoint_info_topic(), this->id())
        );
}

std::shared_ptr<ddspipe::core::IWriter> SpyParticipant::create_writer(
        const ddspipe::core::ITopic& topic)
{
    if (is_participant_info_topic(topic))
    {
        return participants_writer_;
    }
    else if (is_endpoint_info_topic(topic))
    {
        return endpoints_writer_;
    }
    else
    {
        return ddspipe::participants::SchemaParticipant::create_writer(topic);
    }
}

utils::ReturnCode SpyParticipant::new_participant_info_(
        const ddspipe::core::IRoutingData& data)
{
    // Assuming that data is of type required
    auto& participant_info = dynamic_cast<const ParticipantInfoData&>(data);
    ParticipantInfo info = participant_info.info;
    ddspipe::core::types::Guid guid = info.guid;
    model_->participant_database_.add_or_modify(std::move(guid), std::move(info));
    return utils::ReturnCode::RETCODE_OK;
}

utils::ReturnCode SpyParticipant::new_endpoint_info_(
        const ddspipe::core::IRoutingData& data)
{
    // Assuming that data is of type required
    auto& endpoint_info = dynamic_cast<const EndpointInfoData&>(data);
    EndpointInfo info = endpoint_info.info;
    ddspipe::core::types::Guid guid = info.guid;
    model_->endpoint_database_.add_or_modify(std::move(guid), std::move(info));
    return utils::ReturnCode::RETCODE_OK;
}

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
