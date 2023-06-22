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
"""
Tests for the fastddsspy executable.

Contains a package of system test for fastddsspy tool

Usage: test.py --exe <binary_path> --pub <publisher_path> --test <test_name>

Arguments:
    Fast DDS Spy binary path          : -e | --exe binary_path
    Fast DDS publisher path           : -p | --pub publisher_path
    Test name                         : -t | --test
"""
import argparse
import importlib
import os
import sys


DESCRIPTION = """Script to execute Fast DDS Spy executable test"""
USAGE = ('python3 tests.py -e <path/to/fastddsspy-executable>'
         ' [-d]')


def is_linux() -> bool:
    """
    @brief Check if the script is running in a Linux environment.

    @return: True if the script is running in a Linux environment, False otherwise.
    """
    return os.name == 'posix'


def executable_permission_value():
    """
    @brief Return the executable permission value depending on the operating system.

    @return: The executable permission value.
    """
    if os.name == 'nt':
        return os.X_OK  # windows
    else:
        return os.EX_OK


def file_exist_and_have_permissions(file_path):
    """
    @brief Check if a file exists and has executable permissions.

    @param file_path: The path of the file to check.
    @return: The file path if it exists and has executable permissions, otherwise None.
    """
    if os.access(file_path, executable_permission_value()):
        return file_path
    else:
        return None


def parse_options():
    """
    @brief Parse command-line arguments.

    @return: The parsed arguments.
    """
    parser = argparse.ArgumentParser(
        formatter_class=argparse.ArgumentDefaultsHelpFormatter,
        add_help=True,
        description=(DESCRIPTION),
        usage=(USAGE)
    )
    required_args = parser.add_argument_group('required arguments')
    required_args.add_argument(
        '-e',
        '--exe',
        type=file_exist_and_have_permissions,
        required=True,
        help='Path to DDS Spy executable.'
    )
    required_args.add_argument(
        '-p',
        '--pub',
        type=file_exist_and_have_permissions,
        required=True,
        help='Path to DDS Publisher executable.'
    )
    required_args.add_argument(
        '-t',
        '--test',
        type=str,
        required=True,
        help='Test to run.'
    )

    return parser.parse_args()


def get_config_path_spy(arguments_spy, exec_spy, config):
    """
    @brief Get the path of the configuration of the Spy

    @param arguments_spy: List of arguments for the DDS Spy.
    @param exec_spy: Fast DDS Spy executable file path.
    @param config: Name of the configuration file.
    """
    index = arguments_spy.index('configuration')
    if is_linux():
        arguments_spy[index] = \
            exec_spy.replace('fastddsspy_tool/fastddsspy',
                             config)
    else:
        if 'Debug' in exec_spy:
            build_type = 'Debug'
        else:
            build_type = 'Release'
        arguments_spy[index] = \
            exec_spy.replace('fastddsspy_tool/' + build_type + '/fastddsspy.exe',
                             config)
    return arguments_spy


def main():
    """@brief The main entry point of the program."""
    args = parse_options()

    module = importlib.import_module('test_cases.'+args.test)
    test_class = module.TestCase_instance()
    test_class.exec_spy = args.exe
    test_class.exec_dds = args.pub

    if test_class.config != '':
        test_class.arguments_spy = get_config_path_spy(
                                    test_class.arguments_spy,
                                    test_class.exec_spy,
                                    test_class.config)

    dds = test_class.run_dds()
    spy = test_class.run_tool()

    if spy is None:
        print('ERROR: Wrong output')
        test_class.stop_dds(dds)
        sys.exit(1)

    if not test_class.one_shot:

        output = test_class.send_command_tool(spy)

        if not test_class.valid_output(output):
            test_class.stop_tool(spy)
            test_class.stop_dds(dds)
            print('ERROR: Output command not valid')
            sys.exit(1)

    if not test_class.stop_dds(dds):
        sys.exit(1)

    if not test_class.one_shot:
        test_class.stop_tool(spy)

    sys.exit(0)


if __name__ == '__main__':
    main()
