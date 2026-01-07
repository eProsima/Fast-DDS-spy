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

#include <cpp_utils/testing/gtest_aux.hpp>
#include <gtest/gtest.h>

#include <fastdds/dds/xtypes/dynamic_types/DynamicType.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicTypeBuilder.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicTypeBuilderFactory.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicData.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicDataFactory.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicPubSubType.hpp>

#include <fastddsspy_participants/model/InstanceCache.hpp>

#include <fastddsspy_participants/testing/random_values.hpp>
#include <fastddsspy_participants/testing/dynamic_types_utils.hpp>

using namespace eprosima;

/**
 * Test: Basic instance addition
 */
TEST(InstanceCacheTest, add_single_instance)
{
    spy::participants::InstanceCache cache;
    
    // Create topic
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "TestTopic";
    topic.type_name = "TestType";
    
    // Create type with key
    std::vector<std::string> key_names = {"id"};
    auto dyn_type = eprosima::spy::participants::testing::create_test_type_with_keys("TestType", key_names);
    
    // Create data
    auto writer_guid = ddspipe::core::testing::random_guid();
    std::map<std::string, int32_t> key_values = {{"id", 100}};
    auto data = eprosima::spy::participants::testing::create_test_data_with_keys(dyn_type, key_values, writer_guid);
    
    // Add instance
    bool result = cache.add_or_update_instance(topic, dyn_type, *data);
    ASSERT_TRUE(result);
    
    // Check instance exists
    auto instances = cache.get_active_instances(topic.m_topic_name);
    ASSERT_EQ(instances.size(), 1);
    ASSERT_TRUE(instances.begin()->find("100") != std::string::npos);
    
    // Check key fields
    auto key_fields = cache.get_key_fields(topic.m_topic_name);
    ASSERT_EQ(key_fields.size(), 1);
    ASSERT_EQ(key_fields[0], "id");
}

/**
 * Test: Multiple instances
 */
TEST(InstanceCacheTest, add_multiple_instances)
{
    spy::participants::InstanceCache cache;
    
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "MultiInstanceTopic";
    topic.type_name = "MultiInstanceType";
    
    std::vector<std::string> key_names = {"sensor_id"};
    auto dyn_type = eprosima::spy::participants::testing::create_test_type_with_keys("MultiInstanceType", key_names);
    
    auto writer_guid = ddspipe::core::testing::random_guid();
    
    // Add 3 different instances
    for (int i = 1; i <= 3; ++i)
    {
        std::map<std::string, int32_t> key_values = {{"sensor_id", i * 10}};
        auto data = eprosima::spy::participants::testing::create_test_data_with_keys(dyn_type, key_values, writer_guid);
        bool result = cache.add_or_update_instance(topic, dyn_type, *data);
        ASSERT_TRUE(result);
    }
    
    auto instances = cache.get_active_instances(topic.m_topic_name);
    ASSERT_EQ(instances.size(), 3);
}

/**
 * Test: Update existing instance
 */
TEST(InstanceCacheTest, update_existing_instance)
{
    spy::participants::InstanceCache cache;
    
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "UpdateTopic";
    topic.type_name = "UpdateType";
    
    std::vector<std::string> key_names = {"id"};
    auto dyn_type = eprosima::spy::participants::testing::create_test_type_with_keys("UpdateType", key_names);
    
    auto writer_guid = ddspipe::core::testing::random_guid();
    std::map<std::string, int32_t> key_values = {{"id", 500}};
    
    // Add instance first time
    auto data1 = eprosima::spy::participants::testing::create_test_data_with_keys(dyn_type, key_values, writer_guid);
    cache.add_or_update_instance(topic, dyn_type, *data1);
    
    auto instances_before = cache.get_active_instances(topic.m_topic_name);
    ASSERT_EQ(instances_before.size(), 1);

    // Add same instance again (update)
    auto data2 = eprosima::spy::participants::testing::create_test_data_with_keys(dyn_type, key_values, writer_guid);
    cache.add_or_update_instance(topic, dyn_type, *data2);
    
    auto instances_after = cache.get_active_instances(topic.m_topic_name);
    
    // Should still be 1 instance
    ASSERT_EQ(instances_after.size(), 1);
}

