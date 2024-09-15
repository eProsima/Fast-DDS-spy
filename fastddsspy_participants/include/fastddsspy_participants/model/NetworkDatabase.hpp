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
// limitations under the License\.

#pragma once

#include <cpp_utils/collection/database/SafeDatabase.hpp>
#include <cpp_utils/types/Atomicable.hpp>

#include <fastddsspy_participants/library/library_dll.h>
#include <fastddsspy_participants/types/ParticipantInfo.hpp>
#include <fastddsspy_participants/types/EndpointInfo.hpp>

namespace eprosima {
namespace spy {
namespace participants {

/**
 * TODO comment
 */
struct NetworkDatabase
{
public:

    utils::SafeDatabase<ddspipe::core::types::Guid, ParticipantInfo> participant_database_;

    utils::SafeDatabase<ddspipe::core::types::Guid, EndpointInfo> endpoint_database_;

    FASTDDSSPY_PARTICIPANTS_DllAPI
    bool get_topic(
            const std::string& topic_name,
            ddspipe::core::types::DdsTopic& topic) const noexcept;
};

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
