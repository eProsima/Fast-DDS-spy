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
            name='--configFailTypeCommand',
            one_shot=True,
            command=[],
            dds=False,
            config='fastddsspy_tool/test/application/configuration/\
configuration_wrong_type.yaml',
            arguments_dds=[],
            arguments_spy=['--config-path', 'configuration', 'datareaders'],
            output="""\x1b[37;1m2023-04-13 11:36:09.453 \
\x1b[31;1m[\x1b[37;1mFASTDDSSPY_TOOL\x1b[31;1m Error] \
\x1b[37mError Loading Fast DDS Spy Configuration from file \
/home/irenebm/eprosima/annapurna/DDS-Spy/build/fastddsspy_tool\
/test/application/configuration/configuration_wrong_type.yaml. \
Error message:\n Error loading DDS Router configuration from yaml:\n \
Error getting required value of type <N8eprosima7ddspipe4core5types8\
DomainIdE> in tag <domain> :\n Incorrect format for primitive value, \
expected <j>:\n yaml-cpp: error at line 4, column 11: bad conversion\
\x1b[34;1m -> Function \x1b[36mmain\x1b[m\n"""
        )
