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
            name='ToolHelpCommand',
            one_shot=False,
            command=[],
            dds=False,
            config='',
            arguments_dds=[],
            arguments_spy=['help'],
            output=
""">> \x1b[0mFast DDS Spy is an interactive CLI that allow to instrospect DDS networks.

Each command shows data related with the network in Yaml format.

Commands available and the information they show:

\thelp                   : this help.

\tversion                : tool version.

\tquit                   : exit interactive CLI and close program.

\tparticipants           : DomainParticipants discovered in the network.

\tparticipants verbose   : verbose information about DomainParticipants discovered in the network.

\tparticipants <Guid>    : verbose information related with a specific DomainParticipant.

\twriters                : DataWriters discovered in the network.

\twriters verbose        : verbose information about DataWriters discovered in the network.

\twriters <Guid>         : verbose information related with a specific DataWriter.

\treader                 : DataReaders discovered in the network.

\treader verbose         : verbose information about DataReaders discovered in the network.

\treader <Guid>          : verbose information related with a specific DataReader.

\ttopics                 : Topics discovered in the network.

\ttopics verbose         : verbose information about Topics discovered in the network.

\ttopics <name>          : verbose information related with a specific Topic.

\tshow <name>            : data of a specific Topic (Data Type must be discovered).

\tshow <name> verbose    : data with additional source info of a specific Topic.

\tshow all               : verbose data of all topics (only those whose Data Type is discovered).



Notes and comments:

\tTo exit from data printing, press enter.

\tEach command is accessible by using its first letter (h/v/q/p/w/r/t/s).



For more information about these commands and formats, please refer to the documentation:

https://fast-dds-spy.readthedocs.io/en/latest/





"""
        )
