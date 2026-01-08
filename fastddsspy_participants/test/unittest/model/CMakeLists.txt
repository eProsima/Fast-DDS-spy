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

#########################################
# Fast DDS Spy Data Streamer tests
#########################################

set(TEST_NAME DataStreamerTest)

set(TEST_SOURCES
        DataStreamerTest.cpp
    )
all_library_sources("${TEST_SOURCES}")

set(TEST_LIST
        topic_type_discovered
        deactivate
        add_data
        add_data_two_topics
    )

set(TEST_EXTRA_LIBRARIES
        fastcdr
        fastdds
        cpp_utils
        ddspipe_core
        ddspipe_participants
    )

add_unittest_executable(
        "${TEST_NAME}"
        "${TEST_SOURCES}"
        "${TEST_LIST}"
        "${TEST_EXTRA_LIBRARIES}"
    )

#########################################
# Fast DDS Spy Instance Cache tests
#########################################

set(TEST_NAME InstanceCacheTest)

set(TEST_SOURCES
        InstanceCacheTest.cpp
    )
all_library_sources("${TEST_SOURCES}")

set(TEST_LIST
        add_single_instance
        add_multiple_instances
        update_existing_instance
        multiple_writers_same_instance
        writer_removal
        multiple_key_fields
        type_without_keys
        clear_cache
        non_existent_topic
        writer_removal_non_existent_topic
        multiple_topics
        serialization_failure
        max_instances_limit
        multiple_writers_all_removed
        readd_after_removal
        writer_activation
    )

set(TEST_EXTRA_LIBRARIES
        fastcdr
        fastdds
        cpp_utils
        ddspipe_core
        ddspipe_participants
    )

add_unittest_executable(
        "${TEST_NAME}"
        "${TEST_SOURCES}"
        "${TEST_LIST}"
        "${TEST_EXTRA_LIBRARIES}"
    )