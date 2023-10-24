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
    , dds_participant_(
        std::make_shared<participants::SpyDdsParticipant>(
            configuration.simple_configuration,
            payload_pool_,
            discovery_database_))
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

    // Init Dds Participant
    dds_participant_->init();

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
        configuration.ddspipe_configuration,
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

utils::ReturnCode Backend::reload_configuration(
        const yaml::Configuration& new_configuration)
{
    return pipe_->reload_configuration(new_configuration.ddspipe_configuration);
}

std::shared_ptr<eprosima::spy::participants::SpyModel> Backend::model() const noexcept
{
    return model_;
}

} /* namespace spy */
} /* namespace eprosima */
