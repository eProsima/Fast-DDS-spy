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

#include <fastdds/rtps/builtin/data/ParticipantProxyData.hpp>
#include <fastdds/rtps/builtin/data/ReaderProxyData.hpp>
#include <fastdds/rtps/builtin/data/WriterProxyData.hpp>
#include <fastdds/rtps/participant/RTPSParticipant.hpp>

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

void SpyDdsParticipant::onParticipantDiscovery(
        fastdds::rtps::RTPSParticipant* participant,
        fastdds::rtps::ParticipantDiscoveryInfo&& discovery_info,
        bool& should_be_ignored)
{
    fastdds::rtps::ParticipantProxyData proxy_copy_info(discovery_info.info);

    // If comes from this participant is not interesting
    if (come_from_this_participant_(proxy_copy_info.m_guid))
    {
        return;
    }

    ParticipantInfo info;
    info.active = (discovery_info.status == eprosima::fastdds::rtps::ParticipantDiscoveryInfo::DISCOVERED_PARTICIPANT
            || discovery_info.status == eprosima::fastdds::rtps::ParticipantDiscoveryInfo::CHANGED_QOS_PARTICIPANT);
    info.name = std::string(proxy_copy_info.m_participantName);
    info.guid = proxy_copy_info.m_guid;

    ddspipe::participants::rtps::CommonParticipant::onParticipantDiscovery(participant, std::move(discovery_info), should_be_ignored);

    internal_notify_participant_discovered_(info);
}

void SpyDdsParticipant::onReaderDiscovery(
        fastdds::rtps::RTPSParticipant* participant,
        fastdds::rtps::ReaderDiscoveryInfo&& info,
        bool& should_be_ignored)
{
    fastdds::rtps::ReaderProxyData proxy_copy(info.info);

    // If comes from this participant is not interesting
    if (come_from_this_participant_(proxy_copy.guid()))
    {
        return;
    }

    EndpointInfo endpoint_info = ddspipe::participants::detail::create_endpoint_from_info_(info, id());

    // If participant left or dropped, this notification arrives as well
    endpoint_info.active = !(info.status == fastdds::rtps::ReaderDiscoveryInfo::DISCOVERY_STATUS::REMOVED_READER);

    ddspipe::participants::DynTypesParticipant::onReaderDiscovery(participant, std::move(info), should_be_ignored);

    internal_notify_endpoint_discovered_(endpoint_info);
}

void SpyDdsParticipant::onWriterDiscovery(
        fastdds::rtps::RTPSParticipant* participant,
        fastdds::rtps::WriterDiscoveryInfo&& info,
        bool& should_be_ignored)
{
    fastdds::rtps::WriterProxyData proxy_copy(info.info);

    // If comes from this participant is not interesting
    if (come_from_this_participant_(proxy_copy.guid()))
    {
        return;
    }

    EndpointInfo endpoint_info = ddspipe::participants::detail::create_endpoint_from_info_(info, id());

    // If participant left or dropped, this notification arrives as well
    endpoint_info.active = !(info.status == fastdds::rtps::WriterDiscoveryInfo::DISCOVERY_STATUS::REMOVED_WRITER);

    ddspipe::participants::DynTypesParticipant::onWriterDiscovery(participant, std::move(info), should_be_ignored);

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

/*
 * NOTE: this function is required apart from come_from_same_participant_
 * because this participant has 2 guids, the rtps and the dds participant ones
 */
bool SpyDdsParticipant::come_from_this_participant_(
        const ddspipe::core::types::Guid& guid) const noexcept
{
    return (guid.guid_prefix() == rtps_participant_->getGuid().guidPrefix);
}

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
