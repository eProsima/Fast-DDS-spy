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

function(GET_FILENAMES_WITHOUT_PARENT PATH_LIST RESULT_VARIABLE)
    set(FILENAMES "")

    foreach(PATH ${${PATH_LIST}})
        get_filename_component(FILENAME ${PATH} NAME)
        string(REGEX REPLACE "\\.[^.]*$" "" fileNameWithoutExtension ${FILENAME})
        list(APPEND FILENAMES ${fileNameWithoutExtension})
    endforeach()

    set(${RESULT_VARIABLE} ${FILENAMES} PARENT_SCOPE)
endfunction()

file(
    GLOB_RECURSE TEST_PATH
    "test_cases/*.py"
)

GET_FILENAMES_WITHOUT_PARENT(TEST_PATH TEST_FILENAMES)

message(STATUS "TEST FILES NAME: ${TEST_FILENAMES}")

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

# compile AdvanceConfigurationExample needed for tests
add_subdirectory(dds/AdvancedConfigurationExample)

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
foreach(TEST IN LISTS TEST_FILENAMES)
    set(TEST_NAME "tool.application.fastddsspy.test.test_cases.${TEST}")

    if(WIN32)
        add_test(
            NAME ${TEST_NAME}
            COMMAND powershell "-File" ${PWS_LAUNCHER}
                    ${PYTHON_EXECUTABLE}
                    ${CMAKE_CURRENT_SOURCE_DIR}/test.py
                    $<TARGET_FILE:fastddsspy_tool>
                    $<TARGET_FILE:AdvancedConfigurationExample>
                    ${TEST}
        )

    else()

        add_test(
                NAME ${TEST_NAME}
                COMMAND ${PYTHON_EXECUTABLE}
                        ${CMAKE_CURRENT_SOURCE_DIR}/test.py
                        "--exe" $<TARGET_FILE:fastddsspy_tool>
                        "--pub" $<TARGET_FILE:AdvancedConfigurationExample>
                        "--test" ${TEST}
            )

    endif()

    # Add labels to tests
    # TODO use cmake_utils
    set_test_labels(${TEST_NAME})

    # Set test properties
    set_tests_properties(
        ${TEST_NAME}
        PROPERTIES
            ENVIRONMENT "${TEST_ENVIRONMENT}"
        )

endforeach()

unset(TEST_ENVIRONMENT)
