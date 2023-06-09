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

import re

import test_class


class TestCase_instance (test_class.TestCase):
    """@brief A subclass of `test_class.TestCase` representing a specific test case."""

    def __init__(self):
        """
        @brief Initialize the TestCase_instance object.

        This test launch:
            fastddsspy --config-path fastddsspy_tool/test/application/configuration/\
                configuration_discovery_time.yaml topics HelloWorldTopic
            AdvancedConfigurationExample publisher
        """
        super().__init__(
            name='TopicsNameDDSCommand',
            one_shot=True,
            command=[],
            dds=True,
            config='fastddsspy_tool/test/application/configuration/\
configuration_discovery_time.yaml',
            arguments_dds=[],
            arguments_spy=['--config-path', 'configuration', 'topics', 'HelloWorldTopic'],
            commands_spy=[],
            output="""name: HelloWorldTopic\n\
type: HelloWorld\n\
datawriters:\n\
- %%guid%%\n\
rate: %%rate%%\n\
dynamic_type_discovered: false\n"""
        )

    def valid_guid(self, guid) -> bool:
        """
        @brief Check if a GUID has the correct pattern.

        @param guid: The GUID to check.
        @return Returns True if the GUID is valid, False otherwise.
        """
        pattern = r'^((-)\s([0-9a-f]{2}\.){11}[0-9a-f]{2}\|([0-9a-f]\.){3}[0-9a-f]{1,})$'
        id_guid = guid[guid.find('-'):]
        if not re.match(pattern, id_guid):
            print('Not valid guid: ')
            print(guid)
            return False
        return True