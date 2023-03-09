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

#include <fastddsspy_participants/writer/InternalWriter.hpp>

namespace eprosima {
namespace spy {
namespace participants {

InternalWriter::InternalWriter(
        const ddspipe::core::types::ParticipantId& participant_id,
        const std::function<void(ddspipe::core::IRoutingData&)>& callback)
    : ddspipe::participants::BaseWriter(participant_id)
    , callback_(callback)
{
    // Do nothing
}

utils::ReturnCode InternalWriter::write_nts_(
        ddspipe::core::IRoutingData& data) noexcept
{
    callback_(data);
    return utils::ReturnCode::RETCODE_OK;
}

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
