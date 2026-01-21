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

#include <ddspipe_core/configuration/DdsPipeConfiguration.hpp>
#include <ddspipe_core/types/dynamic_types/types.hpp>

#include "Backend.hpp"

namespace eprosima {
namespace spy {

Backend::Backend(
        const yaml::Configuration& configuration)
    : configuration_(configuration)
    , payload_pool_(std::make_shared<ddspipe::core::FastPayloadPool>())
    , discovery_database_(std::make_shared<ddspipe::core::DiscoveryDatabase>())
    , thread_pool_(
        std::make_shared<utils::SlotThreadPool>(
            configuration.n_threads))
    , participant_database_(std::make_shared<ddspipe::core::ParticipantsDatabase>())
    , model_(
        std::make_shared<participants::SpyModel>(configuration.ros2_types))
    , spy_participant_(
        std::make_shared<participants::SpyParticipant>(
            configuration.spy_configuration,
            payload_pool_,
            discovery_database_,
            model_))
{
    // Check configuration
    utils::Formatter error_msg;
    if (!configuration_.is_valid(error_msg))
    {
        throw utils::ConfigurationException(error_msg);
    }

    load_internal_topics_(configuration_);

    if (configuration.xml_enabled)
    {
        dds_participant_ = std::make_shared<participants::SpyDdsXmlParticipant>(
            configuration.dds_configuration,
            payload_pool_,
            discovery_database_);

        std::dynamic_pointer_cast<participants::SpyDdsXmlParticipant>(dds_participant_)->init();
    }
    else
    {
        dds_participant_ = std::make_shared<participants::SpyDdsParticipant>(
            std::dynamic_pointer_cast<ddspipe::participants::SimpleParticipantConfiguration>(configuration.
                    dds_configuration),
            payload_pool_,
            discovery_database_);

        std::dynamic_pointer_cast<participants::SpyDdsParticipant>(dds_participant_)->init();
    }

    // Populate Participant Database
    participant_database_->add_participant(
        dds_participant_->id(),
        dds_participant_
        );
    participant_database_->add_participant(
        spy_participant_->id(),
        spy_participant_
        );

    // Create and initialize Pipe
    pipe_ = std::make_unique<ddspipe::core::DdsPipe>(
        configuration_.ddspipe_configuration,
        discovery_database_,
        payload_pool_,
        participant_database_,
        thread_pool_);

    pipe_->enable();
}

Backend::~Backend()
{
    pipe_->disable();
}

void Backend::load_internal_topics_(
        yaml::Configuration& configuration)
{
    // Create a built-in topic to transmit participant information
    configuration.ddspipe_configuration.builtin_topics.insert(
        utils::Heritable<eprosima::ddspipe::core::types::DdsTopic>::make_heritable(
            spy::participants::participant_info_topic()));

    // Create a built-in topic to transmit endpoint information
    configuration.ddspipe_configuration.builtin_topics.insert(
        utils::Heritable<eprosima::ddspipe::core::types::DdsTopic>::make_heritable(
            spy::participants::endpoint_info_topic()));

    // Create an internal topic to transmit the dynamic types
    configuration.ddspipe_configuration.builtin_topics.insert(
        utils::Heritable<eprosima::ddspipe::core::types::DdsTopic>::make_heritable(
            eprosima::ddspipe::core::types::type_object_topic()));

    if (!configuration.ddspipe_configuration.allowlist.empty())
    {
        // The allowlist is not empty. Add the internal topics.
        eprosima::ddspipe::core::types::WildcardDdsFilterTopic type_object_topic;
        type_object_topic.topic_name.set_value(eprosima::ddspipe::core::types::TYPE_OBJECT_TOPIC_NAME);

        configuration.ddspipe_configuration.allowlist.insert(
            utils::Heritable<eprosima::ddspipe::core::types::WildcardDdsFilterTopic>::make_heritable(
                type_object_topic));

        eprosima::ddspipe::core::types::WildcardDdsFilterTopic participant_info_topic;
        participant_info_topic.topic_name.set_value(participants::PARTICIPANT_INFO_TOPIC_NAME);

        configuration.ddspipe_configuration.allowlist.insert(
            utils::Heritable<eprosima::ddspipe::core::types::WildcardDdsFilterTopic>::make_heritable(
                participant_info_topic));

        eprosima::ddspipe::core::types::WildcardDdsFilterTopic endpoint_info_topic;
        endpoint_info_topic.topic_name.set_value(participants::ENDPOINT_INFO_TOPIC_NAME);

        configuration.ddspipe_configuration.allowlist.insert(
            utils::Heritable<eprosima::ddspipe::core::types::WildcardDdsFilterTopic>::make_heritable(
                endpoint_info_topic));
    }
}

utils::ReturnCode Backend::reload_configuration(
        yaml::Configuration& new_configuration)
{
    load_internal_topics_(new_configuration);

    return pipe_->reload_configuration(new_configuration.ddspipe_configuration);
}

std::shared_ptr<eprosima::spy::participants::SpyModel> Backend::model() const noexcept
{
    return model_;
}

void Backend::update_readers_track_partitions(
        std::set<std::string> partitions_set)
{
    // Function used to update the SpyDdsXmlParticipant filter data structures
    // This structures are used when creating a Spy Reader.
    // ONLY DDS
    //std::dynamic_pointer_cast<participants::SpyDdsXmlParticipant>(dds_participant_)->update_filters(0, partitions_set);
    // BOTH, DDS and RTPS
    dds_participant_->update_filters(0, partitions_set, "", "");

    // Function used to update 'content_topicfilter' in the active topics
    pipe_->update_partitions(partitions_set);
}

void Backend::update_readers_track_content_filter(
        const std::string& topic_name,
        const std::string& expression)
{
    // Function used to update the SpyDdsXmlParticipant filter data structures
    // This structures are used when creating a Spy Reader.

    // ONLY DDS
    //std::dynamic_pointer_cast<participants::SpyDdsXmlParticipant>(dds_participant_)->update_filters(1, std::set<std::string>(), topic_name, expression);
    // BOTH, DDS and RTPS
    dds_participant_->update_filters(1, std::set<std::string>(), topic_name, expression);
    // Function used to update 'content_topicfilter' in the active topics
    pipe_->update_content_filter(topic_name, expression);
}

} /* namespace spy */
} /* namespace eprosima */
