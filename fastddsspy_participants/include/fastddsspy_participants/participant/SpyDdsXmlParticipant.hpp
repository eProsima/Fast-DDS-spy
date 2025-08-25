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

#include <fastdds/rtps/builtin/data/ParticipantBuiltinTopicData.hpp>
#include <fastdds/rtps/builtin/data/PublicationBuiltinTopicData.hpp>
#include <fastdds/rtps/builtin/data/SubscriptionBuiltinTopicData.hpp>
#include <fastdds/rtps/participant/ParticipantDiscoveryInfo.hpp>
#include <fastdds/rtps/participant/RTPSParticipant.hpp>
#include <fastdds/rtps/reader/ReaderDiscoveryStatus.hpp>
#include <fastdds/rtps/writer/WriterDiscoveryStatus.hpp>

#include <ddspipe_participants/participant/dynamic_types/XmlDynTypesParticipant.hpp>
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
class SpyDdsXmlParticipant : public ddspipe::participants::<XmlDynTypesParticipant>
{
public:

    FASTDDSSPY_PARTICIPANTS_DllAPI
    SpyDdsXmlParticipant(
            const std::shared_ptr<ddspipe::participants::XmlParticipantConfiguration>& participant_configuration,
            const std::shared_ptr<ddspipe::core::PayloadPool>& payload_pool,
            const std::shared_ptr<ddspipe::core::DiscoveryDatabase>& discovery_database);

    FASTDDSSPY_PARTICIPANTS_DllAPI
    ~SpyDdsXmlParticipant() = default;

    //! Override create_reader_() IParticipant method
    FASTDDSSPY_PARTICIPANTS_DllAPI
    std::shared_ptr<ddspipe::core::IReader> create_reader(
            const ddspipe::core::ITopic& topic) override;

    class SpyDdsXmlParticipantListener : public ddspipe::participants::XmlDynTypesParticipant::XmlDynTypesDdsListener
    {
    public:

        FASTDDSSPY_PARTICIPANTS_DllAPI
        explicit SpyDdsXmlParticipantListener(
                std::shared_ptr<ddspipe::participants::SimpleParticipantConfiguration> conf,
                std::shared_ptr<ddspipe::core::DiscoveryDatabase> ddb,
                std::shared_ptr<ddspipe::participants::InternalReader> type_object_reader,
                std::shared_ptr<ddspipe::participants::InternalReader> participants_reader,
                std::shared_ptr<ddspipe::participants::InternalReader> endpoints_reader);

        FASTDDSSPY_PARTICIPANTS_DllAPI
        void on_participant_discovery(
                fastdds::dds::DomainParticipant* participant,
                fastdds::rtps::ParticipantDiscoveryStatus reason,
                const fastdds::rtps::ParticipantBuiltinTopicData& info,
                bool& /*should_be_ignored*/) override;

        FASTDDSSPY_PARTICIPANTS_DllAPI
        void on_data_reader_discovery(
                fastdds::dds::DomainParticipant* participant,
                fastdds::rtps::ReaderDiscoveryStatus reason,
                const fastdds::dds::SubscriptionBuiltinTopicData& info,
                bool& /*should_be_ignored*/) override;

        FASTDDSSPY_PARTICIPANTS_DllAPI
        void on_data_writer_discovery(
                fastdds::dds::DomainParticipant* participant,
                fastdds::rtps::WriterDiscoveryStatus reason,
                const fastdds::dds::PublicationBuiltinTopicData& info,
                bool& /*should_be_ignored*/) override;

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

protected:

    //! Override method from \c CommonParticipant to create the internal RTPS participant listener
    FASTDDSSPY_PARTICIPANTS_DllAPI
    std::unique_ptr<fastdds::dds::DomainParticipantListener> create_listener_() override;

    //! Participants Internal Reader
    std::shared_ptr<ddspipe::participants::InternalReader> participants_reader_;

    //! Endpoint Internal Reader
    std::shared_ptr<ddspipe::participants::InternalReader> endpoints_reader_;
};

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
