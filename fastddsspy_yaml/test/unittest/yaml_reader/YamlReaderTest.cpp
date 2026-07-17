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
 * Test load a non-existing Spy configuration.
 */
TEST(YamlReaderTest, raise_exception_reading_non_existing_configuration)
{
    std::string yml_path = "./some_path/some_file.yaml";

    // Load configuration
    ASSERT_THROW(eprosima::spy::yaml::Configuration configuration(yml_path), eprosima::utils::ConfigurationException);
}

/**
 * Test that a Spy configuration with invalid values raises an exception.
 */
TEST(YamlReaderTest, parse_parameter_overflow_triggers_exception)
{
    // Some parameters can raise an exception when parsed
    const char* yml_str =
            R"(
            specs:
                discovery-time: 9999999999
        )";

    Yaml yml = YAML::Load(yml_str);

    try
    {
        eprosima::spy::yaml::Configuration configuration(yml);
        FAIL() << "Expected eprosima::utils::ConfigurationException";
    }
    catch (const eprosima::utils::ConfigurationException& e)
    {
        EXPECT_NE(std::string(e.what()).find("Error loading DDS Router configuration from yaml"),
                std::string::npos) << "Actual message: " << e.what();
    }
    catch (const std::exception& e)
    {
        FAIL() << "Expected ConfigurationException, got: " << e.what();
    }
}

/**
 * Test load a whole Spy Configuration from yaml node.
 */
TEST(YamlReaderTest, get_spy_configuration_trivial)
{
    const char* yml_str =
            R"(
            dds:
                ros2-types: true
                ros2-easy-mode: '127.0.0.1'
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
    ASSERT_EQ(configuration.dds_configuration->id, "SimpleParticipant");
    ASSERT_EQ(configuration.dds_configuration->app_id, "FASTDDS_SPY");
    ASSERT_EQ(configuration.dds_configuration->app_metadata, "");
    ASSERT_FALSE(configuration.dds_configuration->is_repeater);

    ASSERT_EQ(configuration.spy_configuration->id, "Fast-Spy-007");
    ASSERT_EQ(configuration.spy_configuration->app_id, "FASTDDS_SPY");
    ASSERT_EQ(configuration.spy_configuration->app_metadata, "");
    ASSERT_FALSE(configuration.spy_configuration->is_repeater);

    ASSERT_EQ(configuration.dds_configuration->easy_mode_ip, "127.0.0.1");
}

/**
 * Test that a duplicated manual topic takes the filter value of the last appearance.
 */
TEST(YamlReaderTest, duplicate_manual_topic_name_overwrites_filter)
{
    const char* yml_str =
            R"(
            dds:
                topics:
                    - name: "topic_a"
                      filter: "first_filter"
                    - name: "topic_a"
                      filter: "second_filter"
        )";

    Yaml yml = YAML::Load(yml_str);
    eprosima::spy::yaml::Configuration configuration(yml);

    ASSERT_EQ(configuration.dds_configuration->content_topic_filter_dict.size(), 1u);
    EXPECT_EQ(configuration.dds_configuration->content_topic_filter_dict.at("topic_a"), "second_filter");
}

/**
 * Test that the method "is_valid" fails with null dds configuration.
 */
TEST(YamlReaderTest, is_valid_fails_when_dds_configuration_null)
{
    eprosima::spy::yaml::Configuration configuration;
    configuration.dds_configuration.reset();

    utils::Formatter error_msg;
    ASSERT_FALSE(configuration.is_valid(error_msg));
    EXPECT_NE(error_msg.to_string().find("DDS participant configuration is not initialized"),
            std::string::npos) << "Actual message: " << error_msg.to_string();
}

/**
 * Test that the method "is_valid" fails with a domain ID out of range.
 */
TEST(YamlReaderTest, is_valid_fails_when_domain_out_of_range)
{
    eprosima::spy::yaml::Configuration configuration;
    configuration.dds_configuration->domain.domain_id =
            eprosima::ddspipe::core::types::DomainId::MAX_DOMAIN_ID + 1;

    utils::Formatter error_msg;
    ASSERT_FALSE(configuration.is_valid(error_msg));
    EXPECT_NE(error_msg.to_string().find("Domain ID must be between 0 and"),
            std::string::npos) << "Actual message: " << error_msg.to_string();
}

int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
