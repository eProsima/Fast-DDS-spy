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

"""Tests for the fastddsspy executable."""

import os

import test_class


def is_windows() -> bool:
    """
    @brief Check if the script is running in a Windows environment.

    @return: True if the script is running in a Windows environment, False otherwise.
    """
    return os.name == 'nt'


class TestCase_instance (test_class.TestCase):
    """@brief A subclass of `test_class.TestCase` representing a specific test case."""

    def __init__(self):
        """
        @brief Initialize the TestCase_instance object.

        This test launch:
            fastddsspy --help
        """
        super().__init__(
            name='--HelpCommand',
            one_shot=True,
            command=[],
            dds=False,
            config='',
            arguments_dds=[],
            arguments_spy=['--help'],
            commands_spy=[],
            output="""Usage: Fast DDS Spy \n\
Start an interactive CLI to introspect a DDS network.\n\
General options:\n\

Application help and information.\n\
  -h --help           Print this help message.\n\
  -v --version        Print version, branch and commit hash.\n\
\n\
Application parameters\n\
  -c --config-path    Path to the Configuration File (yaml format) \
[Default: ./FASTDDSSPY_CONFIGURATION.yaml].\n\
  -r --reload-time    Time period in seconds to reload configuration file. \
This is needed when FileWatcher functionality is not available \
(e.g. config file is a symbolic link). Value 0 does not reload file. [Default: 0].\n\
     --domain         Set the domain (0-255) to spy on. [Default = 0].\n\
\n\
Debug parameters\n\
  -d --debug          Set log verbosity to Info                                   \
                                                                                  \n\
                                             (Using this option with \
--log-filter and/or --log-verbosity will head to undefined behaviour).\n\
     --log-filter     Set a Regex Filter to filter by category the info and warning \
log entries. [Default = "FASTDDSSPY"]. \n\
     --log-verbosity  Set a Log Verbosity Level higher or equal the one given. \
(Values accepted: "info","warning","error" no Case Sensitive) [Default = "warning"]. \n\n
"""
        )

    def valid_output(self, output):
        """
        @brief Validate the output against the expected output or delegate \
        the validation to the parent class.

        @param output: The actual output obtained from executing a command.
        @return True if the output is considered valid, either because the \
        system is running on Windows or the parent class validates the output, \
        False otherwise.
        """
        if (is_windows()):
            return True
        else:
            return super().valid_output(output)
