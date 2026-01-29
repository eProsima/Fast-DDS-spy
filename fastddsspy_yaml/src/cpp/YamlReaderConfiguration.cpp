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
#include <ddspipe_core/types/dds/TopicQoS.hpp>
#include <ddspipe_core/types/dynamic_types/types.hpp>
#include <ddspipe_core/types/topic/dds/DdsTopic.hpp>
#include <ddspipe_core/types/topic/filter/ManualTopic.hpp>
#include <ddspipe_core/types/topic/filter/WildcardDdsFilterTopic.hpp>

#include <ddspipe_participants/xml/XmlHandlerConfiguration.hpp>
#include <ddspipe_participants/types/address/Address.hpp>

#include <ddspipe_yaml/yaml_configuration_tags.hpp>
#include <ddspipe_yaml/Yaml.hpp>
#include <ddspipe_yaml/YamlManager.hpp>
#include <ddspipe_yaml/YamlReader.hpp>

#include <fastddsspy_yaml/yaml_configuration_tags.hpp>

#include <fastddsspy_yaml/YamlReaderConfiguration.hpp>

namespace eprosima {
namespace spy {
namespace yaml {

using namespace eprosima::ddspipe::core;
using namespace eprosima::ddspipe::core::types;
using namespace eprosima::ddspipe::participants;
using namespace eprosima::ddspipe::participants::types;
using namespace eprosima::ddspipe::yaml;

Configuration::Configuration()
    : dds_configuration(std::make_shared<ddspipe::participants::XmlParticipantConfiguration>())
    , spy_configuration(std::make_shared<participants::SpyParticipantConfiguration>())
{
    dds_configuration->id = "SimpleParticipant";
    dds_configuration->app_id = "FASTDDS_SPY";
    dds_configuration->app_metadata = "";
    dds_configuration->is_repeater = false;
    spy_configuration->id = "Fast-Spy-007";
    spy_configuration->app_id = "FASTDDS_SPY";
    spy_configuration->app_metadata = "";
    spy_configuration->is_repeater = false;
}

Configuration::Configuration(
        const Yaml& yml,
        const CommandlineArgsSpy* args /*= nullptr*/)
    : Configuration()
{
    load_configuration_(yml, args);
}

Configuration::Configuration(
        const std::string& file_path,
        const CommandlineArgsSpy* args /*= nullptr*/)
    : Configuration()
{
    load_configuration_from_file_(file_path, args);
}

void Configuration::load_configuration_(
        const Yaml& yml,
        const CommandlineArgsSpy* args)
{
    try
    {
        YamlReaderVersion version = LATEST;

        /////
        // Get optional specs configuration options
        if (YamlReader::is_tag_present(yml, SPECS_TAG))
        {
            auto specs_yml = YamlReader::get_value_in_tag(yml, SPECS_TAG);
            load_specs_configuration_(specs_yml, version);
        }

        /////
        // Get optional DDS configuration options
        if (YamlReader::is_tag_present(yml, DDS_TAG))
        {
            auto dds_yml = YamlReader::get_value_in_tag(yml, DDS_TAG);
            load_dds_configuration_(dds_yml, version);
        }

        // Block ROS 2 services (RPC) topics
        // RATIONALE:
        // At the time of this writting, services in ROS 2 behave in the following manner: a ROS 2 service
        // client awaits to discover a server, and it is then when a request is sent to this (and only this) server,
        // from which a response is expected.
        // Hence, if these topics are not blocked, the client would wrongly believe Fast-DDS-spy is a server, thus
        // sending a request for which a response will not be received.
        WildcardDdsFilterTopic rpc_request_topic, rpc_response_topic;
        rpc_request_topic.topic_name.set_value("rq/*");
        rpc_response_topic.topic_name.set_value("rr/*");

        ddspipe_configuration.blocklist.insert(
            utils::Heritable<WildcardDdsFilterTopic>::make_heritable(rpc_request_topic));
        ddspipe_configuration.blocklist.insert(
            utils::Heritable<WildcardDdsFilterTopic>::make_heritable(rpc_response_topic));

        // Only trigger the DdsPipe's callbacks when discovering or removing writers
        ddspipe_configuration.discovery_trigger = DiscoveryTrigger::WRITER;

        /////
        // Log Configuration's set methods: Depending on where Log Configuration has been configured
        // (Yaml, Command-Line and/or by default) these methods will set DdsPipeConfiguration's log_configuration
        // taking into account this precedence:
        //  1. Log Configuration set on Command-line.
        //  2. Log Configuration set by YAML.
        //  3. Log Configuration set by default.
        if (args != nullptr)
        {
            ddspipe_configuration.log_configuration.set(args->log_verbosity);
            ddspipe_configuration.log_configuration.set(args->log_filter);

            if (args->domain.is_set())
            {
                // Set domain from command-line
                dds_configuration->domain = args->domain.get_value();
            }
        }
    }
    catch (const std::exception& e)
    {
        throw eprosima::utils::ConfigurationException(
                  utils::Formatter() << "Error loading DDS Router configuration from yaml:\n " << e.what());
    }
}

void Configuration::load_dds_configuration_(
        const Yaml& yml,
        const ddspipe::yaml::YamlReaderVersion& version)
{
    /////
    // Get optional xml configuration
    if (YamlReader::is_tag_present(yml, XML_TAG))
    {
        YamlReader::fill<XmlHandlerConfiguration>(
            xml_configuration,
            YamlReader::get_value_in_tag(yml, XML_TAG),
            version);
    }

    // Check if FASTDDSSPY_PROFILE_TAG exists
    if (YamlReader::is_tag_present(yml, FASTDDSSPY_PROFILE_TAG))
    {
        dds_configuration->participant_profile = YamlReader::get<std::string>(yml, FASTDDSSPY_PROFILE_TAG, version);
    }

    /////
    // Get optional allowlist
    if (YamlReader::is_tag_present(yml, ALLOWLIST_TAG))
    {
        ddspipe_configuration.allowlist = YamlReader::get_set<utils::Heritable<IFilterTopic>>(yml, ALLOWLIST_TAG,
                        version);
    }

    /////
    // Get optional blocklist
    if (YamlReader::is_tag_present(yml, BLOCKLIST_TAG))
    {
        ddspipe_configuration.blocklist = YamlReader::get_set<utils::Heritable<IFilterTopic>>(yml, BLOCKLIST_TAG,
                        version);
    }

    /////
    // Get optional topics
    if (YamlReader::is_tag_present(yml, TOPICS_TAG))
    {
        const auto& manual_topics = YamlReader::get_list<ManualTopic>(yml, TOPICS_TAG, version);
        ddspipe_configuration.manual_topics =
                std::vector<ManualTopic>(manual_topics.begin(), manual_topics.end());

        for (const auto topic: manual_topics)
        {
            dds_configuration->content_topic_filter_dict[topic.first->topic_name] =
                    topic.first->content_topic_filter;
        }
    }

    // Set the domain in Simple Participant Configuration
    if (YamlReader::is_tag_present(yml, DOMAIN_ID_TAG))
    {
        dds_configuration->domain = YamlReader::get<DomainId>(yml, DOMAIN_ID_TAG, version);
    }

    /////
    // Get optional whitelist interfaces
    if (YamlReader::is_tag_present(yml, WHITELIST_INTERFACES_TAG))
    {
        dds_configuration->whitelist = YamlReader::get_set<WhitelistType>(yml, WHITELIST_INTERFACES_TAG,
                        version);
    }

    /////
    // Get optional partitions
    if (YamlReader::is_tag_present(yml, PARTITIONLIST_TAG))
    {
        dds_configuration->allowed_partition_list = YamlReader::get_set<std::string>(yml, PARTITIONLIST_TAG,
                        version);
    }

    // Optional get Transport protocol
    if (YamlReader::is_tag_present(yml, TRANSPORT_DESCRIPTORS_TRANSPORT_TAG))
    {
        dds_configuration->transport = YamlReader::get<TransportDescriptors>(yml,
                        TRANSPORT_DESCRIPTORS_TRANSPORT_TAG,
                        version);
    }
    else
    {
        dds_configuration->transport = TransportDescriptors::builtin;
    }

    if (YamlReader::is_tag_present(yml, EASY_MODE_TAG))
    {
        dds_configuration->easy_mode_ip = YamlReader::get<IpType>(yml, EASY_MODE_TAG, version);
    }

    // Optional get ignore participant flags
    if (YamlReader::is_tag_present(yml, IGNORE_PARTICIPANT_FLAGS_TAG))
    {
        dds_configuration->ignore_participant_flags = YamlReader::get<IgnoreParticipantFlags>(yml,
                        IGNORE_PARTICIPANT_FLAGS_TAG,
                        version);
    }
    else
    {
        dds_configuration->ignore_participant_flags = IgnoreParticipantFlags::no_filter;
    }

    /////
    // Get optional ros2_types
    if (YamlReader::is_tag_present(yml, ROS2_TYPES_TAG))
    {
        ros2_types = YamlReader::get<bool>(yml, ROS2_TYPES_TAG, version);
    }
}

void Configuration::load_specs_configuration_(
        const Yaml& yml,
        const ddspipe::yaml::YamlReaderVersion& version)
{
    // Get optional number of threads
    if (YamlReader::is_tag_present(yml, NUMBER_THREADS_TAG))
    {
        n_threads = YamlReader::get<unsigned int>(yml, NUMBER_THREADS_TAG, version);
    }

    /////
    // Get optional Topic QoS
    if (YamlReader::is_tag_present(yml, SPECS_QOS_TAG))
    {
        YamlReader::fill<TopicQoS>(topic_qos, YamlReader::get_value_in_tag(yml, SPECS_QOS_TAG), version);
        TopicQoS::default_topic_qos.set_value(topic_qos);
    }

    // Get optional gathering time
    if (YamlReader::is_tag_present(yml, GATHERING_TIME_TAG))
    {
        one_shot_wait_time_ms = YamlReader::get<utils::Duration_ms>(yml, GATHERING_TIME_TAG, version);
    }

    // Get optional rtps enabled
    if (YamlReader::is_tag_present(yml, RTPS_ENABLED_TAG))
    {
        dds_enabled = !YamlReader::get<bool>(yml, RTPS_ENABLED_TAG, version);
    }

    /////
    // Get optional Log Configuration
    if (YamlReader::is_tag_present(yml, LOG_CONFIGURATION_TAG))
    {
        ddspipe_configuration.log_configuration = YamlReader::get<DdsPipeLogConfiguration>(yml, LOG_CONFIGURATION_TAG,
                        version);
    }
}

void Configuration::load_configuration_from_file_(
        const std::string& file_path,
        const CommandlineArgsSpy* args)
{
    Yaml yml;

    // Load file
    try
    {
        if (!file_path.empty())
        {
            yml = YamlManager::load_file(file_path);
        }
    }
    catch (const std::exception& e)
    {
        throw eprosima::utils::ConfigurationException(
                  utils::Formatter() << "Error loading Fast-DDS Spy configuration from file: <" << file_path <<
                      "> :\n " << e.what());
    }

    load_configuration_(yml, args);
}

bool Configuration::is_valid(
        utils::Formatter& error_msg) const noexcept
{
    if (n_threads < 1)
    {
        error_msg << "Must be at least 1 thread. ";
        return false;
    }
    return true;
}

} /* namespace yaml */
} /* namespace spy */
} /* namespace eprosima */
