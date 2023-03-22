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

#include <fastddsspy_participants/model/SpyModel.hpp>
#include <fastddsspy_participants/visualization/parser_data.hpp>

namespace eprosima {
namespace spy {
namespace participants {

/**
 * Helper methods to get structured information from model.
 */
struct ModelParser
{
    static std::vector<SimpleParticipantData> participants(
            const SpyModel& model);
    static std::vector<ComplexParticipantData> participants_verbose(
            const SpyModel& model);
    static ComplexParticipantData participants(
            const SpyModel& model,
            const ddspipe::core::types::Guid& guid);

    static std::vector<SimpleEndpointData> writers(
            const SpyModel& model);
    static std::vector<ComplexEndpointData> writers_verbose(
            const SpyModel& model);
    static ComplexEndpointData writers(
            const SpyModel& model,
            const ddspipe::core::types::Guid& guid);

    static std::vector<SimpleEndpointData> readers(
            const SpyModel& model);
    static std::vector<ComplexEndpointData> readers_verbose(
            const SpyModel& model);
    static ComplexEndpointData readers(
            const SpyModel& model,
            const ddspipe::core::types::Guid& guid);

    static ddspipe::core::types::DdsTopic get_topic(
            const SpyModel& model,
            std::string topic_name);

    static std::vector<SimpleTopicData> topics(
            const SpyModel& model);
    static std::vector<ComplexTopicData> topics_verbose(
            const SpyModel& model);
    static ComplexTopicData topics(
            const SpyModel& model,
            const std::string& topic_name);
};

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
