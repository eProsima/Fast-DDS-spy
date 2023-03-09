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

#include <functional>

#include <ddspipe_participants/writer/auxiliar/BaseWriter.hpp>

namespace eprosima {
namespace spy {
namespace participants {

/**
 * Reader implementation that allows to introduce custom data to DDS Pipe.
 */
class InternalWriter : public ddspipe::participants::BaseWriter
{
public:

    InternalWriter(
            const ddspipe::core::types::ParticipantId& participant_id,
            const std::function<void(ddspipe::core::IRoutingData&)>& callback);

protected:

    virtual utils::ReturnCode write_nts_(
            ddspipe::core::IRoutingData& data) noexcept override;

    const std::function<void(ddspipe::core::IRoutingData&)> callback_;
};

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
