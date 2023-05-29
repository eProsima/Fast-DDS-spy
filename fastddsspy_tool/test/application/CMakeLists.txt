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

###########################################################################
# Create tests                                                            #
###########################################################################

find_package(PythonInterp 3 REQUIRED)

# Name of files to test
set(TEST_LIST
    one_shot__config
    one_shot__config_fail_file
    one_shot__config_fail_type
    one_shot__config_fail_empty_arg
    one_shot__log_filter
    one_shot__log_filter_fail
    one_shot__log_verb_fail
    one_shot__log_verb_info
    one_shot__log_verb_warning
    one_shot__log_verb_error
    one_shot__reload_time
    one_shot__reload_time_fail
    one_shot__debug
    one_shot_help
    one_shot__help
    one_shot_null
    one_shot__null
    one_shot__version
    one_shot_datareader
    one_shot_datareader_verbose
    one_shot_datawriter
    one_shot_datawriter_verbose
    one_shot_participants
    one_shot_participants_verbose
    one_shot_quit
    one_shot_show_all
    one_shot_show_fail
    one_shot_show_topic
    one_shot_show_topic_verbose
    one_shot_topics
    one_shot_topics_name
    one_shot_topics_verbose
    tool_datareader
    tool_datawriter
    tool_help
    tool_null
    tool_participants
    tool_show_all
    tool_show_topic
    tool_topics
    tool_version
    one_shot_datareader_dds
    one_shot_datareader_guid_dds
    one_shot_datareader_verbose_dds
    one_shot_datawriter_dds
    one_shot_datawriter_verbose_dds_qos
    one_shot_datawriter_guid_dds_fail
    one_shot_datawriter_verbose_dds
    one_shot_help_dds
    one_shot_participants_dds
    one_shot_participants_guid_dds
    one_shot_participants_verbose_dds
    one_shot_quit_dds
    one_shot_topics_dds
    one_shot_topics_name_dds
    one_shot_topics_verbose_dds
    tool_datareader_dds
    tool_datawriter_dds
    tool_help_dds
    tool_participants_dds
    tool_show_topic_dds
    tool_topics_dds
)

# windows auxiliary script to fork test execution
set(PWS_LAUNCHER
    ${CMAKE_CURRENT_SOURCE_DIR}/launcher.ps1
)

# Calculate environment
set(TEST_ENVIRONMENT
    "PATH=$ENV{PATH};$<TARGET_FILE_DIR:${PROJECT_NAME}>")

foreach(PROJECT_DEPENDENCY ${MODULE_DEPENDENCIES})

    message(STATUS "Finding target for libraries for ${PROJECT_DEPENDENCY}")

    if(TARGET ${PROJECT_DEPENDENCY} OR TARGET eprosima::${PROJECT_DEPENDENCY} OR TARGET ${PROJECT_DEPENDENCY}::${PROJECT_DEPENDENCY})
        set(TEST_ENVIRONMENT
            "${TEST_ENVIRONMENT};$<TARGET_FILE_DIR:${PROJECT_DEPENDENCY}>")

    elseif(EXISTS ${PROJECT_DEPENDENCY}_LIBRARY)
        get_filename_component(
            ${PROJECT_DEPENDENCY}_LIBRARY_DIR ${${PROJECT_DEPENDENCY}_LIBRARY} DIRECTORY)
        set(TEST_ENVIRONMENT
            "${TEST_ENVIRONMENT};${${PROJECT_DEPENDENCY}_LIBRARY_DIR}")
        unset(${PROJECT_DEPENDENCY}_LIBRARY_DIR)

    else()

        message(STATUS "${PROJECT_DEPENDENCY} could not be added to TEST_ENVIRONMENT")

    endif()

endforeach()

if(WIN32)

    if(TARGET tinyxml2 OR TARGET tinyxml2::tinyxml2)
        set(TEST_ENVIRONMENT
            "${TEST_ENVIRONMENT};$<TARGET_FILE_DIR:${TINYXML2_LIBRARY}>")
    elseif(EXISTS TINYXML2_LIBRARY)
        get_filename_component(
            TINYXML2_LIBRARY_DIR ${TINYXML2_LIBRARY} DIRECTORY)
        set(TEST_ENVIRONMENT
            "${TEST_ENVIRONMENT};${TINYXML2_LIBRARY_DIR}")
        unset(TINYXML2_LIBRARY_DIR)
    endif()

    if(TARGET yamlcpp OR TARGET yamlcpp::yamlcpp)
        set(TEST_ENVIRONMENT
            "${TEST_ENVIRONMENT};$<TARGET_FILE_DIR:${YAMLCPP_LIBRARY}>")
    elseif(EXISTS YAMLCPP_LIBRARY)
        get_filename_component(
            YAMLCPP_LIBRARY_DIR ${YAMLCPP_LIBRARY} DIRECTORY)
        set(TEST_ENVIRONMENT
            "${TEST_ENVIRONMENT};${YAMLCPP_LIBRARY_DIR}")
        unset(YAMLCPP_LIBRARY_DIR)
    endif()

    string(REPLACE ";" "\\;" TEST_ENVIRONMENT "${TEST_ENVIRONMENT}")

endif(WIN32)

set(TEST_NEEDED_SOURCES
        configuration/configuration_basic.yaml
        configuration/configuration_wrong_empty_arg.yaml
        configuration/configuration_wrong_type.yaml
        configuration/configuration_discovery_time.yaml
    )

foreach(NEEDED_SOURCE ${TEST_NEEDED_SOURCES})
    configure_file(${CMAKE_CURRENT_SOURCE_DIR}/${NEEDED_SOURCE}
        ${CMAKE_CURRENT_BINARY_DIR}/${NEEDED_SOURCE}
        COPYONLY)
endforeach()

# populate the tests
foreach(TEST IN LISTS TEST_LIST)
    set(TEST_NAME "tool.application.fastddsspy.test.${TEST}")

    if(WIN32)
        add_test(
            NAME ${TEST_NAME}
            COMMAND powershell "-File" ${PWS_LAUNCHER}
                    ${PYTHON_EXECUTABLE}
                    ${CMAKE_CURRENT_SOURCE_DIR}/test.py
                    $<TARGET_FILE:fastddsspy_tool>
                    ${TEST}
        )

    else()

        add_test(
                NAME ${TEST_NAME}
                COMMAND ${PYTHON_EXECUTABLE}
                        ${CMAKE_CURRENT_SOURCE_DIR}/test.py
                        "--exe" $<TARGET_FILE:fastddsspy_tool>
                        "--test" ${TEST}
            )

    endif()

    # Add labels to tests
    set_test_labels(${TEST_NAME})

    # Set test properties
    set_tests_properties(
        ${TEST_NAME}
        PROPERTIES
            ENVIRONMENT "${TEST_ENVIRONMENT}"
        )

endforeach()

unset(TEST_ENVIRONMENT)

add_subdirectory(dds/AdvancedConfigurationExample)
