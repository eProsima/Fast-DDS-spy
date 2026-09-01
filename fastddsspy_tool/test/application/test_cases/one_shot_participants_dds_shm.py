# Copyright 2026 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

"""Cross-platform test for Fast DDS Spy over Shared Memory Transport."""

import test_class


class TestCase_instance(test_class.TestCase):
    """Test participant discovery using Shared Memory Transport."""

    def __init__(self):
        """Initialize the Shared Memory Transport test case."""
        super().__init__(
            name='ParticipantsDDSSharedMemoryCommand',
            one_shot=True,
            command=[],
            dds=True,
            config='fastddsspy_tool/test/application/configuration/\
configuration_discovery_time_shm.yaml',
            arguments_dds=['--transport=shm'],
            arguments_spy=['--config-path', 'configuration', 'participants'],
            commands_spy=[],
            output="""- name: Participant_pub\n\
  guid: %%guid%%\n"""
        )
