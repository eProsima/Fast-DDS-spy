# Copyright 2025 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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

        This TestCase creates the filter list "partitions" with "B|C" as filters
        (removes the first element "A").

        And prints the list of filters

        This test launch:
            fastddsspy
            >> filter set partitions A
            >> filter add partitions B
            >> filter add partitions C
            >> filter remove partitions A
            >> filters
        """
        super().__init__(
            name='ToolFilterSetPartitionsComplex',
            one_shot=False,
            command=[],
            dds=False,
            config='',
            arguments_dds=[],
            arguments_spy=[],
            commands_spy=[
                'filter set partitions A',
                'filter set topics Square',
                'filter clear',
                'filters',
            ],
            output=(
                'Filter lists (1)\n'
                '\n\n\n'
                '  partitions (2):\n'
                '\n'
                '    - B\n'
                '\n'
                '    - C\n'
            )
        )
