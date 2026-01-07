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

#include <fastddsspy_participants/model/InstanceCache.hpp>

#include <sstream>
#include <fastdds/dds/xtypes/dynamic_types/MemberDescriptor.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicDataFactory.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicTypeBuilderFactory.hpp>
#include <fastdds/dds/topic/TopicDataType.hpp>
#include <fastdds/dds/xtypes/utils.hpp>
#include <fastdds/dds/log/Log.hpp>

namespace eprosima {
namespace spy {
namespace participants {

bool InstanceCache::add_or_update_instance(
        const ddspipe::core::types::DdsTopic& topic,
        const fastdds::dds::DynamicType::_ref_type& dyn_type,
        const ddspipe::core::types::RtpsPayloadData& data) noexcept
{
    try
    {
        std::unique_lock<std::shared_timed_mutex> lock(mutex_);

        auto writer_guid = data.source_guid;
        auto instance_handle = data.instanceHandle;

        extract_key_field_names_(topic.m_topic_name, dyn_type);

        auto& topic_instances = instances_by_topic_[topic.m_topic_name];

        auto instance_it = topic_instances.find(instance_handle);
        bool instance_already_cached = (instance_it != topic_instances.end() &&
                !instance_it->second.key_representation.empty());

        // Fast path: instance already cached
        if (instance_already_cached)
        {
            instance_it->second.active_writers.insert(writer_guid);
            instance_it->second.last_seen = std::chrono::system_clock::now();
            return true;
        }

        // Check instance limit
        if (topic_instances.size() >= MAX_INSTANCES_PER_TOPIC &&
                instance_it == topic_instances.end())
        {
            EPROSIMA_LOG_WARNING(FASTDDSSPY_INSTANCECACHE,
                    "Maximum instances reached for topic: " << topic.m_topic_name);
            return false;
        }

        lock.unlock();

        // Slow path: serialize key fields for new instance
        std::string key_json = serialize_key_to_json_(dyn_type, data);

        if (key_json.empty())
        {
            return false;
        }

        // Store the new instance
        lock.lock();
        auto& instance_info = topic_instances[instance_handle];
        instance_info.key_representation = key_json;
        instance_info.active_writers.insert(writer_guid);
        instance_info.last_seen = std::chrono::system_clock::now();

        return true;
    }
    catch (const std::exception& e)
    {
        EPROSIMA_LOG_WARNING(FASTDDSSPY_INSTANCECACHE,
                "Exception in add_or_update_instance: " << e.what());
        return false;
    }
    catch (...)
    {
        EPROSIMA_LOG_WARNING(FASTDDSSPY_INSTANCECACHE,
                "Unknown exception in add_or_update_instance");
        return false;
    }
}

std::set<std::string> InstanceCache::get_active_instances(
        const std::string& topic_name) const noexcept
{
    std::shared_lock<std::shared_timed_mutex> lock(mutex_);

    auto it = instances_by_topic_.find(topic_name);
    if (it == instances_by_topic_.end())
    {
        return {};
    }

    std::set<std::string> result;
    for (const auto& instance_pair : it->second)
    {
        const InstanceInfo& info = instance_pair.second;
        if (!info.active_writers.empty())
        {
            result.insert(info.key_representation);
        }
    }

    return result;
}

std::vector<std::string> InstanceCache::get_key_fields(
        const std::string& topic_name) const noexcept
{
    std::shared_lock<std::shared_timed_mutex> lock(mutex_);

    auto it = key_fields_cache_.find(topic_name);
    if (it != key_fields_cache_.end())
    {
        return it->second;
    }

    return {};
}

void InstanceCache::on_writer_changed(
        const ddspipe::core::types::Guid& writer_guid,
        const std::string& topic_name,
        bool active) noexcept
{
    if (active)
    {
        return; // Writer additions are handled in add_or_update_instance
    }

    std::unique_lock<std::shared_timed_mutex> lock(mutex_);

    auto topic_it = instances_by_topic_.find(topic_name);
    if (topic_it == instances_by_topic_.end())
    {
        return;
    }

    auto& topic_instances = topic_it->second;
    for (auto it = topic_instances.begin(); it != topic_instances.end();)
    {
        it->second.active_writers.erase(writer_guid);

        // Remove instance if no more active writers
        if (it->second.active_writers.empty())
        {
            EPROSIMA_LOG_INFO(FASTDDSSPY_INSTANCECACHE,
                    "Removing instance on topic " << topic_name
                                                  << " - no more active writers");
            it = topic_instances.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

void InstanceCache::clear() noexcept
{
    std::unique_lock<std::shared_timed_mutex> lock(mutex_);
    instances_by_topic_.clear();
    key_fields_cache_.clear();
}

// Private methods

void InstanceCache::extract_key_field_names_(
        const std::string& topic_name,
        const fastdds::dds::DynamicType::_ref_type& dyn_type) noexcept
{
    // Already cached?
    if (key_fields_cache_.find(topic_name) != key_fields_cache_.end())
    {
        return;
    }

    try
    {
        std::vector<std::string> key_names;
        fastdds::dds::DynamicTypeMembersById members;

        if (fastdds::dds::RETCODE_OK == dyn_type->get_all_members(members))
        {
            for (const auto& member_pair : members)
            {
                const auto& member = member_pair.second;
                auto descriptor = fastdds::dds::traits<fastdds::dds::MemberDescriptor>::make_shared();
                if (fastdds::dds::RETCODE_OK == member->get_descriptor(descriptor) &&
                        descriptor->is_key())
                {
                    key_names.push_back(member->get_name().to_string());
                }
            }
        }

        key_fields_cache_[topic_name] = key_names;
    }
    catch (const std::exception& e)
    {
        EPROSIMA_LOG_WARNING(FASTDDSSPY_INSTANCECACHE,
                "Exception extracting key fields: " << e.what());
    }
}

std::string InstanceCache::serialize_key_to_json_(
        const fastdds::dds::DynamicType::_ref_type& dyn_type,
        const ddspipe::core::types::RtpsPayloadData& data) noexcept
{
    try
    {
        // Deserialize payload to DynamicData
        auto& data_no_const = const_cast<ddspipe::core::types::RtpsPayloadData&>(data);
        fastdds::dds::DynamicPubSubType pubsub_type(dyn_type);
        auto dyn_data = fastdds::dds::DynamicDataFactory::get_instance()->create_data(dyn_type);

        if (!pubsub_type.deserialize(data_no_const.payload, &dyn_data))
        {
            EPROSIMA_LOG_WARNING(FASTDDSSPY_INSTANCECACHE,
                    "Failed to deserialize payload");
            return "";
        }

        // Create key-only DynamicData
        auto key_data = create_key_only_dynamic_data_(dyn_type, dyn_data);
        if (!key_data)
        {
            EPROSIMA_LOG_WARNING(FASTDDSSPY_INSTANCECACHE,
                    "Failed to create key-only DynamicData");
            return "";
        }

        // Serialize to JSON
        std::stringstream key_json_stream;
        if (fastdds::dds::RETCODE_OK != fastdds::dds::json_serialize(
                    key_data,
                    fastdds::dds::DynamicDataJsonFormat::EPROSIMA,
                    key_json_stream))
        {
            EPROSIMA_LOG_WARNING(FASTDDSSPY_INSTANCECACHE,
                    "Failed to serialize keys to JSON");
            return "";
        }

        return key_json_stream.str();
    }
    catch (const std::exception& e)
    {
        EPROSIMA_LOG_WARNING(FASTDDSSPY_INSTANCECACHE,
                "Exception serializing key to JSON: " << e.what());
        return "";
    }
}

fastdds::dds::DynamicData::_ref_type InstanceCache::create_key_only_dynamic_data_(
        const fastdds::dds::DynamicType::_ref_type& original_type,
        const fastdds::dds::DynamicData::_ref_type& original_data) noexcept
{
    try
    {
        fastdds::dds::DynamicTypeMembersById members;
        if (fastdds::dds::RETCODE_OK != original_type->get_all_members(members))
        {
            return nullptr;
        }

        // Build key-only type
        auto type_factory = fastdds::dds::DynamicTypeBuilderFactory::get_instance();
        auto key_type_desc = fastdds::dds::traits<fastdds::dds::TypeDescriptor>::make_shared();
        key_type_desc->kind(fastdds::dds::TK_STRUCTURE);
        key_type_desc->name(original_type->get_name().to_string() + "_KeysOnly");

        auto key_type_builder = type_factory->create_type(key_type_desc);

        bool has_keys = false;
        for (const auto& member_pair : members)
        {
            const auto& member = member_pair.second;
            auto member_desc = fastdds::dds::traits<fastdds::dds::MemberDescriptor>::make_shared();

            if (fastdds::dds::RETCODE_OK != member->get_descriptor(member_desc))
            {
                continue;
            }

            if (member_desc->is_key())
            {
                has_keys = true;

                auto new_member_desc = fastdds::dds::traits<fastdds::dds::MemberDescriptor>::make_shared();
                new_member_desc->name(member_desc->name());
                new_member_desc->type(member_desc->type());
                new_member_desc->id(member_desc->id());

                key_type_builder->add_member(new_member_desc);
            }
        }

        if (!has_keys)
        {
            return nullptr;
        }

        auto key_type = key_type_builder->build();
        auto key_data = fastdds::dds::DynamicDataFactory::get_instance()->create_data(key_type);

        // Copy key values
        for (const auto& member_pair : members)
        {
            const auto& member = member_pair.second;
            auto member_desc = fastdds::dds::traits<fastdds::dds::MemberDescriptor>::make_shared();

            if (fastdds::dds::RETCODE_OK != member->get_descriptor(member_desc) ||
                    !member_desc->is_key())
            {
                continue;
            }

            auto loaned_value = original_data->loan_value(member->get_id());
            if (loaned_value)
            {
                key_data->set_complex_value(member->get_id(), loaned_value);
                original_data->return_loaned_value(loaned_value);
            }
        }

        return key_data;
    }
    catch (const std::exception& e)
    {
        EPROSIMA_LOG_WARNING(FASTDDSSPY_INSTANCECACHE,
                "Exception in create_key_only_dynamic_data: " << e.what());
        return nullptr;
    }
    catch (...)
    {
        return nullptr;
    }
}

} /* namespace participants */
} /* namespace spy */
} /* namespace eprosima */
