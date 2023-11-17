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

#include <fastddsspy_yaml/YamlReaderConfiguration.hpp>

using namespace eprosima;
using namespace eprosima::ddspipe::yaml;
using namespace eprosima::spy::participants;

/**
 * Test load a whole Spy Configuration from yaml node.
 */
TEST(YamlReaderTest, get_spy_configuration_trivial)
{
    const char* yml_str =
            R"(
            version: v4.0
            dds:
                ros2-types: true
        )";

    Yaml yml = YAML::Load(yml_str);

    // Load configuration
    eprosima::spy::yaml::Configuration configuration(yml);

    // Check is valid
    utils::Formatter error_msg;
    ASSERT_TRUE(configuration.is_valid(error_msg));

    // Check yaml specified data
    ASSERT_EQ(configuration.ros2_types, true);

    // Check app data
    ASSERT_EQ(configuration.simple_configuration->id, "SimpleParticipant");
    ASSERT_EQ(configuration.simple_configuration->app_id, "FASTDDS_SPY");
    ASSERT_EQ(configuration.simple_configuration->app_metadata, "");
    ASSERT_FALSE(configuration.simple_configuration->is_repeater);

    ASSERT_EQ(configuration.spy_configuration->id, "Fast-Spy-007");
    ASSERT_EQ(configuration.spy_configuration->app_id, "FASTDDS_SPY");
    ASSERT_EQ(configuration.spy_configuration->app_metadata, "");
    ASSERT_FALSE(configuration.spy_configuration->is_repeater);
}

int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
