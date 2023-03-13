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
#include <shared_mutex>

#include <fastrtps/types/DynamicTypePtr.h>

#include <ddspipe_core/types/topic/dds/DdsTopic.hpp>
#include <ddspipe_core/types/dds/Payload.hpp>

#include <ddspipe_participants/participant/dynamic_types/ISchemaHandler.hpp>

#include <fastddsspy_participants/types/ParticipantInfo.hpp>

namespace eprosima {
namespace spy {
namespace participants {

/**
 * TODO
 */
class DataVisualizer : public ddspipe::participants::ISchemaHandler
{
public:

    bool activate(
            const ddspipe::core::types::DdsTopic& topic_to_activate,
            std::ostream* target = &std::cout);

    void deactivate();

    virtual void add_schema(
            const fastrtps::types::DynamicType_ptr& dynamic_type) override;

    virtual void add_data(
            const ddspipe::core::types::DdsTopic& topic,
            ddspipe::core::types::RtpsPayloadData& data) override;

protected:

    void print_data_nts_(
        fastrtps::types::DynamicType_ptr& type,
        ddspipe::core::types::RtpsPayloadData& data) const noexcept;

    bool activated_ {false};

    ddspipe::core::types::DdsTopic activated_topic_;

    std::map<std::string, fastrtps::types::DynamicType_ptr> types_discovered_;

    std::ostream* target_;

    mutable std::shared_mutex mutex_;
};

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
