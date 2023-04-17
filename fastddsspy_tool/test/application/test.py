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
import importlib
import os
import sys
import time

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
    """TODO."""
    args = parse_options()

    module = importlib.import_module(args.test)
    test_function = module.TestCase_instance()

    test_function.exec_spy = args.exe
    local_path_dds = 'fastddsspy_tool/test/application/dds/AdvancedConfigurationExample/'

    if test_function.is_linux():
        local_dds = local_path_dds + 'AdvancedConfigurationExample'
        test_function.exec_dds = args.exe.replace('fastddsspy_tool/fastddsspy', local_dds)

        if test_function.configuration != '':
            index = test_function.arguments_spy.index('configuration')
            test_function.arguments_spy[index] = \
                args.exe.replace('fastddsspy_tool/fastddsspy', test_function.configuration)
    else:
        local_dds = local_path_dds + 'Debug/AdvancedConfigurationExample'
        test_function.exec_dds = args.exe.replace('fastddsspy_tool/Debug/fastddsspy', local_dds)

        if test_function.configuration != '':
            index = test_function.arguments_spy.index('configuration')
            if test_function.is_linux():
                test_function.arguments_spy[index] = \
                    args.exe.replace('fastddsspy_tool/fastddsspy', test_function.configuration)
            else:
                test_function.arguments_spy[index] = \
                    args.exe.replace('fastddsspy_tool/Debug/fastddsspy.exe', test_function.configuration)

    if (test_function.dds):
        dds = test_function.run_dds()
        if test_function.is_stop(dds):
            print('ERROR: DDS Publisher not running')
            sys.exit(1)
        # give time to start publishing
        time.sleep(1.0)

    spy = test_function.run_tool()

    if (spy == 'wrong output'):
        print('ERROR: Wrong output')
        sys.exit(1)

    if not test_function.one_shot:
        if test_function.is_stop(spy):
            print('ERROR: Fast DDS Spy not running')
            sys.exit(1)

        output = test_function.send_command_tool(spy)

        if not test_function.valid_output(output):
            print('ERROR: Output command not valid')
            sys.exit(1)

        test_function.stop_tool(spy)

    if not test_function.is_stop(spy):
        print('ERROR: Fast DDS Spy still running')
        sys.exit(1)

    if not test_function.valid_returncode(spy.returncode):
        print('ERROR: Wrong Fast DDS Spy return code')
        sys.exit(1)

    if (test_function.dds):
        test_function.stop_dds(dds)

        if not test_function.is_stop(dds):
            print('ERROR: DDS Publisher still running')
            sys.exit(1)

        # if not test_function.valid_returncode(dds.returncode):
        #     print('ERROR: Wrong DDS Publisher return code')
        #     sys.exit(1)

    return 0


if __name__ == '__main__':
    main()
