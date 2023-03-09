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

#include <fastddsspy_participants/types/EndpointInfo.hpp>

namespace eprosima {
namespace spy {
namespace participants {

ddspipe::core::types::TopicInternalTypeDiscriminator EndpointInfoData::internal_type_discriminator() const noexcept
{
    return INTERNAL_TOPIC_TYPE_ENDPOINT_INFO;
}

ddspipe::core::types::DdsTopic endpoint_info_topic() noexcept
{
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = ENDPOINT_INFO_TOPIC_NAME;
    topic.type_name = ENDPOINT_INFO_DATA_TYPE_NAME;
    topic.m_internal_type_discriminator = INTERNAL_TOPIC_TYPE_ENDPOINT_INFO;

    return topic;
}

bool is_endpoint_info_topic(
        const ddspipe::core::ITopic& topic) noexcept
{
    return topic.internal_type_discriminator() == INTERNAL_TOPIC_TYPE_ENDPOINT_INFO;
}

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
