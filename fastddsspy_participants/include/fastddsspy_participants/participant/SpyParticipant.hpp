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

#include <cpp_utils/types/Atomicable.hpp>

#include <ddspipe_participants/participant/dynamic_types/SchemaParticipant.hpp>

#include <fastddsspy_participants/types/ParticipantInfo.hpp>
#include <fastddsspy_participants/writer/InternalWriter.hpp>
#include <fastddsspy_participants/model/SpyModel.hpp>

namespace eprosima {
namespace spy {
namespace participants {

/**
 * TODO
 */
class SpyParticipant : public ddspipe::participants::SchemaParticipant
{
public:

    // TODO
    SpyParticipant(
            const std::shared_ptr<ddspipe::participants::ParticipantConfiguration>& participant_configuration,
            const std::shared_ptr<ddspipe::core::PayloadPool>& payload_pool,
            const std::shared_ptr<ddspipe::core::DiscoveryDatabase>& discovery_database,
            const std::shared_ptr<SpyModel>& model_);

    //! Override create_writer_() IParticipant method
    std::shared_ptr<ddspipe::core::IWriter> create_writer(
            const ddspipe::core::ITopic& topic) override;

protected:

    utils::ReturnCode new_participant_info_(
            const ddspipe::core::IRoutingData& data);

    utils::ReturnCode new_endpoint_info_(
            const ddspipe::core::IRoutingData& data);

    //! Participants Internal Reader
    std::shared_ptr<InternalWriter> participants_writer_;

    std::shared_ptr<InternalWriter> endpoints_writer_;

    std::shared_ptr<SpyModel> model_;
};

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
