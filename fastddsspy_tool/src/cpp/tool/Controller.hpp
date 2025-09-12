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
#include <mutex>

#include <fastdds/dds/xtypes/dynamic_types/DynamicType.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicData.hpp>

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

    void one_shot_run(
            const std::vector<std::string>& args);

    utils::ReturnCode reload_configuration(
            yaml::Configuration& new_configuration);

protected:

    void run_command_(
            const utils::Command<CommandValue>& command);

    ////////////////////////////
    // DATA STREAM CALLBACKS
    static fastdds::dds::DynamicData::_ref_type get_dynamic_data_(
            const fastdds::dds::DynamicType::_ref_type& dyn_type,
            const ddspipe::core::types::RtpsPayloadData& data) noexcept;

    void data_stream_callback_(
            const ddspipe::core::types::DdsTopic& topic,
            const fastdds::dds::DynamicType::_ref_type& dyn_type,
            const ddspipe::core::types::RtpsPayloadData& data);

    void data_stream_callback_verbose_(
            const ddspipe::core::types::DdsTopic& topic,
            const fastdds::dds::DynamicType::_ref_type& dyn_type,
            const ddspipe::core::types::RtpsPayloadData& data);

    /////////////////////
    // PARSE ARGUMENTS
    bool verbose_argument_(
            const std::string& argument) const noexcept;

    bool verbose_verbose_argument_(
            const std::string& argument) const noexcept;

    bool idl_argument_(
            const std::string& argument) const noexcept;

    bool all_argument_(
            const std::string& argument) const noexcept;

    ////////////////////////////////////////////////////////////////////////////////////
    // COMMANDS ROUTINES

    /////////////////////
    // ENTITIES
    void participants_command_(
            const std::vector<std::string>& arguments) noexcept;
    void writers_command_(
            const std::vector<std::string>& arguments) noexcept;
    void readers_command_(
            const std::vector<std::string>& arguments) noexcept;
    void topics_command_(
            const std::vector<std::string>& arguments) noexcept;

    /////////////////////
    // DATA STREAM
    void print_command_(
            const std::vector<std::string>& arguments) noexcept;

    /////////////////////
    // FILTER
    void filter_command_(
            const std::vector<std::string>& arguments) noexcept;

    /////////////////////
    // AUXILIARY
    void version_command_(
            const std::vector<std::string>& arguments) noexcept;
    void help_command_(
            const std::vector<std::string>& arguments) noexcept;
    void error_command_(
            const std::vector<std::string>& arguments) noexcept;

    /////////////////////
    // VARIABLES
    Backend backend_;

    Input input_;

    View view_;

    std::shared_ptr<eprosima::spy::participants::SpyModel> model_;

    yaml::Configuration configuration_;

private:

    template <typename SimpleF, typename VerboseF, typename specificF>
    void dds_entity_command__(
            const std::vector<std::string>& arguments,
            SimpleF simple_function,
            VerboseF verbose_function,
            specificF specific_function,
            const char* entity_name) noexcept;

    /*bool check_filter_ditc_category(
        std::string category) noexcept;*/

    std::mutex view_mutex_;

    std::map<std::string, std::set<std::string>> filter_dict;

    std::set<std::string> allowed_filters_categories_;

};

} /* namespace spy */
} /* namespace eprosima */

// Include implementation template file
#include "impl/Controller.ipp"