/**
 * Test: Multiple writers for same instance
 */
TEST(InstanceCacheTest, multiple_writers_same_instance)
{
    spy::participants::InstanceCache cache;
    
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "MultiWriterTopic";
    topic.type_name = "MultiWriterType";
    
    std::vector<std::string> key_names = {"id"};
    auto dyn_type = eprosima::spy::participants::testing::create_test_type_with_keys("MultiWriterType", key_names);
    
    std::map<std::string, int32_t> key_values = {{"id", 999}};
    
    // Add from writer 1
    auto writer_guid_1 = ddspipe::core::testing::random_guid(1);
    auto data1 = eprosima::spy::participants::testing::create_test_data_with_keys(dyn_type, key_values, writer_guid_1);
    cache.add_or_update_instance(topic, dyn_type, *data1);
    
    // Add from writer 2 (same key)
    auto writer_guid_2 = ddspipe::core::testing::random_guid(2);
    auto data2 = eprosima::spy::participants::testing::create_test_data_with_keys(dyn_type, key_values, writer_guid_2);
    cache.add_or_update_instance(topic, dyn_type, *data2);
    
    auto instances = cache.get_active_instances(topic.m_topic_name);
    
    // Should still be 1 instance (same key)
    ASSERT_EQ(instances.size(), 1);
    
    // Remove writer 1
    cache.on_writer_changed(writer_guid_1, topic.m_topic_name, false);
    
    instances = cache.get_active_instances(topic.m_topic_name);
    
    // Instance should still exist (writer 2 is still active)
    ASSERT_EQ(instances.size(), 1);
    
    // Remove writer 2
    cache.on_writer_changed(writer_guid_2, topic.m_topic_name, false);
    
    instances = cache.get_active_instances(topic.m_topic_name);
    
    // Now instance should be gone
    ASSERT_EQ(instances.size(), 0);
}

/**
 * Test: Writer removal
 */
TEST(InstanceCacheTest, writer_removal)
{
    spy::participants::InstanceCache cache;
    
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "WriterRemovalTopic";
    topic.type_name = "WriterRemovalType";
    
    std::vector<std::string> key_names = {"id"};
    auto dyn_type = eprosima::spy::participants::testing::create_test_type_with_keys("WriterRemovalType", key_names);
    
    auto writer_guid = ddspipe::core::testing::random_guid();
    std::map<std::string, int32_t> key_values = {{"id", 777}};
    
    // Add instance
    auto data = eprosima::spy::participants::testing::create_test_data_with_keys(dyn_type, key_values, writer_guid);
    cache.add_or_update_instance(topic, dyn_type, *data);
    
    auto instances_before = cache.get_active_instances(topic.m_topic_name);
    ASSERT_EQ(instances_before.size(), 1);
    
    // Remove writer
    cache.on_writer_changed(writer_guid, topic.m_topic_name, false);
    
    auto instances_after = cache.get_active_instances(topic.m_topic_name);
    
    // Instance should be removed
    ASSERT_EQ(instances_after.size(), 0);
}

/**
 * Test: Multiple key fields
 */
