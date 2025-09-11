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

#include <ddspipe_core/types/topic/filter/WildcardDdsFilterTopic.hpp>

#include <fastddsspy_participants/library/library_dll.h>
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
    FASTDDSSPY_PARTICIPANTS_DllAPI
    static std::vector<SimpleParticipantData> participants(
            const SpyModel& model) noexcept;
    FASTDDSSPY_PARTICIPANTS_DllAPI
    static std::vector<ComplexParticipantData> participants_verbose(
            const SpyModel& model) noexcept;
    FASTDDSSPY_PARTICIPANTS_DllAPI
    static ComplexParticipantData participants(
            const SpyModel& model,
            const ddspipe::core::types::Guid& guid) noexcept;

    FASTDDSSPY_PARTICIPANTS_DllAPI
    static std::vector<SimpleEndpointData> writers(
            const SpyModel& model) noexcept;
    FASTDDSSPY_PARTICIPANTS_DllAPI
    static std::vector<ComplexEndpointData> writers_verbose(
            const SpyModel& model) noexcept;
    FASTDDSSPY_PARTICIPANTS_DllAPI
    static ComplexEndpointData writers(
            const SpyModel& model,
            const ddspipe::core::types::Guid& guid) noexcept;

    FASTDDSSPY_PARTICIPANTS_DllAPI
    static std::vector<SimpleEndpointData> readers(
            const SpyModel& model) noexcept;
    FASTDDSSPY_PARTICIPANTS_DllAPI
    static std::vector<ComplexEndpointData> readers_verbose(
            const SpyModel& model) noexcept;
    FASTDDSSPY_PARTICIPANTS_DllAPI
    static ComplexEndpointData readers(
            const SpyModel& model,
            const ddspipe::core::types::Guid& guid) noexcept;

    FASTDDSSPY_PARTICIPANTS_DllAPI
    static std::set<eprosima::ddspipe::core::types::DdsTopic> get_topics(
            const SpyModel& model,
            const ddspipe::core::types::WildcardDdsFilterTopic& filter_topic,
            const std::map<std::string, std::set<std::string>> filter_dict) noexcept;

    FASTDDSSPY_PARTICIPANTS_DllAPI
    static SimpleTopicData simple_topic_data(
            const SpyModel& model,
            const ddspipe::core::types::DdsTopic& topic) noexcept;
    FASTDDSSPY_PARTICIPANTS_DllAPI
    static ComplexTopicData complex_topic_data(
            const SpyModel& model,
            const ddspipe::core::types::DdsTopic& topic) noexcept;

    FASTDDSSPY_PARTICIPANTS_DllAPI
    static std::vector<SimpleTopicData> topics(
            const SpyModel& model,
            const ddspipe::core::types::WildcardDdsFilterTopic& filter_topic,
            const std::map<std::string, std::set<std::string>> filter_dict) noexcept;
    FASTDDSSPY_PARTICIPANTS_DllAPI
    static std::vector<ComplexTopicData> topics_verbose(
            const SpyModel& model,
            const ddspipe::core::types::WildcardDdsFilterTopic& filter_topic,
            const std::map<std::string, std::set<std::string>> filter_dict) noexcept;
    FASTDDSSPY_PARTICIPANTS_DllAPI
    static std::string topics_type_idl(
            const SpyModel& model,
            const ddspipe::core::types::WildcardDdsFilterTopic& filter_topic,
            const std::map<std::string, std::set<std::string>> filter_dict) noexcept;
};

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
