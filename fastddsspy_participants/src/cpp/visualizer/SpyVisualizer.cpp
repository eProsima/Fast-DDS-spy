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

#include <fastddsspy_participants/visualizer/SpyVisualizer.hpp>

namespace eprosima {
namespace spy {
namespace participants {

void SpyVisualizer::print_topics(
        std::ostream& target /* = std::cout */) const noexcept
{
    std::set<ddspipe::core::types::DdsTopic> topics = get_topics_();

    target << "  Topics:\n";
    for (const auto& topic : topics)
    {
        std::string topic_discovered = (is_topic_type_discovered(topic) ? "" : " [Not DataType available]");
        target << "    - " << topic.topic_name() << " [" << topic.type_name << "]" << topic_discovered << "\n";
    }
    target << std::endl;
}

std::set<ddspipe::core::types::DdsTopic> SpyVisualizer::get_topics_() const noexcept
{
    std::set<ddspipe::core::types::DdsTopic> result;

    std::shared_lock<EndpointInfoDatabase> _(endpoint_database_);

    for (const auto& endpoint : endpoint_database_)
    {
        result.insert(endpoint.second.topic);
    }
    return result;
}

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
