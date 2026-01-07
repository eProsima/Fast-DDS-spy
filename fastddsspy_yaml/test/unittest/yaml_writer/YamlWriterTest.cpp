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
#include <nlohmann/json.hpp>

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

// struct TopicKeysData
// {
//     std::string topic_name;
//     std::vector<std::string> key_fields;
//     std::vector<std::string> instances;
//     size_t instance_count;
// };

/**
 * Convert a TopicKeysData to yaml
 */
TEST(YamlWriterTest, test_TopicKeysData_compact_true)
{
    TopicKeysData data;

    data.topic_name = "Name";
    data.key_fields = {"Key1", "Key2"};
    data.instances = {
        R"({"Key1":"Value1", "Key2":"Value2"})",
        R"({"Key1":"Value1", "Key2":"Value3"})"
    };
    data.instance_count = 1;

    // Set yaml using set
    Yaml yml;
    set(yml, data, true);

    // Set yaml using Yaml functions
    Yaml yml_expected;
    yml_expected["topic"] = "Name";
    yml_expected["keys"][0] = "Key1";
    yml_expected["keys"][1] = "Key2";
    yml_expected["instance_count"] = 1;

    // Check they are the same
    ASSERT_EQ(
        utils::generic_to_string(yml),
        utils::generic_to_string(yml_expected)
        );
}

/**
 * Convert a TopicKeysData to yaml
 */
TEST(YamlWriterTest, test_TopicKeysData_compact_false)
{
    TopicKeysData data;

    data.topic_name = "Name";
    data.key_fields = {"Key1", "Key2"};
    data.instances = {
        R"({"Key1":"Value1", "Key2":"Value2"})",
        R"({"Key1":"Value1", "Key2":"Value3"})"
    };
    data.instance_count = 1;

    // Set yaml using set
    Yaml yml;
    set(yml, data, false);

    // Set yaml using Yaml functions
    Yaml yml_expected;
    yml_expected["topic"] = "Name";
    yml_expected["keys"][0] = "Key1";
    yml_expected["keys"][1] = "Key2";
    yml_expected["instances"][0]["Key1"] = "Value1";
    yml_expected["instances"][0]["Key2"] = "Value2";
    yml_expected["instances"][1]["Key1"] = "Value1";
    yml_expected["instances"][1]["Key2"] = "Value3";
    yml_expected["instance_count"] = 1;

    // Check they are the same
    ASSERT_EQ(
        utils::generic_to_string(yml),
        utils::generic_to_string(yml_expected)
        );
}

/**
 * Test TopicKeysData with comprehensive JSON structure covering all json_to_yaml paths
 */
