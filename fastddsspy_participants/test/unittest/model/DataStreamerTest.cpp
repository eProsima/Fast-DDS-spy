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

#include <cpp_utils/testing/gtest_aux.hpp>
#include <gtest/gtest.h>

#include <fastdds/dds/xtypes/dynamic_types/DynamicType.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicTypeBuilder.hpp>
#include <fastdds/dds/xtypes/dynamic_types/DynamicTypeBuilderFactory.hpp>
#include <fastdds/dds/xtypes/type_representation/detail/dds_xtypes_typeobject.hpp>

#include <fastddsspy_participants/model/DataStreamer.hpp>

using namespace eprosima;

fastdds::dds::DynamicType::_ref_type create_schema(
        ddspipe::core::types::DdsTopic& topic)
{
    fastdds::dds::TypeDescriptor::_ref_type type_descriptor {fastdds::dds::traits<fastdds::dds::TypeDescriptor>::
                                                             make_shared()};
    type_descriptor->name(topic.type_name);
    fastdds::dds::DynamicTypeBuilder::_ref_type struct_builder {fastdds::dds::DynamicTypeBuilderFactory::get_instance()
                                                                        ->create_type(type_descriptor)};
    fastdds::dds::DynamicType::_ref_type dynamic_type_topic {struct_builder->build()};
    return dynamic_type_topic;
}

TEST(DataStreamerTest, activate_false)
{
    spy::participants::DataStreamer ds;
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "topic1";
    topic.type_name = "type1";

    std::shared_ptr<spy::participants::DataStreamer::CallbackType> cb =
            std::make_shared<spy::participants::DataStreamer::CallbackType>();

    ASSERT_FALSE(ds.activate(topic, cb));
}

TEST(DataStreamerTest, activate_true)
{
    spy::participants::DataStreamer ds;
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "topic1";
    topic.type_name = "type1";

    std::shared_ptr<spy::participants::DataStreamer::CallbackType> cb =
            std::make_shared<spy::participants::DataStreamer::CallbackType>();

    fastdds::dds::DynamicType::_ref_type dynamic_type_topic;
    dynamic_type_topic = create_schema(topic);

    fastdds::dds::xtypes::TypeIdentifier type_identifier;
    ds.add_schema(dynamic_type_topic, type_identifier);

    ASSERT_TRUE(ds.activate(topic, cb));
}

TEST(DataStreamerTest, activate_twice)
{
    spy::participants::DataStreamer ds;

    ddspipe::core::types::DdsTopic topic_1;
    topic_1.m_topic_name = "topic1";
    topic_1.type_name = "type1";

    std::shared_ptr<spy::participants::DataStreamer::CallbackType> cb =
            std::make_shared<spy::participants::DataStreamer::CallbackType>();

    fastdds::dds::DynamicType::_ref_type dynamic_type_topic_1;
    dynamic_type_topic_1 = create_schema(topic_1);

    fastdds::dds::xtypes::TypeIdentifier type_identifier_1;
    ds.add_schema(dynamic_type_topic_1, type_identifier_1);

    ddspipe::core::types::DdsTopic topic_2;
    topic_2.m_topic_name = "topic2";
    topic_2.type_name = "type2";

    fastdds::dds::DynamicType::_ref_type dynamic_type_topic_2;
    dynamic_type_topic_2 = create_schema(topic_2);

    fastdds::dds::xtypes::TypeIdentifier type_identifier_2;
    ds.add_schema(dynamic_type_topic_2, type_identifier_2);

    // is this the correct behaviour?
    ASSERT_TRUE(ds.activate(topic_1, cb));
    ASSERT_TRUE(ds.activate(topic_2, cb));
}

TEST(DataStreamerTest, topic_type_discovered)
{
    spy::participants::DataStreamer ds;

    ddspipe::core::types::DdsTopic topic_1;
    topic_1.m_topic_name = "topic1";
    topic_1.type_name = "type1";

    ASSERT_FALSE(ds.is_topic_type_discovered(topic_1));

    ddspipe::core::types::DdsTopic topic_2;
    topic_2.m_topic_name = "topic2";
    topic_2.type_name = "type2";

    fastdds::dds::DynamicType::_ref_type dynamic_type_topic_2;
    dynamic_type_topic_2 = create_schema(topic_2);

    fastdds::dds::xtypes::TypeIdentifier type_identifier;
    ds.add_schema(dynamic_type_topic_2, type_identifier);

    ASSERT_TRUE(ds.is_topic_type_discovered(topic_2));
}

