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

#include <fstream>
#include <iostream>

#include <cpp_utils/testing/gtest_aux.hpp>
#include <gtest/gtest.h>

#include <cpp_utils/exception/ConfigurationException.hpp>

#include <ddspipe_yaml/YamlManager.hpp>
#include <ddspipe_yaml/YamlValidator.hpp>

#include <fastddsspy_yaml/YamlReaderConfiguration.hpp>

using namespace eprosima;
using namespace eprosima::ddspipe::yaml;
using namespace eprosima::spy::yaml;

namespace test {
// Paths and files for the tests
std::string schema_path = "./fastddsspy_config_schema.json";

// Vectors with the valid and invalid YAML files for the recorder
std::vector<std::string> valid_files = {
    "./valid_config_files/complete_config_test.yaml",
    "./valid_config_files/dds_topics_filtering_config_test.yaml",
    "./valid_config_files/docu_example.yaml",
    "./valid_config_files/logging_config_test.yaml",
    "./valid_config_files/manual_topic_config_test.yaml",
    "./valid_config_files/minimal_config_test.yaml",
    // Config files under Fast-DDS-spy/resources
    "./valid_config_files/fastddsspy_configuration.yaml",
};
std::vector<std::string> invalid_files = {
    "./invalid_config_files/allowlist_topic_no_name.yaml",
    "./invalid_config_files/invalid_domain.yaml",
    "./invalid_config_files/invalid_ignore_participant_flags.yaml",
    "./invalid_config_files/invalid_qos_downsampling.yaml",
    "./invalid_config_files/invalid_threads.yaml",
    "./invalid_config_files/invalid_transport.yaml",
    "./invalid_config_files/invalid_verbosity.yaml",
    "./invalid_config_files/log_publish_no_enable.yaml",
    "./invalid_config_files/topics_manual_topic_no_name.yaml",
    "./invalid_config_files/unknown_property.yaml",
    "./invalid_config_files/xml_no_raw_nor_files.yaml",
};

} // namespace test

/**
 * Test directly that a set of valid YAML configurations pass the validation
 */
TEST(YamlValidatorDdsSpyTest, spy_direct_validation_passed)
{
    YamlValidator validator;
    validator.set_schema(YamlValidator::InputType::FROM_FILE, test::schema_path);

    // valid files
    {
        for (std::string st : test::valid_files)
        {
            Yaml yml = YamlManager::load_file(st);
            EXPECT_TRUE(validator.validate_YAML(yml)) << "Failed for file: " << st;
        }
    }
}

/**
 * Test directly that a set of invalid YAML configurations don't pass the validation
 */
TEST(YamlValidatorDdsSpyTest, spy_direct_validation_failed)
{
    YamlValidator validator;
    validator.set_schema(YamlValidator::InputType::FROM_FILE, test::schema_path);

    // invalid files
    {
        for (std::string st : test::invalid_files)
        {
            Yaml yml = YamlManager::load_file(st);
            // Validate is called with false to prevent filling the output with the specific errors
            EXPECT_FALSE(validator.validate_YAML(yml, false)) << "Failed for file: " << st;
        }
    }
}

/**
 * Test using the YamlReader that a set of valid YAML configurations pass the validation
 */
TEST(YamlValidatorDdsSpyTest, spy_reader_validation_passed)
{
    // valid files
    {
        for (std::string st : test::valid_files)
        {
            EXPECT_NO_THROW(Configuration config = Configuration(st); )
                << "Failed for file: " << st;
        }
    }
}

/**
 * Test using the YamlReader that a set of invalid YAML configurations don't pass the validation
 */
TEST(YamlValidatorDdsSpyTest, spy_reader_validation_failed)
{
    // invalid files
    {
        for (std::string st : test::invalid_files)
        {
            try
            {
                Configuration config = Configuration(st);
                FAIL() << "Expected eprosima::utils::ConfigurationException for file:\n'" << st << "'\n";
            }
            catch (const eprosima::utils::ConfigurationException& e)
            {
                EXPECT_NE(std::string(e.what()).find("is not a valid dds-spy configuration"), std::string::npos)
                    << "Failed for file\n'" << st << "'\nActual message: " << e.what();
            }
            catch (const std::exception& e)
            {
                FAIL() << "Expected eprosima::utils::ConfigurationException but "
                       << "caught a different exception for file:\n'"
                       << st << "'\n"
                       << "Actual message: " << e.what();
            }
        }
    }
}

int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