TEST(YamlWriterTest, test_TopicKeysData_json_comprehensive)
{
    TopicKeysData data;
    data.topic_name = "ComprehensiveTest";
    data.key_fields = {"data"};
    data.instances = {
        R"({
            "data": {
                "null_value": null,
                "boolean_true": true,
                "boolean_false": false,
                "integer_positive": 42,
                "integer_negative": -123,
                "integer_zero": 0,
                "unsigned_value": 4294967295,
                "float_positive": 3.14159,
                "float_negative": -2.71828,
                "float_scientific": 1.23e-10,
                "string_simple": "Hello World",
                "string_empty": "",
                "string_special": "With \"quotes\" and \n newlines",
                "primitive_array_int": [1, 2, 3, 4, 5],
                "primitive_array_string": ["a", "b", "c"],
                "primitive_array_bool": [true, false, true],
                "primitive_array_mixed": [1, "text", 3.14, true, null],
                "empty_array": [],
                "object_array": [
                    {"id": 1, "name": "first"},
                    {"id": 2, "name": "second"}
                ],
                "nested_object": {
                    "level1": {
                        "level2": {
                            "value": "deep"
                        }
                    }
                },
                "nested_array": [[1, 2], [3, 4], [5, 6]],
                "empty_object": {},
                "object_with_empty": {
                    "empty_nested": {}
                }
            }
        })"
    };
    data.instance_count = 1;

    Yaml yml;
    set(yml, data, false);

    ASSERT_EQ(yml["instances"].size(), 1);
    const auto& instance = yml["instances"][0]["data"];
    
    // Test null (is_null path)
    ASSERT_TRUE(instance["null_value"].IsNull());
    
    // Test booleans (is_boolean path)
    ASSERT_TRUE(instance["boolean_true"].as<bool>());
    ASSERT_FALSE(instance["boolean_false"].as<bool>());
    
    // Test integers (is_number_integer path)
    ASSERT_EQ(instance["integer_positive"].as<int>(), 42);
    ASSERT_EQ(instance["integer_negative"].as<int>(), -123);
    ASSERT_EQ(instance["integer_zero"].as<int>(), 0);
    
    // Test unsigned (is_number_unsigned path)
    // Use a value that fits in uint32_t to avoid conversion issues
    ASSERT_EQ(instance["unsigned_value"].as<unsigned int>(), 4294967295u);
    
    // Test floats (is_number_float path)
    ASSERT_DOUBLE_EQ(instance["float_positive"].as<double>(), 3.14159);
    ASSERT_DOUBLE_EQ(instance["float_negative"].as<double>(), -2.71828);
    ASSERT_NEAR(instance["float_scientific"].as<double>(), 1.23e-10, 1e-15);
    
    // Test strings (is_string path)
    ASSERT_EQ(instance["string_simple"].as<std::string>(), "Hello World");
    ASSERT_EQ(instance["string_empty"].as<std::string>(), "");
    ASSERT_EQ(instance["string_special"].as<std::string>(), "With \"quotes\" and \n newlines");
    
    // Test primitive arrays (is_array + is_primitive_array=true, flow style)
    ASSERT_TRUE(instance["primitive_array_int"].IsSequence());
    ASSERT_EQ(instance["primitive_array_int"].size(), 5);
    ASSERT_EQ(instance["primitive_array_int"][0].as<int>(), 1);
    ASSERT_EQ(instance["primitive_array_int"][4].as<int>(), 5);
    ASSERT_EQ(instance["primitive_array_int"].Style(), YAML::EmitterStyle::Flow);
    
    ASSERT_TRUE(instance["primitive_array_string"].IsSequence());
    ASSERT_EQ(instance["primitive_array_string"][0].as<std::string>(), "a");
    ASSERT_EQ(instance["primitive_array_string"].Style(), YAML::EmitterStyle::Flow);
    
    ASSERT_TRUE(instance["primitive_array_bool"].IsSequence());
    ASSERT_TRUE(instance["primitive_array_bool"][0].as<bool>());
    ASSERT_FALSE(instance["primitive_array_bool"][1].as<bool>());
    ASSERT_EQ(instance["primitive_array_bool"].Style(), YAML::EmitterStyle::Flow);
    
    // Test mixed primitive array (is_array + is_primitive_array=true, flow style)
    ASSERT_TRUE(instance["primitive_array_mixed"].IsSequence());
    ASSERT_EQ(instance["primitive_array_mixed"].size(), 5);
    ASSERT_EQ(instance["primitive_array_mixed"][0].as<int>(), 1);
    ASSERT_EQ(instance["primitive_array_mixed"][1].as<std::string>(), "text");
    ASSERT_DOUBLE_EQ(instance["primitive_array_mixed"][2].as<double>(), 3.14);
    ASSERT_TRUE(instance["primitive_array_mixed"][3].as<bool>());
    ASSERT_TRUE(instance["primitive_array_mixed"][4].IsNull());
    ASSERT_EQ(instance["primitive_array_mixed"].Style(), YAML::EmitterStyle::Flow);
    
    // Test empty array (is_array + is_primitive_array=false due to empty, block style)
    ASSERT_TRUE(instance["empty_array"].IsSequence());
    ASSERT_EQ(instance["empty_array"].size(), 0);
    // Empty arrays go through block style path (is_primitive_array returns false for empty)
    
    // Test object array (is_array + is_primitive_array=false, block style)
    ASSERT_TRUE(instance["object_array"].IsSequence());
    ASSERT_EQ(instance["object_array"].size(), 2);
    ASSERT_EQ(instance["object_array"][0]["id"].as<int>(), 1);
    ASSERT_EQ(instance["object_array"][0]["name"].as<std::string>(), "first");
    ASSERT_EQ(instance["object_array"][1]["id"].as<int>(), 2);
    ASSERT_NE(instance["object_array"].Style(), YAML::EmitterStyle::Flow);
    
    // Test nested objects (is_object path)
    ASSERT_TRUE(instance["nested_object"].IsMap());
    ASSERT_TRUE(instance["nested_object"]["level1"].IsMap());
    ASSERT_TRUE(instance["nested_object"]["level1"]["level2"].IsMap());
    ASSERT_EQ(instance["nested_object"]["level1"]["level2"]["value"].as<std::string>(), "deep");
    
    // Test nested arrays (is_array + is_primitive_array=false, contains arrays not primitives)
    ASSERT_TRUE(instance["nested_array"].IsSequence());
    ASSERT_EQ(instance["nested_array"].size(), 3);
    ASSERT_TRUE(instance["nested_array"][0].IsSequence());
    ASSERT_EQ(instance["nested_array"][0].size(), 2);
    ASSERT_EQ(instance["nested_array"][0][0].as<int>(), 1);
    ASSERT_EQ(instance["nested_array"][2][1].as<int>(), 6);
    // Inner arrays are primitive arrays, so they use flow style
    ASSERT_EQ(instance["nested_array"][0].Style(), YAML::EmitterStyle::Flow);
    
    // Test empty object (is_object path with empty)
    ASSERT_TRUE(instance["empty_object"].IsMap());
    ASSERT_EQ(instance["empty_object"].size(), 0);
    
    // Test object containing empty object (is_object path)
    ASSERT_TRUE(instance["object_with_empty"].IsMap());
    ASSERT_TRUE(instance["object_with_empty"]["empty_nested"].IsMap());
    ASSERT_EQ(instance["object_with_empty"]["empty_nested"].size(), 0);
}

int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