TEST(InstanceCacheTest, multiple_key_fields)
{
    spy::participants::InstanceCache cache;
    
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "MultiKeyTopic";
    topic.type_name = "MultiKeyType";
    
    std::vector<std::string> key_names = {"sensor_id", "location_id", "device_id"};
    auto dyn_type = eprosima::spy::participants::testing::create_test_type_with_keys("MultiKeyType", key_names);
    
    auto writer_guid = ddspipe::core::testing::random_guid();
    
    // Add instance with all three keys
    std::map<std::string, int32_t> key_values = {
        {"sensor_id", 1},
        {"location_id", 100},
        {"device_id", 5000}
    };
    
    auto data = eprosima::spy::participants::testing::create_test_data_with_keys(dyn_type, key_values, writer_guid);
    
    bool result = cache.add_or_update_instance(topic, dyn_type, *data);
    ASSERT_TRUE(result);
    
    // Check key fields
    auto key_fields = cache.get_key_fields(topic.m_topic_name);
    ASSERT_EQ(key_fields.size(), 3);
    
    // Check instance
    auto instances = cache.get_active_instances(topic.m_topic_name);
    ASSERT_EQ(instances.size(), 1);
    
    // Verify all key values are in JSON
    const auto& instance_json = *instances.begin();
    ASSERT_TRUE(instance_json.find("1") != std::string::npos);
    ASSERT_TRUE(instance_json.find("100") != std::string::npos);
    ASSERT_TRUE(instance_json.find("5000") != std::string::npos);
}

/**
 * Test: Type without keys
 */
TEST(InstanceCacheTest, type_without_keys)
{
    spy::participants::InstanceCache cache;
    
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "NoKeyTopic";
    topic.type_name = "NoKeyType";
    
    // Create type WITHOUT keys
    auto factory = fastdds::dds::DynamicTypeBuilderFactory::get_instance();
    auto type_desc = fastdds::dds::traits<fastdds::dds::TypeDescriptor>::make_shared();
    type_desc->kind(fastdds::dds::TK_STRUCTURE);
    type_desc->name("NoKeyType");
    
    auto type_builder = factory->create_type(type_desc);
    
    // Add only non-key field
    auto value_desc = fastdds::dds::traits<fastdds::dds::MemberDescriptor>::make_shared();
    value_desc->name("value");
    value_desc->type(factory->get_primitive_type(fastdds::dds::TK_INT32));
    value_desc->id(0);
    value_desc->is_key(false);
    type_builder->add_member(value_desc);
    
    auto dyn_type = type_builder->build();
    
    // Create data
    auto dyn_data = fastdds::dds::DynamicDataFactory::get_instance()->create_data(dyn_type);
    dyn_data->set_int32_value(0, 42);
    
    fastdds::dds::DynamicPubSubType pubsub_type(dyn_type);
    auto data = std::make_unique<ddspipe::core::types::RtpsPayloadData>();
    data->payload.reserve(pubsub_type.calculate_serialized_size(
        &dyn_data, fastdds::dds::DEFAULT_DATA_REPRESENTATION));
    pubsub_type.serialize(&dyn_data, data->payload, fastdds::dds::DEFAULT_DATA_REPRESENTATION);
    data->source_guid = ddspipe::core::testing::random_guid();
    pubsub_type.compute_key(&dyn_data, data->instanceHandle, false);
    
    // Add instance
    cache.add_or_update_instance(topic, dyn_type, *data);
    
    // Should have no key fields
    auto key_fields = cache.get_key_fields(topic.m_topic_name);
    ASSERT_EQ(key_fields.size(), 0);
    
    // Should have no instances
    auto instances = cache.get_active_instances(topic.m_topic_name);
    ASSERT_EQ(instances.size(), 0);
}

/**
 * Test: Clear cache
 */
TEST(InstanceCacheTest, clear_cache)
{
    spy::participants::InstanceCache cache;
    
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "ClearTestTopic";
    topic.type_name = "ClearTestType";
    
    std::vector<std::string> key_names = {"id"};
    auto dyn_type = eprosima::spy::participants::testing::create_test_type_with_keys("ClearTestType", key_names);
    
    auto writer_guid = ddspipe::core::testing::random_guid();
    
    // Add some instances
    for (int i = 1; i <= 5; ++i)
    {
        std::map<std::string, int32_t> key_values = {{"id", i}};
        auto data = eprosima::spy::participants::testing::create_test_data_with_keys(dyn_type, key_values, writer_guid);
        cache.add_or_update_instance(topic, dyn_type, *data);
    }
    
    auto instances_before = cache.get_active_instances(topic.m_topic_name);
    ASSERT_EQ(instances_before.size(), 5);
    
    // Clear
    cache.clear();
    
    auto instances_after = cache.get_active_instances(topic.m_topic_name);
    ASSERT_EQ(instances_after.size(), 0);
    
    auto key_fields_after = cache.get_key_fields(topic.m_topic_name);
    ASSERT_EQ(key_fields_after.size(), 0);
}

