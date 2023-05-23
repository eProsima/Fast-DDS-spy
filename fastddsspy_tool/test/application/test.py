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


def get_exec_dds_arguments_spy(test_class, args):
    """TODO."""
    local_path_dds = 'fastddsspy_tool/test/application/dds/AdvancedConfigurationExample/'
    if test_class.is_linux():
        local_dds = local_path_dds + 'AdvancedConfigurationExample'
        test_class.exec_dds = args.exe.replace('fastddsspy_tool/fastddsspy', local_dds)
        if test_class.config != '':
            index = test_class.arguments_spy.index('configuration')
            test_class.arguments_spy[index] = \
                args.exe.replace('fastddsspy_tool/fastddsspy',
                                 test_class.config)
    else:

        if 'Debug' in args.exe:
            build_type = 'Debug'
        else:
            build_type = 'Release'

        local_dds = local_path_dds + build_type + '/AdvancedConfigurationExample'
        test_class.exec_dds = args.exe.replace('fastddsspy_tool/' + build_type + '/fastddsspy',
                                               local_dds)

        if test_class.config != '':
            index = test_class.arguments_spy.index('configuration')
            test_class.arguments_spy[index] = \
                args.exe.replace('fastddsspy_tool/' + build_type + '/fastddsspy.exe',
                                 test_class.config)


def main():
    """TODO."""
    args = parse_options()

    module = importlib.import_module(args.test)
    test_class = module.TestCase_instance()
    test_class.exec_spy = args.exe

    get_exec_dds_arguments_spy(test_class, args)

    dds = test_class.run_dds()

    spy = test_class.run_tool()

    if (spy == 'wrong output'):
        print('ERROR: Wrong output')
        test_class.stop_dds(dds)
        sys.exit(1)

    if not test_class.one_shot:

        output = test_class.send_command_tool(spy)

        if not test_class.stop_tool(spy):
            test_class.stop_dds(dds)
            sys.exit(1)

        if not test_class.valid_output(output):
            print('ERROR: Output command not valid')
            sys.exit(1)

    if not test_class.stop_dds(dds):
        sys.exit(1)

    sys.exit(0)


if __name__ == '__main__':
    main()