TEST(DataStreamerTest, deactivate)
{
    spy::participants::DataStreamer ds;
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "topic1";
    topic.type_name = "type1";

    std::atomic<uint32_t> data_sent(0);

    std::shared_ptr<spy::participants::DataStreamer::CallbackType> cb =
            std::make_shared<spy::participants::DataStreamer::CallbackType>(
        [&data_sent]
            (const ddspipe::core::types::DdsTopic& topic,
        const fastdds::dds::DynamicType::_ref_type& type,
        const ddspipe::core::types::RtpsPayloadData& data)
        {
            data_sent++;
        });

    fastdds::dds::DynamicType::_ref_type dynamic_type_topic;
    dynamic_type_topic = create_schema(topic);

    fastdds::dds::xtypes::TypeIdentifier type_identifier;
    ds.add_schema(dynamic_type_topic, type_identifier);

    ds.activate(topic, cb);

    ddspipe::core::types::RtpsPayloadData data;

    unsigned int rand_1 = rand() % 20;

    for (unsigned int i = 0; i < rand_1; i++)
    {
        ds.add_data(topic, data);
    }

    ds.deactivate();

    unsigned int rand_2 = rand() % 20;

    for (unsigned int i = 0; i < rand_2; i++)
    {
        ds.add_data(topic, data);
    }

    ASSERT_EQ(data_sent, rand_1);
}

TEST(DataStreamerTest, add_data)
{
    spy::participants::DataStreamer ds;
    ddspipe::core::types::DdsTopic topic;
    topic.m_topic_name = "topic1";
    topic.type_name = "type1";

    std::atomic<uint32_t> data_sent(0);

    std::shared_ptr<spy::participants::DataStreamer::CallbackType> cb =
            std::make_shared<spy::participants::DataStreamer::CallbackType>(
        [&data_sent]
            (const ddspipe::core::types::DdsTopic& topic,
        const fastdds::dds::DynamicType::_ref_type& type,
        const ddspipe::core::types::RtpsPayloadData& data)
        {
            data_sent++;
        });

    fastdds::dds::DynamicType::_ref_type dynamic_type_topic;
    dynamic_type_topic = create_schema(topic);

    fastdds::dds::xtypes::TypeIdentifier type_identifier;
    ds.add_schema(dynamic_type_topic, type_identifier);

    ds.activate(topic, cb);

    ddspipe::core::types::RtpsPayloadData data;

    unsigned int rand_1 = rand() % 20;

    for (unsigned int i = 0; i < rand_1; i++)
    {
        ds.add_data(topic, data);
    }

    ASSERT_EQ(data_sent, rand_1);
}

TEST(DataStreamerTest, add_data_two_topics)
{
    spy::participants::DataStreamer ds;

    ddspipe::core::types::DdsTopic topic_1;
    topic_1.m_topic_name = "topic1";
    topic_1.type_name = "type1";

    std::atomic<uint32_t> data_sent_1(0);

    std::shared_ptr<spy::participants::DataStreamer::CallbackType> cb_1 =
            std::make_shared<spy::participants::DataStreamer::CallbackType>(
        [&data_sent_1]
            (const ddspipe::core::types::DdsTopic& topic,
        const fastdds::dds::DynamicType::_ref_type& type,
        const ddspipe::core::types::RtpsPayloadData& data)
        {
            data_sent_1++;
        });

    fastdds::dds::DynamicType::_ref_type dynamic_type_topic_1;
    dynamic_type_topic_1 = create_schema(topic_1);

    fastdds::dds::xtypes::TypeIdentifier type_identifier_1;
    ds.add_schema(dynamic_type_topic_1, type_identifier_1);

    ds.activate(topic_1, cb_1);

    ddspipe::core::types::DdsTopic topic_2;
    topic_2.m_topic_name = "topic2";
    topic_2.type_name = "type2";

    std::atomic<uint32_t> data_sent_2(0);

    std::shared_ptr<spy::participants::DataStreamer::CallbackType> cb_2 =
            std::make_shared<spy::participants::DataStreamer::CallbackType>(
        [&data_sent_2]
            (const ddspipe::core::types::DdsTopic& topic,
        const fastdds::dds::DynamicType::_ref_type& type,
        const ddspipe::core::types::RtpsPayloadData& data)
        {
            data_sent_2++;
        });

    fastdds::dds::DynamicType::_ref_type dynamic_type_topic_2;
    dynamic_type_topic_2 = create_schema(topic_2);

    fastdds::dds::xtypes::TypeIdentifier type_identifier_2;
    ds.add_schema(dynamic_type_topic_2, type_identifier_2);

    ds.activate(topic_2, cb_2);

    ddspipe::core::types::RtpsPayloadData data;

    unsigned int rand_1 = rand() % 20;

    for (unsigned int i = 0; i < rand_1; i++)
    {
        ds.add_data(topic_1, data);
    }

    unsigned int rand_2 = rand() % 20;

    for (unsigned int i = 0; i < rand_2; i++)
    {
        ds.add_data(topic_2, data);
    }

    ASSERT_FALSE(data_sent_1);
    ASSERT_EQ(data_sent_2, rand_2);
}

int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
