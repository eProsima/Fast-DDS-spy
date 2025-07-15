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

<<<<<<< HEAD
void SpyDdsParticipant::on_participant_discovery(
        fastdds::dds::DomainParticipant* participant,
        fastrtps::rtps::ParticipantDiscoveryInfo&& discovery_info)
{
    // If comes from this participant is not interesting
    if (come_from_this_participant_(discovery_info.info.m_guid))
=======
SpyDdsParticipant::SpyDdsParticipantListener::SpyDdsParticipantListener(
        std::shared_ptr<ddspipe::participants::ParticipantConfiguration> conf,
        std::shared_ptr<ddspipe::core::DiscoveryDatabase> ddb,
        std::shared_ptr<ddspipe::participants::InternalReader> type_object_reader,
        std::shared_ptr<ddspipe::participants::InternalReader> participants_reader,
        std::shared_ptr<ddspipe::participants::InternalReader> endpoints_reader)
    : ddspipe::participants::DynTypesParticipant::DynTypesRtpsListener(conf, ddb, type_object_reader)
{
    // Set the internal readers
    participants_reader_ = participants_reader;
    endpoints_reader_ = endpoints_reader;
}

void SpyDdsParticipant::SpyDdsParticipantListener::on_participant_discovery(
        fastdds::rtps::RTPSParticipant* participant,
        fastdds::rtps::ParticipantDiscoveryStatus reason,
        const fastdds::rtps::ParticipantBuiltinTopicData& info,
        bool& should_be_ignored)
{
    // If comes from this participant is not interesting
    if (ddspipe::participants::detail::come_from_same_participant_(info.guid, participant->getGuid()))
>>>>>>> 6a01d7d (Update Spy Listeners (#121))
    {
        return;
    }

    ParticipantInfo info;
    info.active = (discovery_info.status == eprosima::fastrtps::rtps::ParticipantDiscoveryInfo::DISCOVERED_PARTICIPANT
            || discovery_info.status == eprosima::fastrtps::rtps::ParticipantDiscoveryInfo::CHANGED_QOS_PARTICIPANT);
    info.name = std::string(discovery_info.info.m_participantName);
    info.guid = discovery_info.info.m_guid;

<<<<<<< HEAD
    internal_notify_participant_discovered_(info);
}

void SpyDdsParticipant::on_subscriber_discovery(
        fastdds::dds::DomainParticipant* participant,
        fastrtps::rtps::ReaderDiscoveryInfo&& info)
{
    // If comes from this participant is not interesting
    if (come_from_this_participant_(info.info.guid()))
=======
    ddspipe::participants::rtps::CommonParticipant::RtpsListener::on_participant_discovery(participant, reason, info,
            should_be_ignored);

    internal_notify_participant_discovered_(participant_info);
}

void SpyDdsParticipant::SpyDdsParticipantListener::on_reader_discovery(
        fastdds::rtps::RTPSParticipant* participant,
        fastdds::rtps::ReaderDiscoveryStatus reason,
        const fastdds::rtps::SubscriptionBuiltinTopicData& info,
        bool& should_be_ignored)
{
    // If comes from this participant is not interesting
    if (ddspipe::participants::detail::come_from_same_participant_(info.guid, participant->getGuid()))
>>>>>>> 6a01d7d (Update Spy Listeners (#121))
    {
        return;
    }

<<<<<<< HEAD
    EndpointInfo endpoint_info = ddspipe::participants::detail::create_endpoint_from_info_(info, id());

    // If participant left or dropped, this notification arrives as well
    endpoint_info.active = !(info.status == fastrtps::rtps::ReaderDiscoveryInfo::DISCOVERY_STATUS::REMOVED_READER);
=======
    EndpointInfo endpoint_info = ddspipe::participants::detail::create_endpoint_from_info_(info, configuration_->id);
    endpoint_info.active = (reason == fastdds::rtps::ReaderDiscoveryStatus::DISCOVERED_READER
            || reason == fastdds::rtps::ReaderDiscoveryStatus::CHANGED_QOS_READER);

    ddspipe::participants::DynTypesParticipant::DynTypesRtpsListener::on_reader_discovery(participant, reason, info,
            should_be_ignored);
>>>>>>> 6a01d7d (Update Spy Listeners (#121))

    internal_notify_endpoint_discovered_(endpoint_info);
}

<<<<<<< HEAD
void SpyDdsParticipant::on_publisher_discovery(
        fastdds::dds::DomainParticipant* participant,
        fastrtps::rtps::WriterDiscoveryInfo&& info)
{
    // If comes from this participant is not interesting
    if (come_from_this_participant_(info.info.guid()))
=======
void SpyDdsParticipant::SpyDdsParticipantListener::on_writer_discovery(
        fastdds::rtps::RTPSParticipant* participant,
        fastdds::rtps::WriterDiscoveryStatus reason,
        const fastdds::rtps::PublicationBuiltinTopicData& info,
        bool& should_be_ignored)
{
    // If comes from this participant is not interesting
    if (ddspipe::participants::detail::come_from_same_participant_(info.guid, participant->getGuid()))
>>>>>>> 6a01d7d (Update Spy Listeners (#121))
    {
        return;
    }

<<<<<<< HEAD
    EndpointInfo endpoint_info = ddspipe::participants::detail::create_endpoint_from_info_(info, id());

    // If participant left or dropped, this notification arrives as well
    endpoint_info.active = !(info.status == fastrtps::rtps::WriterDiscoveryInfo::DISCOVERY_STATUS::REMOVED_WRITER);
=======
    EndpointInfo endpoint_info = ddspipe::participants::detail::create_endpoint_from_info_(info, configuration_->id);
    endpoint_info.active = (reason == fastdds::rtps::WriterDiscoveryStatus::DISCOVERED_WRITER
            || reason == fastdds::rtps::WriterDiscoveryStatus::CHANGED_QOS_WRITER);

    ddspipe::participants::DynTypesParticipant::DynTypesRtpsListener::on_writer_discovery(participant, reason, info,
            should_be_ignored);
>>>>>>> 6a01d7d (Update Spy Listeners (#121))

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

<<<<<<< HEAD
/*
 * NOTE: this function is required apart from come_from_same_participant_
 * because this participant has 2 guids, the rtps and the dds participant ones
 */
bool SpyDdsParticipant::come_from_this_participant_(
        const ddspipe::core::types::Guid& guid) const noexcept
{
    return (guid.guid_prefix() == dds_participant_->guid().guidPrefix
           ||  guid.guid_prefix() == rtps_participant_->getGuid().guidPrefix
           );
=======
std::unique_ptr<fastdds::rtps::RTPSParticipantListener> SpyDdsParticipant::create_listener_()
{
    // We pass the configuration_ and discovery_database_ attributes from this method to avoid accessing virtual
    // attributes in the constructor
    return std::make_unique<SpyDdsParticipantListener>(configuration_, discovery_database_, type_object_reader_,
                   participants_reader_, endpoints_reader_);
>>>>>>> 6a01d7d (Update Spy Listeners (#121))
}

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
