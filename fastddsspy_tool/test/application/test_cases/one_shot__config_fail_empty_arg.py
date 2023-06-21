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
            fastddsspy --config-path fastddsspy_tool/test/application/configuration/\
                configuration_wrong_empty_arg.yaml datareaders
        """
        super().__init__(
            name='--configFailArgCommand',
            one_shot=True,
            command=[],
            dds=False,
            config='fastddsspy_tool/test/application/configuration/\
configuration_wrong_empty_arg.yaml',
            arguments_dds=[],
            arguments_spy=['--config-path', 'configuration', 'datareaders'],
            commands_spy=[],
            output="""\x1b[37;1m2023-04-13 11:52:11.327 \
\x1b[31;1m[\x1b[37;1mFASTDDSSPY_TOOL\x1b[31;1m Error] \
\x1b[37mError Loading Fast DDS Spy Configuration from file \
/home/user/DDS-Spy/build/fastddsspy_tool\
/test/application/configuration/configuration_wrong_empty_arg.yaml. \
Error message:\n\
 Error loading DDS Router configuration from yaml:\n\
 Error getting required value of type <N8eprosima7ddspipe4core5types8\
DomainIdE> in\
 tag <domain> :\n Trying to read a primitive value of type <j> from\
 a non scalar yaml.\
\x1b[34;1m -> Function \x1b[36mmain\x1b[m\n"""
        )

    def valid_output(self, output):
        """
        @brief Validate the output.

        @param output: The actual output obtained from executing a command.
        @return Always returns True.
        """
        return True
