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

#include <ddspipe_core/types/topic/filter/IFilterTopic.hpp>
#include <ddspipe_core/types/topic/dds/DistributedTopic.hpp>
#include <ddspipe_core/configuration/IConfiguration.hpp>

#include <ddspipe_participants/configuration/SimpleParticipantConfiguration.hpp>
#include <ddspipe_participants/configuration/ParticipantConfiguration.hpp>

#include <ddspipe_yaml/Yaml.hpp>
#include <ddspipe_yaml/YamlReader.hpp>

#include <fastddsspy_participants/configuration/SpyParticipantConfiguration.hpp>

#include <fastddsspy_yaml/library/library_dll.h>

namespace eprosima {
namespace spy {
namespace yaml {

/**
 * @brief Class that encapsulates specific methods to get a full ddspipe Configuration from a yaml node.
 *
 * TODO: Add version configuration so it could load different versions
 */
class Configuration : ddspipe::core::IConfiguration
{
public:

    Configuration();

    Configuration(
            const Yaml& yml);

    Configuration(
            const std::string& file_path);

    virtual bool is_valid(
            utils::Formatter& error_msg) const noexcept override;

    // Participants configurations
    std::shared_ptr<ddspipe::participants::SimpleParticipantConfiguration> simple_configuration;
    std::shared_ptr<participants::SpyParticipantConfiguration> spy_configuration;

    // Topic filtering
    std::set<utils::Heritable<ddspipe::core::types::IFilterTopic>> allowlist {};
    std::set<utils::Heritable<ddspipe::core::types::IFilterTopic>> blocklist {};
    std::set<utils::Heritable<ddspipe::core::types::DistributedTopic>> builtin_topics {};

    // Specs
    unsigned int n_threads = 12;

protected:

    void load_configuration_(
            const Yaml& yml);

    void load_specs_configuration_(
            const Yaml& yml,
            const ddspipe::yaml::YamlReaderVersion& version);

    void load_dds_configuration_(
            const Yaml& yml,
            const ddspipe::yaml::YamlReaderVersion& version);
};

} /* namespace yaml */
} /* namespace spy */
} /* namespace eprosima */
