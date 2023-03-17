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

#include <memory>

#include <fastrtps/types/DynamicDataPtr.h>

#include <fastddsspy_participants/model/DataStreamer.hpp>

#include <fastddsspy_yaml/YamlReaderConfiguration.hpp>

#include "Backend.hpp"
#include "Input.hpp"
#include "View.hpp"

namespace eprosima {
namespace spy {

class Controller
{
public:

    Controller(
            const yaml::Configuration& configuration);

    void run();

    utils::ReturnCode reload_allowed_topics(
            const std::shared_ptr<ddspipe::core::AllowedTopicList>& allowed_topics);

protected:

    static fastrtps::types::DynamicData_ptr get_dynamic_data(
        const fastrtps::types::DynamicType_ptr& dyn_type,
        const ddspipe::core::types::RtpsPayloadData& data) noexcept;

    void data_stream_callback_(
            const ddspipe::core::types::DdsTopic& topic,
            const fastrtps::types::DynamicType_ptr& dyn_type,
            const ddspipe::core::types::RtpsPayloadData& data);

    void data_stream_callback_verbose_(
            const ddspipe::core::types::DdsTopic& topic,
            const fastrtps::types::DynamicType_ptr& dyn_type,
            const ddspipe::core::types::RtpsPayloadData& data);

    bool verbose_argument(
            const std::string& argument) const noexcept;

    bool all_argument(const std::string& argument) const noexcept;

    void participants_command(const std::vector<std::string>& arguments) noexcept;

    void print_command(const std::vector<std::string>& arguments) noexcept;

    bool search_topic_(
            const std::string& topic_name,
            ddspipe::core::types::DdsTopic& topic) const;

    Backend backend_;

    Input input_;

    View view_;

    std::shared_ptr<eprosima::spy::participants::SpyModel> model_;
};

} /* namespace spy */
} /* namespace eprosima */
