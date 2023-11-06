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

#include <fastddsspy_participants/library/library_dll.h>

#include <fastddsspy_participants/model/DataStreamer.hpp>
#include <fastddsspy_participants/model/NetworkDatabase.hpp>

namespace eprosima {
namespace spy {
namespace participants {

/**
 * TODO comment
 */
class SpyModel : public NetworkDatabase, public DataStreamer
{
public:

    FASTDDSSPY_PARTICIPANTS_DllAPI
    SpyModel(
            bool ros2_types = false);

    FASTDDSSPY_PARTICIPANTS_DllAPI
    bool get_ros2_types() const noexcept;

private:

    bool ros2_types_;
};

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
