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

#include <cpp_utils/memory/Heritable.hpp>
#include <cpp_utils/time/time_utils.hpp>

#include <ddspipe_core/configuration/DdsPipeConfiguration.hpp>
#include <ddspipe_core/configuration/IConfiguration.hpp>
#include <ddspipe_core/types/topic/filter/IFilterTopic.hpp>
#include <ddspipe_core/types/topic/dds/DistributedTopic.hpp>

#include <ddspipe_participants/configuration/ParticipantConfiguration.hpp>
#include <ddspipe_participants/configuration/XmlParticipantConfiguration.hpp>
#include <ddspipe_participants/xml/XmlHandlerConfiguration.hpp>

#include <ddspipe_yaml/Yaml.hpp>
#include <ddspipe_yaml/YamlReader.hpp>

#include <fastddsspy_participants/configuration/SpyParticipantConfiguration.hpp>
#include <fastddsspy_participants/types/EndpointInfo.hpp>
#include <fastddsspy_participants/types/ParticipantInfo.hpp>

#include <fastddsspy_yaml/CommandlineArgsSpy.hpp>
#include <fastddsspy_yaml/library/library_dll.h>

namespace eprosima {
namespace spy {
namespace yaml {

/**
 * @brief Class that encapsulates specific methods to get a full FastDdsSpy Configuration from a yaml node.
 */
class Configuration : ddspipe::core::IConfiguration
{
public:

    FASTDDSSPY_YAML_DllAPI
    Configuration();

    FASTDDSSPY_YAML_DllAPI
    Configuration(
            const Yaml& yml,
            const CommandlineArgsSpy* args = nullptr);

    FASTDDSSPY_YAML_DllAPI
    Configuration(
            const std::string& file_path,
            const CommandlineArgsSpy* args = nullptr);

    FASTDDSSPY_YAML_DllAPI
    virtual bool is_valid(
            utils::Formatter& error_msg) const noexcept override;

    //! DdsPipe configuration
    ddspipe::core::DdsPipeConfiguration ddspipe_configuration {};

    // XML configuration
    ddspipe::participants::XmlHandlerConfiguration xml_configuration;
    bool xml_enabled = false;

    // Participants configurations
    std::shared_ptr<ddspipe::participants::XmlParticipantConfiguration> simple_configuration;
    std::shared_ptr<participants::SpyParticipantConfiguration> spy_configuration;

    //! Whether to generate schemas as OMG IDL or ROS2 msg
    bool ros2_types = false;

    // Specs
    unsigned int n_threads = 12;
    utils::Duration_ms one_shot_wait_time_ms = 1000;
    ddspipe::core::types::TopicQoS topic_qos{};

protected:

    void load_configuration_(
            const Yaml& yml,
            const CommandlineArgsSpy* args = nullptr);

    void load_specs_configuration_(
            const Yaml& yml,
            const ddspipe::yaml::YamlReaderVersion& version);

    void load_dds_configuration_(
            const Yaml& yml,
            const ddspipe::yaml::YamlReaderVersion& version);

    void load_configuration_from_file_(
            const std::string& file_path,
            const CommandlineArgsSpy* args = nullptr);
};

} /* namespace yaml */
} /* namespace spy */
} /* namespace eprosima */
