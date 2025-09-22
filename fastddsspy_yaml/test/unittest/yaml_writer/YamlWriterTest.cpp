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

#include <fastddsspy_participants/visualization/parser_data.hpp>

#include <fastddsspy_yaml/YamlWriter.hpp>

using namespace eprosima;
using namespace eprosima::ddspipe::yaml;
using namespace eprosima::spy::participants;

/**
 * Convert a SimpleParticipantData to yaml
 */
TEST(YamlWriterTest, test_SimpleParticipantData)
{
    ddspipe::core::types::Guid guid = ddspipe::core::types::Guid::new_unique_guid();
    SimpleParticipantData data {"Name", guid};

    // Set yaml using set
    Yaml yml;
    set(yml, data);

    // Set yaml using Yaml functions
    Yaml yml_expected;
    yml_expected["name"] = "Name";
    Yaml yml_guid;
    yml_expected["guid"] = utils::generic_to_string(guid);

    // Check they are the same
    ASSERT_EQ(
        utils::generic_to_string(yml),
        utils::generic_to_string(yml_expected)
        );
}

/**
 * Convert a ComplexParticipantData to yaml
 */
TEST(YamlWriterTest, test_ComplexParticipantData)
{
    ddspipe::core::types::Guid guid = ddspipe::core::types::Guid::new_unique_guid();
    ComplexParticipantData data;
    data.name = "Name";
    data.guid = guid;
    data.readers = {};
    data.writers = {{"TopicName", "TopicType", 1}, {"TopicName2", "TopicType", 2}, {"TopicName3", "TopicType2", 1}};

    // Set yaml using set
    Yaml yml;
    set(yml, data);

    // Set yaml using Yaml functions
    Yaml yml_expected;
    yml_expected["name"] = "Name";
    Yaml yml_guid;
    yml_expected["guid"] = utils::generic_to_string(guid);
    yml_expected["datawriters"].push_back("TopicName [TopicType] (1)");
    yml_expected["datawriters"].push_back("TopicName2 [TopicType] (2)");
    yml_expected["datawriters"].push_back("TopicName3 [TopicType2] (1)");
    yml_expected["datareaders"];

    // Check they are the same
    ASSERT_EQ(
        utils::generic_to_string(yml),
        utils::generic_to_string(yml_expected)
        );
}

/**
 * Convert a ComplexParticipantData::Endpoint to yaml
 */
TEST(YamlWriterTest, test_ComplexParticipantData_Endpoint)
{
    ComplexParticipantData::Endpoint data {"TopicName", "TopicType", 42};

    // Set yaml using set
    Yaml yml;
    set(yml, data);

    // Set yaml using Yaml functions
    Yaml yml_expected;
    yml_expected = "TopicName [TopicType] (42)";

    // Check they are the same
    ASSERT_EQ(
        utils::generic_to_string(yml),
        utils::generic_to_string(yml_expected)
        );
}

/**
 * Convert a SimpleEndpointData to yaml
 */
TEST(YamlWriterTest, test_SimpleEndpointData)
{
    ddspipe::core::types::Guid guid = ddspipe::core::types::Guid::new_unique_guid();
    SimpleEndpointData data;
    data.guid = guid;
    data.participant_name = "Name";
    data.topic = {"topic_name", "topic_type"};

    // Set yaml using set
    Yaml yml;
    set(yml, data);

    // Set yaml using Yaml functions
    Yaml yml_expected;
    yml_expected["guid"] = utils::generic_to_string(guid);
    yml_expected["participant"] = "Name";
    yml_expected["topic"] = "topic_name [topic_type]";

    // Check they are the same
    ASSERT_EQ(
        utils::generic_to_string(yml),
        utils::generic_to_string(yml_expected)
        );
}

/**
 * Convert a ComplexEndpointData to yaml
 */
TEST(YamlWriterTest, test_ComplexEndpointData)
{
    ddspipe::core::types::Guid guid = ddspipe::core::types::Guid::new_unique_guid();
    ComplexEndpointData data;
    data.guid = guid;
    data.participant_name = "Name";
    data.topic = {"topic_name", "topic_type", "partition"};
    data.qos = {};

    // Set yaml using set
    Yaml yml;
    set(yml, data);

    // Set yaml using Yaml functions
    Yaml yml_expected;
    yml_expected["guid"] = utils::generic_to_string(guid);
    yml_expected["participant"] = "Name";
    // topic as a map
    yml_expected["topic"]["name"] = "topic_name";
    yml_expected["topic"]["type"] = "topic_type";
    yml_expected["topic"]["partitions"] = "partition";

    // include qos defaults (matches actual output)
    yml_expected["qos"]["durability"] = "volatile";
    yml_expected["qos"]["reliability"] = "reliable";

    // Check they are the same
    ASSERT_EQ(
        utils::generic_to_string(yml),
        utils::generic_to_string(yml_expected)
        );
}

