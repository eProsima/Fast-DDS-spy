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
            name='ToolTopicsDDSCommand',
            one_shot=False,
            command=[],
            dds=True,
            config='',
            arguments_dds=[],
            arguments_spy=['topics'],
            output=""">> \x1b[0m- name: HelloWorldTopic\n\
\n\
  type: HelloWorld\n\
\n\
  datawriters: 1\n\
\n\
  datareaders: 0\n\
\n\
  rate: 10 Hz\n"""
        )
