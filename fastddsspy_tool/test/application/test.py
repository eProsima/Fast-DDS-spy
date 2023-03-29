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

Usage: test.py -e <binary_path>

Arguments:

    Fast DDS Spy binary path          : -e | --exe binary_path

    Run test in Debug mode          : -t | --test

"""

import argparse
import os
import importlib
import sys

DESCRIPTION = """Script to execute Fast DDS Spy executable test"""
USAGE = ('python3 tests.py -e <path/to/fastddsspy-executable>'
         ' [-d]')


def executable_permission_value():
    """Return executable permissions value depending on the OS."""
    if os.name == 'nt':
        return os.X_OK  # windows
    else:
        return os.EX_OK


def file_exist_and_have_permissions(file_path):
    """Check if a file exists and have executable permissions."""
    if os.access(file_path, executable_permission_value()):
        return file_path
    else:
        return None


def parse_options():
    """
    Parse arguments.

    :return: The arguments parsed.
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
        help='Path to discovery-server executable.'
    )
    required_args.add_argument(
        '-t',
        '--test',
        type=str,
        required=True,
        help='Test to run.'
    )
    parser.add_argument(
        '-d',
        '--debug',
        action='store_true',
        help='Print test debugging info.'
    )
    return parser.parse_args()


def main():

    args = parse_options()

    module = importlib.import_module(args.test)
    test_function = module.TestCase_instance()

    test_function.exec_spy = args.exe

    result = test_function.run()

    if test_function.one_shot:
        if not test_function.is_stop_tool(result):
            sys.exit(1)
        if not test_function.valid_output_tool(result.returncode):
            sys.exit(1)
    else:
        if test_function.is_stop_tool(result):
            sys.exit(1)

        test_function.send_command_tool(result)

        test_function.stop_tool(result)

        if not test_function.is_stop_tool(result):
            sys.exit(1)

        if not test_function.valid_output_tool(result.returncode):
            sys.exit(1)

    return 0


if __name__ == '__main__':
    main()