/**
 * Convert a SimpleTopicData to yaml (is_compact = false)
 */
TEST(YamlWriterTest, test_SimpleTopicData_compact_false)
{
    SimpleTopicData data;

    data.name = "Name";
    data.type = "Type";
    data.datawriters = 1;
    data.datareaders = 0;
    data.rate = {15, "Hz"};

    // Set yaml using set
    Yaml yml;
    set(yml, data, false);

    // Set yaml using Yaml functions
    Yaml yml_expected;
    yml_expected["name"] = "Name";
    yml_expected["type"] = "Type";
    yml_expected["datawriters"] = "1";
    yml_expected["datareaders"] = "0";
    yml_expected["rate"] = "15 Hz";

    // Check they are the same
    ASSERT_EQ(
        utils::generic_to_string(yml),
        utils::generic_to_string(yml_expected)
        );
}

/**
 * Convert a SimpleTopicData to yaml (is_compact = true)
 */
TEST(YamlWriterTest, test_SimpleTopicData_compact_true)
{
    SimpleTopicData data;

    data.name = "Name";
    data.type = "Type";
    data.datawriters = 1;
    data.datareaders = 0;
    data.rate = {15, "Hz"};

    // Set yaml using set
    Yaml yml;
    set(yml, data, true);

    // Set yaml using Yaml functions
    Yaml yml_expected;
    yml_expected["topic"] = "Name (Type) (1|0) [15.000000 Hz]";

    // Check they are the same
    ASSERT_EQ(
        utils::generic_to_string(yml),
        utils::generic_to_string(yml_expected)
        );
}

/**
 * Convert a ComplexTopicData to yaml
 */
TEST(YamlWriterTest, test_ComplexTopicData)
{
    ComplexTopicData data;
    ddspipe::core::types::Guid guid = ddspipe::core::types::Guid::new_unique_guid();

    std::ostringstream ss;
    ss << guid;
    std::string guid_str = ss.str();

    data.name = "Name";
    data.type = "Type";
    data.datawriters = {{guid, "partition"}};
    data.datareaders = {};
    data.rate = {15, "Hz"};
    data.discovered = true;

    // Set yaml using set
    Yaml yml;
    set(yml, data);

    // Set yaml using Yaml functions
    Yaml yml_expected;
    yml_expected["name"] = "Name";
    yml_expected["type"] = "Type";
    yml_expected["datawriters"].push_back(guid_str + " [partition]");
    yml_expected["datareaders"];
    yml_expected["rate"] = "15 Hz";
    yml_expected["dynamic_type_discovered"] = true;

    // Check they are the same
    ASSERT_EQ(
        utils::generic_to_string(yml),
        utils::generic_to_string(yml_expected)
        );
}

/**
 * Convert a DdsDataData to yaml
 */
TEST(YamlWriterTest, test_DdsDataData)
{
    DdsDataData data;
    ddspipe::core::types::Guid guid_writer = ddspipe::core::types::Guid::new_unique_guid();

    std::ostringstream ss_writer;
    ss_writer << guid_writer;
    std::string guid_str = ss_writer.str();

    data.topic.topic_name = "Name";
    data.topic.topic_type = "Type";
    data.writer = guid_writer;
    data.partitions = "partition";

    // Set yaml using set
    Yaml yml;
    set(yml, data);

    // Set yaml using Yaml functions
    Yaml yml_expected;
    yml_expected["topic"] = "Name [Type]";
    yml_expected["writer"] = guid_str;
    yml_expected["partitions"] = "partition";
    yml_expected["timestamp"] = "1970/01/01 00:00:00"; // empty

    ASSERT_EQ(utils::generic_to_string(yml),
            utils::generic_to_string(yml_expected));

    // Check they are the same
    ASSERT_EQ(
        utils::generic_to_string(yml),
        utils::generic_to_string(yml_expected)
        );
}

int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
