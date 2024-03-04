// Copyright 2024 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

#include <fastddsspy_yaml/CommandlineArgsSpy.hpp>
#include <fastddsspy_yaml/YamlReaderConfiguration.hpp>

using namespace eprosima;

/**
 * Check DdsSpyConfiguration structure creation.
 *
 * CASES:
 *  Check if chooses correctly log configuration when parsing from terminal and from YAML.
 *  In this case, it checks that:
 *    - The error filter is the one configured through the YAML
 *    - The warning filter is the one configured through the Command-Line
 *    - The info filter is the default (FASTDDSSPY)
 */
TEST(YamlGetConfigurationDdsSpyTest, get_ddsspy_configuration_yaml_vs_commandline)
{
    spy::yaml::CommandlineArgsSpy commandline_args;

    // Setting CommandLine arguments as if configured from CommandLine
    commandline_args.log_filter[utils::VerbosityKind::Warning].set_value("FASTDDSSPY|DDSPIPE|DEBUG");

    const char* yml_str =
            R"(
            specs:
              logging:
                verbosity: info
                filter:
                  error: "DEBUG"
                  warning: "FASTDDSSPY"
        )";

    Yaml yml = YAML::Load(yml_str);

    // Load configuration from YAML
    spy::yaml::Configuration configuration(yml, &commandline_args);

    utils::Formatter error_msg;

    ASSERT_TRUE(configuration.ddspipe_configuration.log_configuration.is_valid(error_msg));
    ASSERT_EQ(configuration.ddspipe_configuration.log_configuration.verbosity.get_value(), utils::VerbosityKind::Info);
    ASSERT_EQ(
        configuration.ddspipe_configuration.log_configuration.filter[utils::VerbosityKind::Error].get_value(),
        "DEBUG");
    ASSERT_EQ(
        configuration.ddspipe_configuration.log_configuration.filter[utils::VerbosityKind::Warning].get_value(),
        "FASTDDSSPY|DDSPIPE|DEBUG");
    ASSERT_EQ(
        configuration.ddspipe_configuration.log_configuration.filter[utils::VerbosityKind::Info].get_value(),
        "FASTDDSSPY");
}

int main(
        int argc,
        char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