/**
 * Test: Non-existent topic
 */
TEST(InstanceCacheTest, non_existent_topic)
{
    spy::participants::InstanceCache cache;
    
    // Query non-existent topic
    auto instances = cache.get_active_instances("NonExistentTopic");
    ASSERT_EQ(instances.size(), 0);
    
    auto key_fields = cache.get_key_fields("NonExistentTopic");
    ASSERT_EQ(key_fields.size(), 0);
}

/**
 * Test: Writer removal for non-existent topic
 */
TEST(InstanceCacheTest, writer_removal_non_existent_topic)
{
    spy::participants::InstanceCache cache;
    
    auto writer_guid = ddspipe::core::testing::random_guid();
    
    // Should not crash
    cache.on_writer_changed(writer_guid, "NonExistentTopic", false);
    
    // Verify no side effects
    auto instances = cache.get_active_instances("NonExistentTopic");
    ASSERT_EQ(instances.size(), 0);
}

/**
 * Test: Multiple topics
 */
TEST(InstanceCacheTest, multiple_topics)
{
    spy::participants::InstanceCache cache;
    
    // Topic 1
    ddspipe::core::types::DdsTopic topic1;
    topic1.m_topic_name = "Topic1";
    topic1.type_name = "Type1";
    
    std::vector<std::string> key_names1 = {"id"};
    auto dyn_type1 = eprosima::spy::participants::testing::create_test_type_with_keys("Type1", key_names1);
    
    auto writer_guid1 = ddspipe::core::testing::random_guid(1);
    std::map<std::string, int32_t> key_values1 = {{"id", 111}};
    auto data1 = eprosima::spy::participants::testing::create_test_data_with_keys(dyn_type1, key_values1, writer_guid1);
    cache.add_or_update_instance(topic1, dyn_type1, *data1);
    
    // Topic 2
    ddspipe::core::types::DdsTopic topic2;
    topic2.m_topic_name = "Topic2";
    topic2.type_name = "Type2";
    
    std::vector<std::string> key_names2 = {"sensor_id"};
    auto dyn_type2 = eprosima::spy::participants::testing::create_test_type_with_keys("Type2", key_names2);
    
    auto writer_guid2 = ddspipe::core::testing::random_guid(2);
    std::map<std::string, int32_t> key_values2 = {{"sensor_id", 222}};
    auto data2 = eprosima::spy::participants::testing::create_test_data_with_keys(dyn_type2, key_values2, writer_guid2);
    cache.add_or_update_instance(topic2, dyn_type2, *data2);
    
    // Verify both topics have instances
    auto instances1 = cache.get_active_instances(topic1.m_topic_name);
    ASSERT_EQ(instances1.size(), 1);
    
    auto instances2 = cache.get_active_instances(topic2.m_topic_name);
    ASSERT_EQ(instances2.size(), 1);
    
    // Verify key fields are correct for each topic
    auto key_fields1 = cache.get_key_fields(topic1.m_topic_name);
    ASSERT_EQ(key_fields1.size(), 1);
    ASSERT_EQ(key_fields1[0], "id");
    
    auto key_fields2 = cache.get_key_fields(topic2.m_topic_name);
    ASSERT_EQ(key_fields2.size(), 1);
    ASSERT_EQ(key_fields2[0], "sensor_id");
}

int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
