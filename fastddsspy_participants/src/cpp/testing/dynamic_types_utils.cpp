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

#include <fastddsspy_participants/testing/dynamic_types_utils.hpp>

#include <fastdds/dds/xtypes/dynamic_types/DynamicTypeBuilder.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicTypeBuilderFactory.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicData.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicDataFactory.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicPubSubType.hpp>

namespace eprosima {
namespace spy {
namespace participants {
namespace testing {

fastdds::dds::DynamicType::_ref_type create_test_type_with_keys(
        const std::string& type_name,
        const std::vector<std::string>& key_field_names)
{
    auto factory = fastdds::dds::DynamicTypeBuilderFactory::get_instance();
    auto type_desc = fastdds::dds::traits<fastdds::dds::TypeDescriptor>::make_shared();
    type_desc->kind(fastdds::dds::TK_STRUCTURE);
    type_desc->name(type_name);
    
    auto type_builder = factory->create_type(type_desc);
    
    // Add key fields
    for (size_t i = 0; i < key_field_names.size(); ++i)
    {
        auto member_desc = fastdds::dds::traits<fastdds::dds::MemberDescriptor>::make_shared();
        member_desc->name(key_field_names[i]);
        member_desc->type(factory->get_primitive_type(fastdds::dds::TK_INT32));
        member_desc->id(i);
        member_desc->is_key(true);
        type_builder->add_member(member_desc);
    }
    
    // Add a non-key field
    auto value_desc = fastdds::dds::traits<fastdds::dds::MemberDescriptor>::make_shared();
    value_desc->name("value");
    value_desc->type(factory->get_primitive_type(fastdds::dds::TK_INT32));
    value_desc->id(key_field_names.size());
    value_desc->is_key(false);
    type_builder->add_member(value_desc);
    
    return type_builder->build();
}

std::unique_ptr<ddspipe::core::types::RtpsPayloadData> create_test_data_with_keys(
        const fastdds::dds::DynamicType::_ref_type& dyn_type,
        const std::map<std::string, int32_t>& key_values,
        const ddspipe::core::types::Guid& writer_guid)
{
    // Create DynamicData
    auto dyn_data = fastdds::dds::DynamicDataFactory::get_instance()->create_data(dyn_type);
    
    // Set key values
    for (const auto& kv : key_values)
    {
        fastdds::dds::DynamicTypeMembersById members;
        dyn_type->get_all_members(members);
        
        for (const auto& member_pair : members)
        {
            if (member_pair.second->get_name().to_string() == kv.first)
            {
                dyn_data->set_int32_value(member_pair.second->get_id(), kv.second);
                break;
            }
        }
    }
    
    // Set non-key value
    dyn_data->set_int32_value(key_values.size(), 42);
    
    // Serialize to payload
    fastdds::dds::DynamicPubSubType pubsub_type(dyn_type);
    
    auto data = std::make_unique<ddspipe::core::types::RtpsPayloadData>();
    
    data->payload.reserve(pubsub_type.calculate_serialized_size(
        &dyn_data, fastdds::dds::DEFAULT_DATA_REPRESENTATION));
    
    pubsub_type.serialize(&dyn_data, data->payload, fastdds::dds::DEFAULT_DATA_REPRESENTATION);
    
    data->source_guid = writer_guid;
    
    pubsub_type.compute_key(&dyn_data, data->instanceHandle, false);
    
    return data;
}

} /* namespace testing */
} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
