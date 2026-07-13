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
import re
import sys
import time
import traceback


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


def has_explicit_domain(arguments_spy) -> bool:
    """Return whether the test already sets a domain from the CLI."""
    return '--domain' in arguments_spy


def config_path_from_arguments(arguments_spy) -> str:
    """Return the resolved config path if the test uses --config-path."""
    if '--config-path' not in arguments_spy:
        return ''

    config_index = arguments_spy.index('--config-path') + 1
    if config_index >= len(arguments_spy):
        return ''

    return arguments_spy[config_index]


def config_has_domain(config_path) -> bool:
    """Return whether the referenced yaml config already sets a DDS domain."""
    if not config_path or not os.path.isfile(config_path):
        return False

    with open(config_path, encoding='utf-8') as file:
        return re.search(r'^\s*domain\s*:', file.read(), flags=re.MULTILINE) is not None


def isolated_test_domain() -> str:
    """
    Pick a non-default domain for this test process to avoid cross-test discovery
    residue and ambient DDS traffic on shared runners.
    """
    return str(30 + ((os.getpid() ^ time.time_ns()) % 170))


def build_test_case(args):
    """Create and configure the requested test case."""
    module = importlib.import_module('test_cases.'+args.test)
    test_class = module.TestCase_instance()
    test_class.exec_spy = args.exe
    test_class.exec_dds = args.pub

    if test_class.config != '':
        test_class.arguments_spy = get_config_path_spy(
                                    test_class.arguments_spy,
                                    test_class.exec_spy,
                                    test_class.config)

    if not has_explicit_domain(test_class.arguments_spy):
        config_path = config_path_from_arguments(test_class.arguments_spy)
        if not config_has_domain(config_path):
            test_domain = isolated_test_domain()
            test_class.arguments_spy = ['--domain', test_domain] + test_class.arguments_spy
            if test_class.dds:
                test_class.arguments_dds = test_class.arguments_dds + ['--domain', test_domain]

    return test_class


def interactive_test_exit_code(test_class, spy) -> int:
    """Return the exit code for an interactive test run."""
    output = test_class.send_commands_tool(spy)

    if not test_class.valid_output(output):
        print('ERROR: Output command not valid')
        return 1

    return 0


def test_exit_code(test_class, spy) -> int:
    """Return the exit code for the current test result."""
    if spy is None:
        print('ERROR: Wrong output')
        return 1

    if test_class.one_shot:
        return 0

    return interactive_test_exit_code(test_class, spy)


def run_test_case(test_class):
    """Run the configured test case and return processes plus exit code."""
    dds = None
    spy = None
    exit_code = 1

    try:
        dds = test_class.run_dds()
        spy = test_class.run_tool()
        exit_code = test_exit_code(test_class, spy)
    except Exception:
        traceback.print_exc()

    return dds, spy, exit_code


def cleanup_test_case(test_class, dds, spy, exit_code) -> int:
    """Stop helper processes and return the final exit code."""
    if dds is not None and not test_class.stop_dds(dds):
        exit_code = 1

    if spy is not None and not test_class.one_shot and not test_class.stop_tool(spy):
        exit_code = 1

    return exit_code


def main():
    """@brief The main entry point of the program."""
    args = parse_options()
    test_class = build_test_case(args)
    dds = None
    spy = None
    exit_code = 1

    try:
        dds, spy, exit_code = run_test_case(test_class)
    finally:
        exit_code = cleanup_test_case(test_class, dds, spy, exit_code)

    sys.exit(exit_code)


if __name__ == '__main__':
    main()
