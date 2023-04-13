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
            name='--configFailCommand',
            one_shot=True,
            command=[],
            dds=False,
            config='hello',
            arguments_dds=[],
            arguments_spy=['--config-path', 'configuration'],
            output="""Usage: Fast DDS Spy \n""" \
"""Start an interactive CLI to introspect a DDS network.
General options:

Application help and information.
  -h --help           Print this help message.
  -v --version        Print version, branch and commit hash.

Application parameters
  -c --config-path    Path to the Configuration File (yaml format) [Default: ./FASTDDSSPY_CONFIGURATION.yaml].
  -r --reload-time    Time period in seconds to reload configuration file. This is needed when FileWatcher functionality is not available (e.g. config file is a symbolic link). Value 0 does not reload file. [Default: 0].

Debug parameters
  -d --debug          Set log verbosity to Info                                                                                                                     \n""" \
"""                                             (Using this option with --log-filter and/or --log-verbosity will head to undefined behaviour).
     --log-filter     Set a Regex Filter to filter by category the info and warning log entries. [Default = "(DDSPIPE|FASTDDSSPY)"]. \n""" \
"""     --log-verbosity  Set a Log Verbosity Level higher or equal the one given. (Values accepted: "info","warning","error" no Case Sensitive) [Default = "warning"]. \n

\x1b[37;1m2023-04-12 14:29:23.337 \x1b[31;1m[\x1b[37;1mFOXGLOVEWS_ARGS\x1b[31;1m Error] \x1b[37mOption '--config-path' requires an existing readable file as argument.\x1b[34;1m -> Function \x1b[36mReadable_File\x1b[m
"""
        )
