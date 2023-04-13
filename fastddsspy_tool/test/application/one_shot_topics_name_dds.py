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

import test_class


class TestCase_instance (test_class.TestCase):

    def __init__(self):
        super().__init__(
            name='TopicsDDSCommand',
            one_shot=True,
            command=[],
            dds=True,
            config='',
            arguments_dds=[],
            arguments_spy=['topics', 'HelloWorldTopic'],
            output="""name: HelloWorldTopic
type: HelloWorld
datawriters:
- 01.0f.d8.74.8b.fc.26.98.00.00.00.00|0.0.1.3
rate: 10 Hz
dynamic_type_discovered: false
"""
        )
