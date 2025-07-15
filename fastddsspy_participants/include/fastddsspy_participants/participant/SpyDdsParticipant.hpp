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

#pragma once

#include <ddspipe_participants/participant/dynamic_types/DynTypesParticipant.hpp>
#include <ddspipe_core/types/dds/Endpoint.hpp>

#include <fastddsspy_participants/library/library_dll.h>
#include <fastddsspy_participants/types/ParticipantInfo.hpp>
#include <fastddsspy_participants/types/EndpointInfo.hpp>

namespace eprosima {
namespace spy {
namespace participants {

/**
 * TODO comment
 */
class SpyDdsParticipant : public ddspipe::participants::DynTypesParticipant
{
public:

    FASTDDSSPY_PARTICIPANTS_DllAPI
    SpyDdsParticipant(
            const std::shared_ptr<ddspipe::participants::SimpleParticipantConfiguration>& participant_configuration,
            const std::shared_ptr<ddspipe::core::PayloadPool>& payload_pool,
            const std::shared_ptr<ddspipe::core::DiscoveryDatabase>& discovery_database);

    FASTDDSSPY_PARTICIPANTS_DllAPI
    ~SpyDdsParticipant() = default;

    //! Override create_reader_() IParticipant method
    FASTDDSSPY_PARTICIPANTS_DllAPI
    std::shared_ptr<ddspipe::core::IReader> create_reader(
            const ddspipe::core::ITopic& topic) override;

<<<<<<< HEAD
    FASTDDSSPY_PARTICIPANTS_DllAPI
    virtual void on_participant_discovery(
            fastdds::dds::DomainParticipant* participant,
            fastrtps::rtps::ParticipantDiscoveryInfo&& info) override;

    FASTDDSSPY_PARTICIPANTS_DllAPI
    virtual void on_subscriber_discovery(
            fastdds::dds::DomainParticipant* participant,
            fastrtps::rtps::ReaderDiscoveryInfo&& info);

    FASTDDSSPY_PARTICIPANTS_DllAPI
    virtual void on_publisher_discovery(
            fastdds::dds::DomainParticipant* participant,
            fastrtps::rtps::WriterDiscoveryInfo&& info);
=======
    class SpyDdsParticipantListener : public ddspipe::participants::DynTypesParticipant::DynTypesRtpsListener
    {
    public:

        FASTDDSSPY_PARTICIPANTS_DllAPI
        explicit SpyDdsParticipantListener(
                std::shared_ptr<ddspipe::participants::ParticipantConfiguration> conf,
                std::shared_ptr<ddspipe::core::DiscoveryDatabase> ddb,
                std::shared_ptr<ddspipe::participants::InternalReader> type_object_reader,
                std::shared_ptr<ddspipe::participants::InternalReader> participants_reader,
                std::shared_ptr<ddspipe::participants::InternalReader> endpoints_reader);

        FASTDDSSPY_PARTICIPANTS_DllAPI
        void on_participant_discovery(
                fastdds::rtps::RTPSParticipant* participant,
                fastdds::rtps::ParticipantDiscoveryStatus reason,
                const fastdds::rtps::ParticipantBuiltinTopicData& info,
                bool& should_be_ignored) override;

        FASTDDSSPY_PARTICIPANTS_DllAPI
        void on_reader_discovery(
                fastdds::rtps::RTPSParticipant* participant,
                fastdds::rtps::ReaderDiscoveryStatus reason,
                const fastdds::rtps::SubscriptionBuiltinTopicData& info,
                bool& should_be_ignored) override;

        FASTDDSSPY_PARTICIPANTS_DllAPI
        void on_writer_discovery(
                fastdds::rtps::RTPSParticipant* participant,
                fastdds::rtps::WriterDiscoveryStatus reason,
                const fastdds::rtps::PublicationBuiltinTopicData& info,
                bool& should_be_ignored) override;

    protected:

        void internal_notify_participant_discovered_(
                const ParticipantInfo& participant_discovered);

        void internal_notify_endpoint_discovered_(
                const EndpointInfo& endpoint_discovered);

        //! Participants Internal Reader
        std::shared_ptr<ddspipe::participants::InternalReader> participants_reader_;

        //! Endpoint Internal Reader
        std::shared_ptr<ddspipe::participants::InternalReader> endpoints_reader_;

    };
>>>>>>> 6a01d7d (Update Spy Listeners (#121))

protected:

    //! Override method from \c CommonParticipant to create the internal RTPS participant listener
    FASTDDSSPY_PARTICIPANTS_DllAPI
    std::unique_ptr<fastdds::rtps::RTPSParticipantListener> create_listener_() override;

    bool come_from_this_participant_(
            const ddspipe::core::types::Guid& guid) const noexcept;

    //! Participants Internal Reader
    std::shared_ptr<ddspipe::participants::InternalReader> participants_reader_;

    //! Endpoint Internal Reader
    std::shared_ptr<ddspipe::participants::InternalReader> endpoints_reader_;
};

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
