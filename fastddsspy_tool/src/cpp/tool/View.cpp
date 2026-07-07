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

#include <chrono>

#include <cpp_utils/utils.hpp>

#include <ddspipe_yaml/Yaml.hpp>
#include <nlohmann/json.hpp>

#include "View.hpp"

namespace eprosima {
namespace spy {

View::View(
        Mode mode)
    : mode_(mode)
{
    // Do nothing
}

void View::print_initial()
{
    static const std::string banner =
"    ______           __  ____  ____  _____      _____ ____       \n"
"   / ____/___ ______/ /_/ __ \\/ __ \\/ ___/     / ___// __ \\__  __\n"
"  / /_  / __ `/ ___/ __/ / / / / / /\\__ \\______\\__ \\/ /_/ / / / /\n"
" / __/ / /_/ (__  ) /_/ /_/ / /_/ /___/ /_____/__/ / ____/ /_/ / \n"
"/_/    \\__,_/____/\\__/_____/_____//____/     /____/_/    \\__, /  \n"
"                                                        /____/   \n";

    if (bridge_mode())
    {
        emit_json_event_("banner", banner);
        return;
    }

    std::cout << "\033[1;32m";
    std::cout << " ____|             |        __ \\   __ \\    ___|        ___|                " << std::endl;
    std::cout << " |     _` |   __|  __|      |   |  |   | \\___ \\      \\___ \\   __ \\   |   | " << std::endl;
    std::cout << " __|  (   | \\__ \\  |        |   |  |   |       |           |  |   |  |   | " << std::endl;
    std::cout << "_|   \\__,_| ____/ \\__|     ____/  ____/  _____/      _____/   .__/  \\__, | " << std::endl;
    std::cout << "                                                              _|     ____/ " << std::endl;
    std::cout << "\033[0m" << std::endl;
}

void View::show(
        const std::string& value)
{
    if (bridge_mode())
    {
        emit_json_event_("output", value);
    }
    else
    {
        show(value.c_str());
    }
}

void View::show(
        const char* value)
{
    if (bridge_mode())
    {
        emit_json_event_("output", value);
    }
    else
    {
        std::cout << value << std::endl;
    }
}

void View::show(
        const Yaml& value)
{
    show(utils::generic_to_string(value));
}

void View::show(
        const utils::Formatter& value)
{
    show(value.to_string());
}

void View::show_block(
        const std::string& value)
{
    if (bridge_mode())
    {
        emit_json_event_("output", value);
    }
    else
    {
        std::cout << value << std::flush;
    }
}

void View::show_error(
        const std::string& value)
{
    if (bridge_mode())
    {
        emit_json_event_("error", value);
    }
    else
    {
        std::cout << "\033[1;31m" << value << "\033[0m" << std::endl;
    }
}

void View::show_error(
        const utils::Formatter& value)
{
    show_error(value.to_string());
}

void View::show_status(
        const std::string& value)
{
    if (bridge_mode())
    {
        emit_json_event_("status", value);
    }
}

void View::show_stream_started(
        const std::string& value)
{
    if (bridge_mode())
    {
        emit_json_event_("stream_started", value);
    }
}

void View::show_stream_stopped(
        const std::string& value)
{
    if (bridge_mode())
    {
        emit_json_event_("stream_stopped", value);
    }
}

bool View::bridge_mode() const noexcept
{
    return mode_ == Mode::ui_bridge_jsonl;
}

void View::emit_json_event_(
        const char* type,
        const std::string& value)
{
    nlohmann::json event = {
        {"type", type},
        {"message", value},
        {"timestamp", timestamp_()}
    };

    std::cout << event.dump() << std::endl;
}

std::string View::timestamp_()
{
    const auto now = std::chrono::system_clock::now().time_since_epoch();
    const auto milliseconds = std::chrono::duration_cast<std::chrono::milliseconds>(now).count();
    return std::to_string(milliseconds);
}

} /* namespace spy */
} /* namespace eprosima */
