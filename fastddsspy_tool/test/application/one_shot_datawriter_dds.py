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
    """TODO."""

    def __init__(self):
        """TODO."""
        super().__init__(
            name='DatawriterDDSCommand',
            one_shot=True,
            command=[],
            dds=True,
            config='',
            arguments_dds=[],
            arguments_spy=['datawriter'],
            output="""- guid: 01.0f.cd.6f.98.9d.4f.19.00.00.00.00|0.0.1.3\n\
  participant: Participant_pub\n\
  topic: HelloWorldTopic [HelloWorld]\n"""
        )