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

#include <functional>
#include <iostream>
#include <shared_mutex>
#include <tuple>
#include <unordered_map>

#include <fastcdr/cdr/fixed_size_string.hpp>

#include <fastdds/dds/xtypes/dynamic_types/DynamicType.hpp>

#include <ddspipe_core/types/dds/Payload.hpp>
#include <ddspipe_core/types/topic/dds/DdsTopic.hpp>
#include <ddspipe_core/types/topic/filter/WildcardDdsFilterTopic.hpp>

#include <fastddsspy_participants/library/library_dll.h>
#include <fastddsspy_participants/model/TopicRateCalculator.hpp>

namespace eprosima {
namespace spy {
namespace participants {

/**
 * TODO comment
 */
class DataStreamer : public TopicRateCalculator
{
public:

    using CallbackType = std::function<void (
                        const ddspipe::core::types::DdsTopic&,
                        const fastdds::dds::DynamicType::_ref_type&,
                        const ddspipe::core::types::RtpsPayloadData&)>;

    FASTDDSSPY_PARTICIPANTS_DllAPI
    bool activate_all(
            const std::shared_ptr<CallbackType>& callback);

    FASTDDSSPY_PARTICIPANTS_DllAPI
    bool activate(
            const ddspipe::core::types::WildcardDdsFilterTopic& topic_to_activate,
            const std::shared_ptr<CallbackType>& callback);

    FASTDDSSPY_PARTICIPANTS_DllAPI
    void deactivate();

    FASTDDSSPY_PARTICIPANTS_DllAPI
    void add_schema(
            const fastdds::dds::DynamicType::_ref_type& dynamic_type,
            const fastdds::dds::xtypes::TypeIdentifier& type_identifier) override;

    FASTDDSSPY_PARTICIPANTS_DllAPI
    void add_data(
            const ddspipe::core::types::DdsTopic& topic,
            ddspipe::core::types::RtpsPayloadData& data) override;

    FASTDDSSPY_PARTICIPANTS_DllAPI
    bool is_topic_type_discovered(
            const ddspipe::core::types::WildcardDdsFilterTopic& topic_to_activate) const noexcept;

    FASTDDSSPY_PARTICIPANTS_DllAPI
    bool is_topic_type_discovered(
            const ddspipe::core::types::DdsTopic& topic_to_activate) const noexcept;

protected:

    bool is_topic_type_discovered_nts_(
            const ddspipe::core::types::WildcardDdsFilterTopic& topic_to_activate) const noexcept;

    bool is_topic_type_discovered_nts_(
            const ddspipe::core::types::DdsTopic& topic_to_activate) const noexcept;

    bool activated_ {false};

    std::shared_ptr<CallbackType> callback_;

    bool activated_all_ {false};

    ddspipe::core::types::WildcardDdsFilterTopic activated_topic_;

    std::map<std::string, fastdds::dds::DynamicType::_ref_type> types_discovered_;

    std::unordered_map<std::string, std::string> topic_type_discovered_;

    mutable std::shared_timed_mutex mutex_;
};

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
