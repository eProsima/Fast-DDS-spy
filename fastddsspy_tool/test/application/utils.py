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

    Run test in Debug mode          : -d | --debug

"""

import argparse
import os
import subprocess

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
    parser.add_argument(
        '-d',
        '--debug',
        action='store_true',
        help='Print test debugging info.'
    )
    return parser.parse_args()


def test_spy_start(fastddsspy, logger):

    command = [fastddsspy]
    logger.info('Executing command: ' + str(command))

    proc = subprocess.Popen(command,
                            stdin=subprocess.PIPE,
                            stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE,
                            universal_newlines=True)
    return proc


def test_spy_command(proc, command, logger):

    try:
        proc.communicate(input=command, timeout=5)
        # proc.kill()
        logger.debug('-----------------------------------------------------')
        logger.debug('Command ' + str(command) + ' worked.')
        logger.debug('-----------------------------------------------------')
    except subprocess.TimeoutExpired:
        proc.kill()
        logger.error('-----------------------------------------------------')
        logger.error('Command ' + str(command) + ' failed.')
        logger.error('-----------------------------------------------------')
        return 1

    return 0
