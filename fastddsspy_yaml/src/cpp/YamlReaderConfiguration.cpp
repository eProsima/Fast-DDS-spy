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

#include <ddspipe_core/types/dds/TopicQoS.hpp>
#include <ddspipe_core/types/dynamic_types/types.hpp>
#include <ddspipe_core/types/topic/dds/DdsTopic.hpp>
#include <ddspipe_core/types/topic/filter/IFilterTopic.hpp>
#include <ddspipe_core/types/topic/filter/WildcardDdsFilterTopic.hpp>

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
using namespace eprosima::ddspipe::participants;
using namespace eprosima::ddspipe::yaml;

Configuration::Configuration()
    : simple_configuration(std::make_shared<ddspipe::participants::SimpleParticipantConfiguration>())
    , spy_configuration(std::make_shared<participants::SpyParticipantConfiguration>())
{
    simple_configuration->id = "SimpleParticipant";
    simple_configuration->is_repeater = false;
    spy_configuration->id = "Fast-Spy-007";
    spy_configuration->is_repeater = false;
}

Configuration::Configuration(
        const Yaml& yml)
    : Configuration()
{
    load_configuration_(yml);
}

Configuration::Configuration(
        const std::string& file_path)
    : Configuration(YamlManager::load_file(file_path))
{
    // Do nothing
}

void Configuration::load_configuration_(
        const Yaml& yml)
{
    try
    {
        YamlReaderVersion version = LATEST;

        /////
        // Get optional DDS configuration options
        if (YamlReader::is_tag_present(yml, DDS_TAG))
        {
            auto dds_yml = YamlReader::get_value_in_tag(yml, DDS_TAG);
            load_dds_configuration_(dds_yml, version);
        }

        /////
        // Get optional specs configuration options
        if (YamlReader::is_tag_present(yml, SPECS_TAG))
        {
            auto specs_yml = YamlReader::get_value_in_tag(yml, SPECS_TAG);
            load_specs_configuration_(specs_yml, version);
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
    // Get optional allowlist
    if (YamlReader::is_tag_present(yml, ALLOWLIST_TAG))
    {
        allowlist = YamlReader::get_set<utils::Heritable<ddspipe::core::types::IFilterTopic>>(yml, ALLOWLIST_TAG,
                        version);

        // Add to allowlist always the type object topic
        ddspipe::core::types::WildcardDdsFilterTopic internal_topic;
        internal_topic.topic_name.set_value(ddspipe::core::types::TYPE_OBJECT_TOPIC_NAME);
        allowlist.insert(
            utils::Heritable<ddspipe::core::types::WildcardDdsFilterTopic>::make_heritable(internal_topic));
    }

    /////
    // Get optional blocklist
    if (YamlReader::is_tag_present(yml, BLOCKLIST_TAG))
    {
        blocklist = YamlReader::get_set<utils::Heritable<ddspipe::core::types::IFilterTopic>>(yml, BLOCKLIST_TAG,
                        version);
    }

    /////
    // Get optional builtin topics
    if (YamlReader::is_tag_present(yml, BUILTIN_TAG))
    {
        builtin_topics = YamlReader::get_set<utils::Heritable<ddspipe::core::types::DistributedTopic>>(yml, BUILTIN_TAG,
                        version);
    }

    // Set the domain in Simple Participant Configuration
    if (YamlReader::is_tag_present(yml, DOMAIN_ID_TAG))
    {
        simple_configuration->domain = YamlReader::get<ddspipe::core::types::DomainId>(yml, DOMAIN_ID_TAG, version);
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

    // Get optional maximum history depth
    if (YamlReader::is_tag_present(yml, MAX_HISTORY_DEPTH_TAG))
    {
        ddspipe::core::types::TopicQoS::default_history_depth = YamlReader::get<unsigned int>(
            yml,
            MAX_HISTORY_DEPTH_TAG,
            version);
    }
}

} /* namespace yaml */
} /* namespace spy */
} /* namespace eprosima */
