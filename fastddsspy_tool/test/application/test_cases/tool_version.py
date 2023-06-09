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

import test_class


class TestCase_instance (test_class.TestCase):
    """@brief A subclass of `test_class.TestCase` representing a specific test case."""

    def __init__(self):
        """
        @brief Initialize the TestCase_instance object.

        This test launch:
            fastddsspy
            >> version
        """
        super().__init__(
            name='ToolVersionCommand',
            one_shot=False,
            command=[],
            dds=False,
            config='',
            arguments_dds=[],
            arguments_spy=[],
            commands_spy=['version'],
            output=""">> \x1b[0mFast DDS Spy v0.1.0\n\
\n\
commit hash: 16ed7e8c93d7481d8b426746af9ec3ffa323f451\n\n"""
        )

    def valid_output(self, output):
        """
        @brief Validate the output.

        @param output: The actual output obtained from executing a command.
        @return Always returns True.
        """
        return True
