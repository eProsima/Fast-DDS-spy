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

#include <ddspipe_participants/utils/utils.hpp>

#include <fastddsspy_participants/participant/SpyDdsParticipant.hpp>
#include <fastddsspy_participants/types/ParticipantInfo.hpp>
#include <fastddsspy_participants/types/EndpointInfo.hpp>

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicTypeBuilderFactory.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicTypeBuilder.hpp>

#include <ddspipe_core/types/dynamic_types/types.hpp>
#include <fastdds/dds/xtypes/utils.hpp>
namespace eprosima {
namespace spy {
namespace participants {

SpyDdsParticipant::SpyDdsParticipant(
        const std::shared_ptr<ddspipe::participants::SimpleParticipantConfiguration>& participant_configuration,
        const std::shared_ptr<ddspipe::core::PayloadPool>& payload_pool,
        const std::shared_ptr<ddspipe::core::DiscoveryDatabase>& discovery_database)
    : ddspipe::participants::DynTypesParticipant(participant_configuration, payload_pool, discovery_database, true) // TODO. danip
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
    {
        return;
    }

    ParticipantInfo participant_info;
    participant_info.active = (reason == fastdds::rtps::ParticipantDiscoveryStatus::DISCOVERED_PARTICIPANT
            || reason == fastdds::rtps::ParticipantDiscoveryStatus::CHANGED_QOS_PARTICIPANT);
    participant_info.name = std::string(info.participant_name);
    participant_info.guid = info.guid;

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
    {
        return;
    }

    EndpointInfo endpoint_info = ddspipe::participants::detail::create_endpoint_from_info_(info, configuration_->id);
    endpoint_info.active = (reason == fastdds::rtps::ReaderDiscoveryStatus::DISCOVERED_READER
            || reason == fastdds::rtps::ReaderDiscoveryStatus::CHANGED_QOS_READER);

    ddspipe::participants::DynTypesParticipant::DynTypesRtpsListener::on_reader_discovery(participant, reason, info,
            should_be_ignored);

    std::string type_idl {};

    if (info.type_information.assigned())
    {
        fastdds::dds::xtypes::TypeObject remote_type_object;
        if (eprosima::fastdds::dds::RETCODE_OK !=
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_object
                (
                    info.type_information.type_information.complete().typeid_with_size().type_id(),
                    remote_type_object))
        {
            EPROSIMA_LOG_WARNING(FASTDDSSPY_DDS_PARTICIPANT,
                    "Error getting type object for type " << info.type_name);
        }
        else
        {
            // Build remotely discovered type
            fastdds::dds::DynamicType::_ref_type remote_type =
                    eprosima::fastdds::dds::DynamicTypeBuilderFactory::get_instance()->create_type_w_type_object(
                remote_type_object)->build();

            // Serialize DynamicType into its IDL representation
            std::stringstream idl;
            idl_serialize(remote_type, idl);
            type_idl = idl.str();
        }
    }

    internal_notify_endpoint_discovered_(endpoint_info, type_idl);
}

void SpyDdsParticipant::SpyDdsParticipantListener::on_writer_discovery(
        fastdds::rtps::RTPSParticipant* participant,
        fastdds::rtps::WriterDiscoveryStatus reason,
        const fastdds::rtps::PublicationBuiltinTopicData& info,
        bool& should_be_ignored)
{
    // If comes from this participant is not interesting
    if (ddspipe::participants::detail::come_from_same_participant_(info.guid, participant->getGuid()))
    {
        return;
    }

    EndpointInfo endpoint_info = ddspipe::participants::detail::create_endpoint_from_info_(info, configuration_->id);
    endpoint_info.active = (reason == fastdds::rtps::WriterDiscoveryStatus::DISCOVERED_WRITER
            || reason == fastdds::rtps::WriterDiscoveryStatus::CHANGED_QOS_WRITER);

    ddspipe::participants::DynTypesParticipant::DynTypesRtpsListener::on_writer_discovery(participant, reason, info,
            should_be_ignored);

    std::string type_idl {};

    if (info.type_information.assigned())
    {
        fastdds::dds::xtypes::TypeObject remote_type_object;
        if (eprosima::fastdds::dds::RETCODE_OK !=
                eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_object
                (
                    info.type_information.type_information.complete().typeid_with_size().type_id(),
                    remote_type_object))
        {
            EPROSIMA_LOG_WARNING(FASTDDSSPY_DDS_PARTICIPANT,
                    "Error getting type object for type " << info.type_name);
        }

        // Build remotely discovered type
        fastdds::dds::DynamicType::_ref_type remote_type =
                eprosima::fastdds::dds::DynamicTypeBuilderFactory::get_instance()->create_type_w_type_object(
            remote_type_object)->build();

        // Serialize DynamicType into its IDL representation
        std::stringstream idl;
        idl_serialize(remote_type, idl);
        type_idl = idl.str();
    }
    internal_notify_endpoint_discovered_(endpoint_info, type_idl);
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
        const EndpointInfo& endpoint_discovered,
        const std::string& type_idl)
{
    // Create data containing Dynamic Type
    auto data = std::make_unique<EndpointInfoData>();
    data->info = endpoint_discovered;
    data->type_idl = type_idl;

    // Insert new data in internal reader queue
    endpoints_reader_->simulate_data_reception(std::move(data));
}

std::unique_ptr<fastdds::rtps::RTPSParticipantListener> SpyDdsParticipant::create_listener_()
{
    // We pass the configuration_ and discovery_database_ attributes from this method to avoid accessing virtual
    // attributes in the constructor
    return std::make_unique<SpyDdsParticipantListener>(configuration_, discovery_database_, type_object_reader_,
                   participants_reader_, endpoints_reader_);
}

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
