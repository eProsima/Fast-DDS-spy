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
import logging
import os
import subprocess
import sys
import time

DESCRIPTION = """Script to execute Fast DDS Spy executable test"""
USAGE = ('python3 tests.py -e <path/to/fastddsspy-executable>'
         ' [-d]')

# Sleep time to let process init and finish
SLEEP_TIME = 1


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


def test_spy_closure(fastddsspy):

    command = [fastddsspy, 'exit']
    logger.info('Executing command: ' + str(command))

    proc = subprocess.run(command, capture_output=True, text=True)

    # sleep to let the server run
    time.sleep(SLEEP_TIME)

    # Check whether the process has terminated already
    # Typically, an exit status of 0 indicates that it ran successfully.
    if proc.returncode:
        logger.debug('-----------------------------------------------------')
        logger.error('Command ' + str(command) + ' failed.')
        logger.debug('-----------------------------------------------------')
        return 1
    return 0


if __name__ == '__main__':

    args = parse_options()

    # Create a custom logger
    logger = logging.getLogger('SYS_TEST')
    # Create handlers
    l_handler = logging.StreamHandler()
    # Create formatters and add it to handlers
    l_format = '[%(asctime)s][%(name)s][%(levelname)s] %(message)s'
    l_format = logging.Formatter(l_format)
    l_handler.setFormatter(l_format)
    # Add handlers to the logger
    logger.addHandler(l_handler)
    # Set log level
    if args.debug:
        logger.setLevel(logging.DEBUG)
    else:
        logger.setLevel(logging.INFO)

    if args.exe is None:
        logger.error(
            'Executable binary file does not exist or has no '
            'executable permissions.')
        sys.exit(1)

    sys.exit(
        test_spy_closure(
            args.exe))           # Path to executable
