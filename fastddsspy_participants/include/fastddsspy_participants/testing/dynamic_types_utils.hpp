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

#ifndef _FASTDDSSPY_PARTICIPANTS_TESTING_DYNAMIC_TYPES_UTILS_HPP_
#define _FASTDDSSPY_PARTICIPANTS_TESTING_DYNAMIC_TYPES_UTILS_HPP_

#include <map>
#include <memory>
#include <string>
#include <vector>

#include <fastdds/dds/xtypes/dynamic_types/DynamicType.hpp>

#include <ddspipe_core/types/dds/Guid.hpp>
#include <ddspipe_core/types/data/RtpsPayloadData.hpp>

namespace eprosima {
namespace spy {
namespace participants {
namespace testing {

/**
 * @brief Create a DynamicType with specified key fields for testing
 *
 * Creates a structure type with:
 * - N key fields (int32) with the specified names
 * - 1 non-key field called "value" (int32)
 *
 * @param type_name Name of the type to create
 * @param key_field_names Vector of key field names
 * @return DynamicType reference
 */
fastdds::dds::DynamicType::_ref_type create_test_type_with_keys(
        const std::string& type_name,
        const std::vector<std::string>& key_field_names);

/**
 * @brief Create test data with specific key values
 *
 * Creates RtpsPayloadData with:
 * - DynamicData serialized with the specified key values
 * - Computed instance handle
 * - Source GUID set
 *
 * @param dyn_type DynamicType to use (should be created with create_test_type_with_keys)
 * @param key_values Map of key field name to value
 * @param writer_guid GUID of the writer
 * @return Unique pointer to RtpsPayloadData
 */
std::unique_ptr<ddspipe::core::types::RtpsPayloadData> create_test_data_with_keys(
        const fastdds::dds::DynamicType::_ref_type& dyn_type,
        const std::map<std::string, int32_t>& key_values,
        const ddspipe::core::types::Guid& writer_guid);

} /* namespace testing */
} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */

#endif // _FASTDDSSPY_PARTICIPANTS_TESTING_DYNAMIC_TYPES_UTILS_HPP_