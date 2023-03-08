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

#include <ddspipe_participants/participant/dynamic_types/ISchemaHandler.hpp>

#include <fastddsspy_participants/types/ParticipantInfo.hpp>

namespace eprosima {
namespace spy {
namespace core {

/**
 * TODO
 */
class DataVisualizer : public ddspipe::participants::ISchemaHandler
{
public:

    bool activate(
            const std::string& activate_topic,
            std::ifstream& target = std::cin);

    void deactivate();

    virtual void add_schema(
            const fastrtps::types::DynamicType_ptr& dynamic_type) override;

    virtual void add_data(
            const core::types::DdsTopic& topic,
            core::types::RtpsPayloadData& data) override;

protected:

    void print_data_(
        fastrtps::types::DynamicType_ptr& type,
        core::types::RtpsPayloadData& data) const noexcept;

    fastrtps::types::DynamicType_ptr get_dynamic_type_(const std::string& type_name);

    bool activated_;

    std::string activated_topic_;

    std::set<std::string> types_discovered_;

    std::ifstream& target_;

    mutable std::shared_mutex mutex_;
};

} /* namespace core */
} /* namespace spy */
} /* namespace eprosima */
