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
            name='DatawriterGuidDDSCommand',
            one_shot=True,
            command=[],
            dds=True,
            config='',
            arguments_dds=[],
            arguments_spy=['datawriter', '01.0f.d8.74.d5.a0.cf.f4.00.00.00.00|0.0.1.3'],
            output="""\x1b[1;31m01.0f.d8.74.d5.a0.cf.f4.00.00.00.00|0.0.1.3 \
does not match with any known writer.\x1b[0m\n"""
        )
