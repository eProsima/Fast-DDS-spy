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

#include <iostream>
#include <functional>
#include <shared_mutex>

#include <fastrtps/types/DynamicTypePtr.h>

#include <ddspipe_core/types/topic/dds/DdsTopic.hpp>
#include <ddspipe_core/types/dds/Payload.hpp>

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
                        const fastrtps::types::DynamicType_ptr&,
                        const ddspipe::core::types::RtpsPayloadData&)>;

    bool activate_all(
            const std::shared_ptr<CallbackType>& callback);

    bool activate(
            const ddspipe::core::types::DdsTopic& topic_to_activate,
            const std::shared_ptr<CallbackType>& callback);

    void deactivate();

    void add_schema(
            const fastrtps::types::DynamicType_ptr& dynamic_type) override;

    void add_data(
            const ddspipe::core::types::DdsTopic& topic,
            ddspipe::core::types::RtpsPayloadData& data) override;

    bool is_topic_type_discovered(
            const ddspipe::core::types::DdsTopic& topic_to_activate) const noexcept;

protected:

    bool is_topic_type_discovered_nts_(
            const ddspipe::core::types::DdsTopic& topic_to_activate) const noexcept;

    bool activated_ {false};

    std::shared_ptr<CallbackType> callback_;

    bool activated_all_ {false};

    ddspipe::core::types::DdsTopic activated_topic_;

    std::map<std::string, fastrtps::types::DynamicType_ptr> types_discovered_;

    mutable std::shared_timed_mutex mutex_;
};

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
