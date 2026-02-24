// Copyright 2026 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

#include <sstream>
#include <string>

#include <cpp_utils/Log.hpp>

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicTypeBuilder.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicTypeBuilderFactory.hpp>
#include <fastdds/dds/xtypes/utils.hpp>

#include <fastddsspy_participants/library/config.h>

namespace eprosima {
namespace spy {
namespace participants {
namespace detail {

template <typename EndpointDiscoveryInfo>
static std::string get_type_idl(
        const EndpointDiscoveryInfo& info)
{
    std::string type_idl {};

    if (!info.type_information.assigned())
    {
        return type_idl;
    }

    fastdds::dds::xtypes::TypeObject remote_type_object;
    if (eprosima::fastdds::dds::RETCODE_OK !=
            eprosima::fastdds::dds::DomainParticipantFactory::get_instance()->type_object_registry().get_type_object
            (
                info.type_information.type_information.complete().typeid_with_size().type_id(),
                remote_type_object))
    {
        EPROSIMA_LOG_WARNING(FASTDDSSPY_DDS_PARTICIPANT,
                "Error getting type object for type " << info.type_name);
        return type_idl;
    }

    // Build remotely discovered type
    fastdds::dds::DynamicType::_ref_type remote_type =
            eprosima::fastdds::dds::DynamicTypeBuilderFactory::get_instance()->create_type_w_type_object(
        remote_type_object)->build();

    // Serialize DynamicType into its IDL representation
    std::stringstream idl;
    idl_serialize(remote_type, idl);
    type_idl = idl.str();

    return type_idl;
}

} /* namespace detail */
} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
