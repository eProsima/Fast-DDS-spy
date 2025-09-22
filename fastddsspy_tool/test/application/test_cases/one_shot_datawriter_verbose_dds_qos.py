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
                configuration_discovery_time.yaml datawriter verbose
            AdvancedConfigurationExample publisher --reliable --transient
        """
        super().__init__(
            name='DatawriterVerboseDDSQosCommand',
            one_shot=True,
            command=[],
            dds=True,
            config='fastddsspy_tool/test/application/configuration/\
configuration_discovery_time.yaml',
            arguments_dds=['--reliable', '--transient'],
            arguments_spy=['--config-path', 'configuration', 'datawriter', 'verbose'],
            commands_spy=[],
            output="""- guid: %%guid%%\n\
  participant: Participant_pub\n\
  topic:\n\
    name: HelloWorldTopic\n\
    type: HelloWorld\n\
    partitions: ""\n\
  qos:\n\
    durability: transient-local\n\
    reliability: reliable\n"""
        )
