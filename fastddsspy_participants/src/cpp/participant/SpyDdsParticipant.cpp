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

SpyDdsParticipant::SpyDdsParticipantListener::SpyDdsParticipantListener(
        ddspipe::core::types::ParticipantId participant_id,
        std::shared_ptr<ddspipe::participants::InternalReader> type_object_reader,
        std::shared_ptr<ddspipe::participants::InternalReader> participants_reader,
        std::shared_ptr<ddspipe::participants::InternalReader> endpoints_reader,
        fastrtps::rtps::GUID_t rtps_guid)
    : ddspipe::participants::DynTypesParticipant::DynTypesDdsListener(type_object_reader, participant_id)
{
    // Set the internal readers
    participants_reader_ = participants_reader;
    endpoints_reader_ = endpoints_reader;
    // Set the participant guid prefixes
    rtps_guid = rtps_guid;
}

void SpyDdsParticipant::SpyDdsParticipantListener::on_participant_discovery(
        fastdds::dds::DomainParticipant* participant,
        fastrtps::rtps::ParticipantDiscoveryInfo&& discovery_info)
{
    // If comes from this participant is not interesting
    if (come_from_this_participant_(discovery_info.info.m_guid, participant->guid()))
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

void SpyDdsParticipant::SpyDdsParticipantListener::on_subscriber_discovery(
        fastdds::dds::DomainParticipant* participant,
        fastrtps::rtps::ReaderDiscoveryInfo&& info)
{
    // If comes from this participant is not interesting
    if (come_from_this_participant_(info.info.guid(), participant->guid()))
    {
        return;
    }

    EndpointInfo endpoint_info = ddspipe::participants::detail::create_endpoint_from_info_(info, participant_id_);

    // If participant left or dropped, this notification arrives as well
    endpoint_info.active = !(info.status == fastrtps::rtps::ReaderDiscoveryInfo::DISCOVERY_STATUS::REMOVED_READER);

    internal_notify_endpoint_discovered_(endpoint_info);
}

void SpyDdsParticipant::SpyDdsParticipantListener::on_publisher_discovery(
        fastdds::dds::DomainParticipant* participant,
        fastrtps::rtps::WriterDiscoveryInfo&& info)
{
    // If comes from this participant is not interesting
    if (come_from_this_participant_(info.info.guid(), participant->guid()))
    {
        return;
    }

    EndpointInfo endpoint_info = ddspipe::participants::detail::create_endpoint_from_info_(info, participant_id_);

    // If participant left or dropped, this notification arrives as well
    endpoint_info.active = !(info.status == fastrtps::rtps::WriterDiscoveryInfo::DISCOVERY_STATUS::REMOVED_WRITER);

    internal_notify_endpoint_discovered_(endpoint_info);
}

void SpyDdsParticipant::SpyDdsParticipantListener::internal_notify_participant_discovered_(
        const ParticipantInfo& participant_discovered)
{
    // Create data containing Dynamic Type
    auto data = std::make_unique<ParticipantInfoData>();
    data->info = participant_discovered;

    // Insert new data in internal reader queue
    participants_reader_->simulate_data_reception(std::move(data));
}

void SpyDdsParticipant::SpyDdsParticipantListener::internal_notify_endpoint_discovered_(
        const EndpointInfo& endpoint_discovered)
{
    // Create data containing Dynamic Type
    auto data = std::make_unique<EndpointInfoData>();
    data->info = endpoint_discovered;

    // Insert new data in internal reader queue
    endpoints_reader_->simulate_data_reception(std::move(data));
}

/*
 * NOTE: this function is required apart from come_from_same_participant_
 * because this participant has 2 guids, the rtps and the dds participant ones
 */
bool SpyDdsParticipant::SpyDdsParticipantListener::come_from_this_participant_(
        const ddspipe::core::types::Guid& guid,
        const ddspipe::core::types::Guid& guid_dds) const noexcept
{
    return (guid.guid_prefix() == guid_dds.guid_prefix()
           ||  guid.guid_prefix() == rtps_guid.guidPrefix
           );
}

std::unique_ptr<fastdds::dds::DomainParticipantListener> SpyDdsParticipant::create_dds_listener_()
{
    // We pass the configuration_ and discovery_database_ attributes from this method to avoid accessing virtual
    // attributes in the constructor
    return std::make_unique<SpyDdsParticipantListener>(configuration_->id, type_object_reader_,
                   participants_reader_, endpoints_reader_, rtps_participant_->getGuid());
}

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
