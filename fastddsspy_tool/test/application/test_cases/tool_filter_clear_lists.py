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

        This TestCase creates two filter lists "partitions" and "topic" and clear all the filters.

        And prints the list of filters (empty)

        This test launch:
            fastddsspy
            >> filter add partitions A
            >> filter set topic Square "x > 10"
            >> filter clear
            >> filters
        """
        super().__init__(
            name='ToolFilterClearLists',
            one_shot=False,
            command=[],
            dds=False,
            config='',
            arguments_dds=[],
            arguments_spy=[],
            commands_spy=[
                'filter add partitions A',
                'filter set topic Square "x > 10"',
                'filters clear',
                'filters',
            ],
            output=(
                '--------\n\n'
                'Filters:\n\n'
                '--------\n\n\n\n'
                '  Topic:\n\n\n\n'
                '  Partitions:\n'
            )
        )
