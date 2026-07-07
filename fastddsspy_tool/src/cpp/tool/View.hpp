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

#include <ostream>
#include <string>

#include <cpp_utils/Formatter.hpp>

#include <fastddsspy_participants/model/DataStreamer.hpp>
#include <ddspipe_yaml/Yaml.hpp>

namespace eprosima {
namespace spy {

class View
{
public:

    enum class Mode
    {
        plain,
        ui_bridge_jsonl
    };

    explicit View(
            Mode mode = Mode::plain);

    void print_initial();

    void show(
            const std::string& value);

    void show(
            const char* value);

    void show(
            const Yaml& value);

    void show(
            const utils::Formatter& value);

    void show_block(
            const std::string& value);

    void show_error(
            const std::string& value);

    void show_error(
            const utils::Formatter& value);

    void show_status(
            const std::string& value);

    void show_stream_started(
            const std::string& value);

    void show_stream_stopped(
            const std::string& value);

    bool bridge_mode() const noexcept;

private:

    void emit_json_event_(
            const char* type,
            const std::string& value);

    static std::string timestamp_();

    Mode mode_;
};

} /* namespace spy */
} /* namespace eprosima */
