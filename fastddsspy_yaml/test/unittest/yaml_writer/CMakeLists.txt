# Copyright 2023 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

############################
# Yaml Writer Test         #
############################

set(TEST_NAME YamlWriterTest)

set(TEST_SOURCES
        ${PROJECT_SOURCE_DIR}/src/cpp/YamlWriter.cpp
        YamlWriterTest.cpp
    )

set(TEST_LIST
        test_SimpleParticipantData
        test_ComplexParticipantData
        test_ComplexParticipantData_Endpoint
        test_SimpleEndpointData
        test_ComplexEndpointData
        test_SimpleTopicData_compact_false
        test_SimpleTopicData_compact_true
        test_ComplexTopicData
        test_DdsDataData
    )

set(TEST_EXTRA_LIBRARIES
        ${MODULE_DEPENDENCIES}
    )

add_unittest_executable(
    "${TEST_NAME}"
    "${TEST_SOURCES}"
    "${TEST_LIST}"
    "${TEST_EXTRA_LIBRARIES}")
