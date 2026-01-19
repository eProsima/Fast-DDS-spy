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

/**
 * @file SpyDdsXmlParticipant.hpp
 * @brief This file contains the definition of the SpyDdsXmlParticipant class and its listener.
 */

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
 * @class SpyDdsXmlParticipant
 * @brief A specialized participant that extends XmlDynTypesParticipant to handle DDS discovery data.
 */
class SpyDdsXmlParticipant : public ddspipe::participants::XmlDynTypesParticipant
{
public:

    /**
     * @brief Constructor for SpyDdsXmlParticipant.
     *
     * Initializes the internal readers for participants and endpoints.
     *
     * @param participant_configuration Shared pointer to the participant configuration.
     * @param payload_pool Shared pointer to the payload pool.
     * @param discovery_database Shared pointer to the discovery database.
     */
    FASTDDSSPY_PARTICIPANTS_DllAPI
    SpyDdsXmlParticipant(
            const std::shared_ptr<ddspipe::participants::XmlParticipantConfiguration>& participant_configuration,
            const std::shared_ptr<ddspipe::core::PayloadPool>& payload_pool,
            const std::shared_ptr<ddspipe::core::DiscoveryDatabase>& discovery_database);

    /**
     * @brief Default destructor.
     */
    FASTDDSSPY_PARTICIPANTS_DllAPI
    ~SpyDdsXmlParticipant() = default;

    /**
     * @brief Creates a reader for the given topic.
     *
     * Depending on the topic type, returns the appropriate internal reader or delegates to the parent class.
     *
     * @param topic The topic for which the reader is created.
     * @return A shared pointer to the created reader.
     */
    FASTDDSSPY_PARTICIPANTS_DllAPI
    std::shared_ptr<ddspipe::core::IReader> create_reader(
            const ddspipe::core::ITopic& topic) override;

    /**
     * @class SpyDdsXmlParticipantListener
     * @brief Listener for handling participant and endpoint discovery events.
     */
    class SpyDdsXmlParticipantListener : public ddspipe::participants::XmlDynTypesParticipant::XmlDynTypesDdsListener
    {
    public:

        /**
         * @brief Constructor for SpyDdsXmlParticipantListener.
         *
         * Sets the internal readers for participants and endpoints.
         *
         * @param conf Shared pointer to the participant configuration.
         * @param ddb Shared pointer to the discovery database.
         * @param type_object_reader Shared pointer to the type object reader.
         * @param participants_reader Shared pointer to the participants reader.
         * @param endpoints_reader Shared pointer to the endpoints reader.
         */
        FASTDDSSPY_PARTICIPANTS_DllAPI
        explicit SpyDdsXmlParticipantListener(
                std::shared_ptr<ddspipe::participants::SimpleParticipantConfiguration> conf,
                std::shared_ptr<ddspipe::core::DiscoveryDatabase> ddb,
                std::shared_ptr<ddspipe::participants::InternalReader> type_object_reader,
                std::shared_ptr<ddspipe::participants::InternalReader> participants_reader,
                std::shared_ptr<ddspipe::participants::InternalReader> endpoints_reader);

        /**
         * @brief Callback for participant discovery events.
         *
         * Processes the discovery event and notifies the internal reader.
         *
         * @param participant The domain participant.
         * @param reason The discovery reason.
         * @param info The discovered participant information.
         * @param should_be_ignored Whether the participant should be ignored.
         */
        FASTDDSSPY_PARTICIPANTS_DllAPI
        void on_participant_discovery(
                fastdds::dds::DomainParticipant* participant,
                fastdds::rtps::ParticipantDiscoveryStatus reason,
                const fastdds::rtps::ParticipantBuiltinTopicData& info,
                bool& /*should_be_ignored*/) override;

        /**
         * @brief Callback for data reader discovery events.
         *
         * Processes the discovery event and notifies the internal reader.
         *
         * @param participant The domain participant.
         * @param reason The discovery reason.
         * @param info The discovered subscription information.
         * @param should_be_ignored Whether the reader should be ignored.
         */
        FASTDDSSPY_PARTICIPANTS_DllAPI
        void on_data_reader_discovery(
                fastdds::dds::DomainParticipant* participant,
                fastdds::rtps::ReaderDiscoveryStatus reason,
                const fastdds::dds::SubscriptionBuiltinTopicData& info,
                bool& /*should_be_ignored*/) override;

        /**
         * @brief Callback for data writer discovery events.
         *
         * Processes the discovery event and notifies the internal reader.
         *
         * @param participant The domain participant.
         * @param reason The discovery reason.
         * @param info The discovered publication information.
         * @param should_be_ignored Whether the writer should be ignored.
         */
        FASTDDSSPY_PARTICIPANTS_DllAPI
        void on_data_writer_discovery(
                fastdds::dds::DomainParticipant* participant,
                fastdds::rtps::WriterDiscoveryStatus reason,
                const fastdds::dds::PublicationBuiltinTopicData& info,
                bool& /*should_be_ignored*/) override;

    protected:

        /**
         * @brief Notify that a participant has been discovered.
         *
         * Creates data containing the discovered participant and inserts it into the internal reader queue.
         *
         * @param participant_discovered The discovered participant information.
         */
        void internal_notify_participant_discovered_(
                const ParticipantInfo& participant_discovered);

        /**
         * @brief Notify that an endpoint has been discovered.
         *
         * Creates data containing the discovered endpoint and inserts it into the internal reader queue.
         *
         * @param endpoint_discovered The discovered endpoint information.
         */
        void internal_notify_endpoint_discovered_(
                const EndpointInfo& endpoint_discovered);

        /// Participants Internal Reader
        std::shared_ptr<ddspipe::participants::InternalReader> participants_reader_;

        /// Endpoint Internal Reader
        std::shared_ptr<ddspipe::participants::InternalReader> endpoints_reader_;

    };


    // TODO. danip
    void update_filters(
        const int flag,
        std::set<std::string> partitions = std::set<std::string>(),
        const std::string& topic_name = "",
        const std::string& expression = "");

protected:

    /**
     * @brief Creates the internal RTPS participant listener.
     *
     * Passes the configuration and discovery database attributes to avoid accessing virtual attributes in the constructor.
     *
     * @return A unique pointer to the created listener.
     */
    FASTDDSSPY_PARTICIPANTS_DllAPI
    std::unique_ptr<fastdds::dds::DomainParticipantListener> create_listener_() override;

    /// Participants Internal Reader
    std::shared_ptr<ddspipe::participants::InternalReader> participants_reader_;

    /// Endpoint Internal Reader
    std::shared_ptr<ddspipe::participants::InternalReader> endpoints_reader_;

    // Filter partitions set
    std::set<std::string> partition_filter_set_;
    // Filter content_topicfilter dict
    std::map<std::string, std::string> topic_filter_dict_;
};

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
