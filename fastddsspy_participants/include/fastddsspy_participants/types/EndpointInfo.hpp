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

#include <ddspipe_core/types/dds/Guid.hpp>
#include <ddspipe_core/interface/IRoutingData.hpp>
#include <ddspipe_core/types/topic/TopicInternalTypeDiscriminator.hpp>
#include <ddspipe_core/types/topic/dds/DdsTopic.hpp>
#include <ddspipe_core/types/dds/Endpoint.hpp>

#include <fastddsspy_participants/library/library_dll.h>

namespace eprosima {
namespace spy {
namespace participants {

using EndpointInfo = ddspipe::core::types::Endpoint;

/**
 * TODO comment
 */
struct EndpointInfoData : public ddspipe::core::IRoutingData
{

    virtual ddspipe::core::types::TopicInternalTypeDiscriminator internal_type_discriminator() const noexcept override;

    //! Info of the endpoint
    EndpointInfo info{};

    //! Endpoint topic type IDL
    std::string type_idl{};
};

FASTDDSSPY_PARTICIPANTS_DllAPI
ddspipe::core::types::DdsTopic endpoint_info_topic() noexcept;

FASTDDSSPY_PARTICIPANTS_DllAPI
bool is_endpoint_info_topic(
        const ddspipe::core::ITopic& topic) noexcept;

constexpr const char* ENDPOINT_INFO_TOPIC_NAME = "__internal__/endpoint_info";
constexpr const char* ENDPOINT_INFO_DATA_TYPE_NAME = "__internal__::endpoint_info";

const ddspipe::core::types::TopicInternalTypeDiscriminator INTERNAL_TOPIC_TYPE_ENDPOINT_INFO = "endpoint::type::v0";

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
