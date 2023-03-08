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
            std::shared_ptr<ddspipe::participants::ParticipantConfiguration> participant_configuration,
            std::shared_ptr<ddspipe::core::PayloadPool> payload_pool,
            std::shared_ptr<ddspipe::core::DiscoveryDatabase> discovery_database,
            std::shared_ptr<DataVisualizer> data_visualizer);

    //! Override create_writer_() IParticipant method
    std::shared_ptr<core::IWriter> create_writer(
            const core::ITopic& topic) override;

protected:

    //! Participants Internal Reader
    std::shared_ptr<InternalWriter> participants_writer_;
    std::shared_ptr<InternalWriter> type_object_writer_;

    std::shared_ptr<DataVisualizer> data_visualizer_;
};

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
