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

#include <ddspipe_yaml/YamlWriter.hpp>

#include <fastddsspy_participants/visualization/parser_data.hpp>

using namespace eprosima;
using namespace eprosima::ddspipe::yaml;
using namespace eprosima::spy::participants;

/**
 * TODO
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
 * TODO
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
 * TODO
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

int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
