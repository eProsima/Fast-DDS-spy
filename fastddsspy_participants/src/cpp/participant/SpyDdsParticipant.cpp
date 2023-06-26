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

#include <fastdds/rtps/participant/RTPSParticipant.h>

#include <ddspipe_participants/utils/utils.hpp>

#include <fastddsspy_participants/participant/SpyDdsParticipant.hpp>
#include <fastddsspy_participants/types/ParticipantInfo.hpp>
#include <fastddsspy_participants/types/EndpointInfo.hpp>

namespace eprosima {
namespace spy {
namespace participants {

SpyDdsParticipant::SpyDdsParticipant(
        const std::shared_ptr<ddspipe::participants::SimpleParticipantConfiguration>& participant_configuration,
        const std::shared_ptr<ddspipe::core::PayloadPool>& payload_pool,
        const std::shared_ptr<ddspipe::core::DiscoveryDatabase>& discovery_database)
    : ddspipe::participants::DynTypesParticipant(participant_configuration, payload_pool, discovery_database)
    , participants_reader_(std::make_shared<ddspipe::participants::InternalReader>(
                this->id()))
    , endpoints_reader_(std::make_shared<ddspipe::participants::InternalReader>(
                this->id()))
{
    // Do nothing
}

std::shared_ptr<ddspipe::core::IReader> SpyDdsParticipant::create_reader(
        const ddspipe::core::ITopic& topic)
{
    // If participant info topic, return the internal reader for it
    if (is_participant_info_topic(topic))
    {
        return this->participants_reader_;
    }

    // If endpoint info topic, return the internal reader for it
    if (is_endpoint_info_topic(topic))
    {
        return this->endpoints_reader_;
    }

    // If not type object, use the parent method
    return ddspipe::participants::DynTypesParticipant::create_reader(topic);
}

void SpyDdsParticipant::on_participant_discovery(
        fastdds::dds::DomainParticipant* participant,
        fastrtps::rtps::ParticipantDiscoveryInfo&& discovery_info)
{
    // If comes from this participant is not interesting
    if (ddspipe::participants::detail::come_from_same_participant_(discovery_info.info.m_guid, this->dds_participant_->guid()))
    {
        return;
    }

    ParticipantInfo info;
    info.active = (discovery_info.status == eprosima::fastrtps::rtps::ParticipantDiscoveryInfo::DISCOVERED_PARTICIPANT
            || discovery_info.status == eprosima::fastrtps::rtps::ParticipantDiscoveryInfo::CHANGED_QOS_PARTICIPANT);
    info.name = std::string(discovery_info.info.m_participantName);
    info.guid = discovery_info.info.m_guid;

    internal_notify_participant_discovered_(info);
}

void SpyDdsParticipant::on_subscriber_discovery(
        fastdds::dds::DomainParticipant* participant,
        fastrtps::rtps::ReaderDiscoveryInfo&& info)
{
    // If comes from this participant is not interesting
    if (ddspipe::participants::detail::come_from_same_participant_(info.info.guid(), this->dds_participant_->guid()))
    {
        return;
    }

    EndpointInfo endpoint_info = ddspipe::participants::detail::create_endpoint_from_info_(info, id());

    // If participant left or dropped, this notification arrives as well
    endpoint_info.active = !(info.status == fastrtps::rtps::ReaderDiscoveryInfo::DISCOVERY_STATUS::REMOVED_READER);

    internal_notify_endpoint_discovered_(endpoint_info);
}

void SpyDdsParticipant::on_publisher_discovery(
        fastdds::dds::DomainParticipant* participant,
        fastrtps::rtps::WriterDiscoveryInfo&& info)
{
    // If comes from this participant is not interesting
    if (ddspipe::participants::detail::come_from_same_participant_(info.info.guid(), this->dds_participant_->guid()))
    {
        return;
    }

    EndpointInfo endpoint_info = ddspipe::participants::detail::create_endpoint_from_info_(info, id());

    // If participant left or dropped, this notification arrives as well
    endpoint_info.active = !(info.status == fastrtps::rtps::WriterDiscoveryInfo::DISCOVERY_STATUS::REMOVED_WRITER);

    internal_notify_endpoint_discovered_(endpoint_info);
}

void SpyDdsParticipant::internal_notify_participant_discovered_(
        const ParticipantInfo& participant_discovered)
{
    // Create data containing Dynamic Type
    auto data = std::make_unique<ParticipantInfoData>();
    data->info = participant_discovered;

    // Insert new data in internal reader queue
    participants_reader_->simulate_data_reception(std::move(data));
}

void SpyDdsParticipant::internal_notify_endpoint_discovered_(
        const EndpointInfo& endpoint_discovered)
{
    // Create data containing Dynamic Type
    auto data = std::make_unique<EndpointInfoData>();
    data->info = endpoint_discovered;

    // Insert new data in internal reader queue
    endpoints_reader_->simulate_data_reception(std::move(data));
}

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
