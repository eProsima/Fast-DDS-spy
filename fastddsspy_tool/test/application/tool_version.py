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
            name='ToolVersionCommand',
            one_shot=False,
            command=[],
            dds=False,
            config='',
            arguments_dds=[],
            arguments_spy=['version'],
            output=
""">> \x1b[0mFast DDS Spy v0.1.0

commit hash: 16ed7e8c93d7481d8b426746af9ec3ffa323f451

"""
        )
