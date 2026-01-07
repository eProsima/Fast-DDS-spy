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

#ifndef _FASTDDSSPY_PARTICIPANTS_MODEL_INSTANCECACHE_HPP_
#define _FASTDDSSPY_PARTICIPANTS_MODEL_INSTANCECACHE_HPP_

#include <map>
#include <set>
#include <string>
#include <vector>
#include <chrono>
#include <shared_mutex>

#include <ddspipe_core/types/data/RtpsPayloadData.hpp>
#include <ddspipe_core/types/topic/dds/DdsTopic.hpp>
#include <ddspipe_core/types/participant/ParticipantId.hpp>

#include <fastdds/dds/xtypes/dynamic_types/DynamicType.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicData.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicPubSubType.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicDataFactory.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicTypeBuilder.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicTypeBuilderFactory.hpp>

namespace eprosima {
namespace spy {
namespace participants {

/**
 * @brief Manages instance tracking for DDS topics
 * 
 * This class is responsible for:
 * - Tracking unique instances per topic using instanceHandle
 * - Caching key field representations as JSON
 * - Managing active writers per instance
 * - Cleaning up instances when writers disappear
 */
class InstanceCache
{
public:

    /**
     * @brief Maximum number of instances to track per topic
     */
    static constexpr size_t MAX_INSTANCES_PER_TOPIC = 1000;

    /**
     * @brief Information about a single instance
     */
    struct InstanceInfo
    {
        std::string key_representation;                           // JSON representation of key fields
        std::set<ddspipe::core::types::Guid> active_writers;      // Writers currently publishing this instance
        std::chrono::system_clock::time_point last_seen;          // Last time data was received
    };

    InstanceCache() = default;
    ~InstanceCache() = default;

    /**
     * @brief Add or update an instance in the cache
     * 
     * @param topic The DDS topic
     * @param dyn_type The dynamic type of the topic
     * @param data The RTPS payload data
     * @return true if instance was added/updated, false if cache is full or error occurred
     */
    bool add_or_update_instance(
            const ddspipe::core::types::DdsTopic& topic,
            const fastdds::dds::DynamicType::_ref_type& dyn_type,
            const ddspipe::core::types::RtpsPayloadData& data) noexcept;

    /**
     * @brief Get all active instances for a topic
     * 
     * @param topic_name The name of the topic
     * @return Set of JSON key representations for active instances
     */
    std::set<std::string> get_active_instances(
            const std::string& topic_name) const noexcept;

    /**
     * @brief Get the key field names for a topic
     * 
     * @param topic_name The name of the topic
     * @return Vector of key field names
     */
    std::vector<std::string> get_key_fields(
            const std::string& topic_name) const noexcept;

    /**
     * @brief Handle writer discovery/removal
     * 
     * @param writer_guid The GUID of the writer
     * @param topic_name The name of the topic
     * @param active true if writer is active, false if removed
     */
    void on_writer_changed(
            const ddspipe::core::types::Guid& writer_guid,
            const std::string& topic_name,
            bool active) noexcept;

    /**
     * @brief Clear all cached data
     */
    void clear() noexcept;

private:

    /**
     * @brief Extract and cache key field names for a topic
     */
    void extract_key_field_names_(
            const std::string& topic_name,
            const fastdds::dds::DynamicType::_ref_type& dyn_type) noexcept;

    /**
     * @brief Serialize key fields to JSON
     */
    std::string serialize_key_to_json_(
            const fastdds::dds::DynamicType::_ref_type& dyn_type,
            const ddspipe::core::types::RtpsPayloadData& data) noexcept;

    /**
     * @brief Create a DynamicData containing only key fields
     */
    fastdds::dds::DynamicData::_ref_type create_key_only_dynamic_data_(
            const fastdds::dds::DynamicType::_ref_type& original_type,
            const fastdds::dds::DynamicData::_ref_type& original_data) noexcept;

    // Thread-safe access
    mutable std::shared_timed_mutex mutex_;

    // topic_name -> {instance_handle -> InstanceInfo}
    std::map<std::string, std::map<ddspipe::core::types::InstanceHandle, InstanceInfo>> instances_by_topic_;

    // topic_name -> [key_field_names]
    std::map<std::string, std::vector<std::string>> key_fields_cache_;
};

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */

#endif // _FASTDDSSPY_PARTICIPANTS_MODEL_INSTANCECACHE_HPP_
