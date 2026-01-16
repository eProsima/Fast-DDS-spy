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
 * @file Backend.hpp
 * @brief This file contains the definition of the Backend class, which manages the Fast DDS Spy backend logic.
 */

#pragma once

#include <memory>

#include <cpp_utils/user_interface/CommandReader.hpp>
#include <cpp_utils/ReturnCode.hpp>
#include <cpp_utils/time/time_utils.hpp>
#include <cpp_utils/utils.hpp>
#include <cpp_utils/thread_pool/pool/SlotThreadPool.hpp>

#include <ddspipe_core/core/DdsPipe.hpp>
#include <ddspipe_core/dynamic/ParticipantsDatabase.hpp>
#include <ddspipe_core/dynamic/DiscoveryDatabase.hpp>
#include <ddspipe_core/efficiency/payload/FastPayloadPool.hpp>
#include <ddspipe_core/interface/IParticipant.hpp>

#include <fastddsspy_participants/participant/SpyDdsParticipant.hpp>
#include <fastddsspy_participants/participant/SpyDdsXmlParticipant.hpp>
#include <fastddsspy_participants/participant/SpyParticipant.hpp>

#include <fastddsspy_yaml/YamlReaderConfiguration.hpp>

#include "Command.hpp"

namespace eprosima {
namespace spy {

/**
 * @class Backend
 * @brief Manages the backend logic for the Fast DDS Spy tool.
 */
class Backend
{
public:

    /**
     * @brief Constructs the Backend object with the given configuration.
     *
     * @param configuration The YAML configuration for the backend.
     */
    Backend(
            const yaml::Configuration& configuration);

    /**
     * @brief Destroys the Backend object and releases resources.
     */
    ~Backend();

    /**
     * @brief Reloads the backend configuration.
     *
     * @param new_configuration The new YAML configuration to load.
     * @return A ReturnCode indicating the success or failure of the operation.
     */
    utils::ReturnCode reload_configuration(
            yaml::Configuration& new_configuration);

    /**
     * @brief Retrieves the SpyModel associated with the backend.
     *
     * @return A shared pointer to the SpyModel.
     */
    std::shared_ptr<eprosima::spy::participants::SpyModel> model() const noexcept;

void update_readers_track_partitions(
            std::set<std::string> partitions_set);

    void update_readers_track_content_filter(
            const std::string& topic_name,
            const std::string& expression);

    void update_readers_track(
            const std::string topic_name,
            const std::set<std::string> filter_partition_set);

    void update_pipeline_filter(
            const std::set<std::string> filter_partition_set);

protected:

    /**
     * @brief Loads internal topics based on the given configuration.
     *
     * @param configuration The YAML configuration to use for loading topics.
     */
    void load_internal_topics_(
            yaml::Configuration& configuration);

    /// The YAML configuration for the backend.
    yaml::Configuration configuration_;

    /// The payload pool used for managing DDS payloads.
    std::shared_ptr<ddspipe::core::PayloadPool> payload_pool_;

    /// The discovery database for managing discovered entities.
    std::shared_ptr<ddspipe::core::DiscoveryDatabase> discovery_database_;

    /// The thread pool for managing asynchronous tasks.
    std::shared_ptr<eprosima::utils::SlotThreadPool> thread_pool_;

    /// The database of participants in the DDS system.
    std::shared_ptr<ddspipe::core::ParticipantsDatabase> participant_database_;

    /// The SpyModel used for interacting with DDS entities.
    std::shared_ptr<eprosima::spy::participants::SpyModel> model_;

    /// The DDS participant used for communication.
    std::shared_ptr<ddspipe::core::IParticipant> dds_participant_;

    /// The SpyParticipant used for spying on DDS entities.
    std::shared_ptr<eprosima::spy::participants::SpyParticipant> spy_participant_;

    /// The DDS pipe used for data routing.
    std::unique_ptr<ddspipe::core::DdsPipe> pipe_;
};

} /* namespace spy */
} /* namespace eprosima */
