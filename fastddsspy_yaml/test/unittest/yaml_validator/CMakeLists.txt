# Copyright 2026 Proyectos y Sistemas de Mantenimiento SL (eProsima).
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

###########################################
# Dds Record & Replay Yaml Validator Test #
###########################################

set(TEST_NAME YamlValidatorDdsSpyTest)


set(TEST_SOURCES
        YamlValidatorDdsSpyTest.cpp
    )

set(TEST_LIST
        spy_direct_validation_passed
        spy_direct_validation_failed
        spy_reader_validation_passed
        spy_reader_validation_failed
    )

set(TEST_EXTRA_LIBRARIES
        yaml-cpp
        fastcdr
        fastdds
        cpp_utils
        ddspipe_core
        ddspipe_participants
        ddspipe_yaml
        fastddsspy_yaml
    )

configure_file(
    "${PROJECT_SOURCE_DIR}/../resources/fastddsspy_config_schema.json"
    "${CMAKE_CURRENT_BINARY_DIR}/fastddsspy_config_schema.json"
    COPYONLY
)

file(REMOVE_RECURSE "${CMAKE_CURRENT_BINARY_DIR}/valid_config_files/")
file(REMOVE_RECURSE "${CMAKE_CURRENT_BINARY_DIR}/invalid_config_files/")

file(GLOB_RECURSE TEST_NEEDED_SOURCES
    RELATIVE ${CMAKE_CURRENT_SOURCE_DIR}
    "valid_config_files/*.yaml"
    "invalid_config_files/*.yaml"
)

file(GLOB EXAMPLE_YAMLS_SPY
    "${PROJECT_SOURCE_DIR}/../resources/*.yaml"
)
file(COPY ${EXAMPLE_YAMLS_SPY}
    DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/valid_config_files/"
)


add_unittest_executable(
    "${TEST_NAME}"
    "${TEST_SOURCES}"
    "${TEST_LIST}"
    "${TEST_EXTRA_LIBRARIES}"
    "${TEST_NEEDED_SOURCES}"
)

