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

#include <fastddsspy_participants/participant/SpyDdsParticipant.hpp>
#include <fastddsspy_participants/participant/SpyParticipant.hpp>

#include <fastddsspy_yaml/YamlReaderConfiguration.hpp>

#include "Command.hpp"

namespace eprosima {
namespace spy {

class Backend
{
public:

    Backend(
            const yaml::Configuration& configuration);

    ~Backend();

    utils::ReturnCode reload_allowed_topics(
            const std::shared_ptr<ddspipe::core::AllowedTopicList>& allowed_topics);

    std::shared_ptr<eprosima::spy::participants::SpyModel> model() const noexcept;

protected:

    // TODO comment
    yaml::Configuration configuration_;

    //! TODO comment
    std::shared_ptr<ddspipe::core::PayloadPool> payload_pool_;

    //! TODO comment
    std::shared_ptr<ddspipe::core::DiscoveryDatabase> discovery_database_;

    //! TODO comment
    std::shared_ptr<ddspipe::core::AllowedTopicList> allowed_topics_;

    //! TODO comment
    std::shared_ptr<eprosima::utils::SlotThreadPool> thread_pool_;

    //! TODO comment
    std::shared_ptr<ddspipe::core::ParticipantsDatabase> participant_database_;

    //! TODO comment
    std::shared_ptr<eprosima::spy::participants::SpyModel> model_;

    //! TODO comment
    std::shared_ptr<eprosima::spy::participants::SpyDdsParticipant> dds_participant_;

    //! TODO comment
    std::shared_ptr<eprosima::spy::participants::SpyParticipant> spy_participant_;

    //! TODO comment
    std::unique_ptr<ddspipe::core::DdsPipe> pipe_;

};

} /* namespace spy */
} /* namespace eprosima */
