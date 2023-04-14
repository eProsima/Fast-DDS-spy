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
            name='HelpCommand',
            one_shot=True,
            command=[],
            dds=False,
            config='',
            arguments_dds=[],
            arguments_spy=['help'],
            output="""Fast DDS Spy is an interactive CLI that allow to instrospect DDS networks.\n\
Each command shows data related with the network in Yaml format.\n\
Commands available and the information they show:\n\
\thelp                   : this help.\n\
\tversion                : tool version.\n\
\tquit                   : exit interactive CLI and close program.\n\
\tparticipants           : DomainParticipants discovered in the network.\n\
\tparticipants verbose   : verbose information about DomainParticipants discovered in the \
network.\n\
\tparticipants <Guid>    : verbose information related with a specific DomainParticipant.\n\
\twriters                : DataWriters discovered in the network.\n\
\twriters verbose        : verbose information about DataWriters discovered in the network.\n\
\twriters <Guid>         : verbose information related with a specific DataWriter.\n\
\treader                 : DataReaders discovered in the network.\n\
\treader verbose         : verbose information about DataReaders discovered in the network.\n\
\treader <Guid>          : verbose information related with a specific DataReader.\n\
\ttopics                 : Topics discovered in the network.\n\
\ttopics verbose         : verbose information about Topics discovered in the network.\n\
\ttopics <name>          : verbose information related with a specific Topic.\n\
\tshow <name>            : data of a specific Topic (Data Type must be discovered).\n\
\tshow <name> verbose    : data with additional source info of a specific Topic.\n\
\tshow all               : verbose data of all topics (only those whose Data Type is \
discovered).\n\
\n\
Notes and comments:\n\
\tTo exit from data printing, press enter.\n\
\tEach command is accessible by using its first letter (h/v/q/p/w/r/t/s).\n\
\n\
For more information about these commands and formats, please refer to the documentation:\n\
https://fast-dds-spy.readthedocs.io/en/latest/\n\

"""
        )
