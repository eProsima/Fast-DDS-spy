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

set(TEST_NAME NetworkDatabaseTest)

set(TEST_SOURCES
        NetworkDatabaseTest.cpp
    )
all_library_sources("${TEST_SOURCES}")

set(TEST_LIST
        trivial
    )

set(TEST_EXTRA_LIBRARIES
        fastcdr
        fastrtps
        cpp_utils
        ddspipe_core
        ddspipe_participants
        ddspipe_yaml
    )

set(TEST_NEEDED_SOURCES
    )

set(TEST_EXTRA_HEADERS
    )

add_blackbox_executable(
        "${TEST_NAME}"
        "${TEST_SOURCES}"
        "${TEST_LIST}"
        "${TEST_NEEDED_SOURCES}"
        "${TEST_EXTRA_HEADERS}"
    )
