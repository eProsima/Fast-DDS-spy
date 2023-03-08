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

#include <ddspipe_core/library/library_dll.h>
#include <ddspipe_core/types/dds/Guid.hpp>

namespace eprosima {
namespace spy {
namespace participants {

/**
 * TODO
 */
struct ParticipantInfo
{
    //! Name of the participant
    std::string name{};

    //! Unique id of the participant
    ddspipe::core::types::Guid guid {};

    //! Whether the participant is active or has left
    bool active;
};

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
