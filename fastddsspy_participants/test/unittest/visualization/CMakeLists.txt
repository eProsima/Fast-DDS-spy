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

set(TEST_NAME ModelParserTest)

set(TEST_SOURCES
        ModelParserTest.cpp
    )
all_library_sources("${TEST_SOURCES}")

set(TEST_LIST
        simple_participant
        simple_participant_n_participants
        participants_verbose_dds_endpoints
        participants_verbose_ros2_types_dds_endpoints
        participants_verbose_ros2_endpoints
        participants_verbose_ros2_types_ros2_endpoints
        complex_participant_dds_endpoints
        complex_participant_ros2_types_dds_endpoints
        complex_participant_ros2_endpoints
        complex_participant_ros2_types_ros2_endpoints
        simple_dds_endpoint_writer
        simple_dds_endpoint_writer_ros2_types
        simple_ros2_endpoint_writer
        simple_ros2_endpoint_writer_ros2_types
        simple_dds_endpoint_reader
        simple_dds_endpoint_reader_ros2_types
        simple_ros2_endpoint_reader
        simple_ros2_endpoint_reader_ros2_types
        simple_endpoint_writer_readers
        simple_endpoint_reader_writers
        dds_endpoint_reader_verbose
        dds_endpoint_reader_verbose_ros2_types
        ros2_endpoint_reader_verbose
        ros2_endpoint_reader_verbose_ros2_types
        dds_endpoint_writer_verbose
        dds_endpoint_writer_verbose_ros2_types
        ros2_endpoint_writer_verbose
        ros2_endpoint_writer_verbose_ros2_types
        complex_dds_endpoint_writer
        complex_dds_endpoint_writer_ros2_types
        complex_ros2_endpoint_writer
        complex_ros2_endpoint_writer_ros2_types
        complex_dds_endpoint_reader
        complex_dds_endpoint_reader_ros2_types
        complex_ros2_endpoint_reader
        complex_ros2_endpoint_reader_ros2_types
        simple_topic_dds_endpoints
        simple_topic_dds_endpoints_ros2_types
        simple_topic_ros2_endpoints
        simple_topic_ros2_endpoints_ros2_types
        topics_verbose_dds_endpoints
        topics_verbose_dds_endpoints_ros2_types
        topics_verbose_ros2_endpoints
        topics_verbose_ros2_endpoints_ros2_types
        complex_topic_dds_endpoints
        complex_topic_dds_endpoints_ros2_types
        complex_topic_ros2_endpoints
        complex_topic_ros2_endpoints_ros2_types
        topics_keys_type_not_discovered
        topics_keys_single_key_single_instance
        topics_keys_multiple_keys_multiple_instances
        topics_keys_ros2_types
        topics_keys_with_topic_filter
        topics_keys_writer_removal
        topics_keys_no_keys
        simple_dds_endpoint_writer_filtered
        simple_dds_endpoint_writer_ros2_types_filtered
        simple_ros2_endpoint_writer_filtered
        simple_ros2_endpoint_writer_ros2_types_filtered
        simple_dds_endpoint_reader_filtered
        simple_dds_endpoint_reader_ros2_types_filtered
        simple_ros2_endpoint_reader_filtered
        simple_ros2_endpoint_reader_ros2_types_filtered
        dds_endpoint_reader_verbose_filtered
        dds_endpoint_reader_verbose_ros2_types_filtered
        ros2_endpoint_reader_verbose_filtered
        ros2_endpoint_reader_verbose_ros2_types_filtered
        dds_endpoint_writer_verbose_filtered
        dds_endpoint_writer_verbose_ros2_types_filtered
        ros2_endpoint_writer_verbose_filtered
        ros2_endpoint_writer_verbose_ros2_types_filtered
        complex_dds_endpoint_writer_filtered
        complex_dds_endpoint_writer_ros2_types_filtered
        complex_ros2_endpoint_writer_filtered
        complex_ros2_endpoint_writer_ros2_types_filtered
        complex_dds_endpoint_reader_filtered
        complex_dds_endpoint_reader_ros2_types_filtered
        complex_ros2_endpoint_reader_filtered
        complex_ros2_endpoint_reader_ros2_types_filtered
        simple_topic_dds_endpoints_filtered
        simple_topic_dds_endpoints_ros2_types_filtered
        simple_topic_ros2_endpoints_filtered
        simple_topic_ros2_endpoints_ros2_types_filtered
        topics_verbose_dds_endpoints_filtered
        topics_verbose_dds_endpoints_ros2_types_filtered
        topics_verbose_ros2_endpoints_filtered
        topics_verbose_ros2_endpoints_ros2_types_filtered
        complex_topic_dds_endpoints_filtered
        complex_topic_dds_endpoints_ros2_types_filtered
        complex_topic_ros2_endpoints_filtered
        complex_topic_ros2_endpoints_ros2_types_filtered
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
