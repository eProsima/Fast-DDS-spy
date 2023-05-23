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

"""
Tests for the fastddsspy executable.

Contains a package of system test for fastddsspy tool

Usage: test.py -e <binary_path>

Arguments:
    Fast DDS Spy binary path          : -e | --exe binary_path
    Run test in Debug mode          : -d | --debug
"""

import logging
import os
import re
import subprocess
import time


class TestCase():
    """Test class."""

    def __init__(self, name, one_shot, command, dds, config, arguments_dds, arguments_spy, output):
        """
        TODO.

        Parameters
        ----------
        name : int
            TODO.
        one_shot  : bool
            TODO.
        command  : str
            TODO.
        dds : bool
            TODO.
        config : str
            TODO.
        arguments_dds : list
            TODO.
        arguments_spy : list
            TODO.
        output : str
            TODO.
        """
        self.name = name
        self.one_shot = one_shot
        self.command = command
        self.dds = dds
        self.config = config
        self.arguments_dds = arguments_dds
        self.arguments_spy = arguments_spy
        self.output = output
        self.exec_spy = ''
        self.exec_dds = ''

        # Create a custom logger
        self.logger = logging.getLogger('SYS_TEST')
        # Create handlers
        l_handler = logging.StreamHandler()
        # Create formatters and add it to handlers
        l_format = '[%(asctime)s][%(name)s][%(levelname)s] %(message)s'
        l_format = logging.Formatter(l_format)
        l_handler.setFormatter(l_format)
        # Add handlers to the logger
        self.logger.addHandler(l_handler)

    def signal_handler(self, signum, frame):
        """
        Ignore Signal handler.

        This method is required in Windows to not handle the signal that
        is sent to the subprocess.
        """
        pass

    def is_linux(self):
        """Return whether the script is running in a Linux environment."""
        return os.name == 'posix'

    def is_windows(self):
        """Return whether the script is running in a Windows environment."""
        return os.name == 'nt'

    def run_dds(self):
        """TODO."""
        self.logger.info('Run tool')
        self.command = [self.exec_dds, 'publisher'] + self.arguments_dds

        self.logger.info('Executing command: ' + str(self.command))

        proc = subprocess.Popen(self.command,
                                stdin=subprocess.PIPE,
                                stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE)

        return proc

    def run_tool(self):
        """TODO."""
        self.logger.info('Run tool')
        if (self.one_shot):
            self.command = [self.exec_spy] + self.arguments_spy
        else:
            self.command = [self.exec_spy]

        self.logger.info('Executing command: ' + str(self.command))

        proc = subprocess.Popen(self.command,
                                stdin=subprocess.PIPE,
                                stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE,
                                encoding='utf8')

        if (self.one_shot):
            output = ''
            try:
                output = proc.communicate(timeout=15)[0]
            except subprocess.TimeoutExpired:
                proc.kill()
            if not self.valid_output(output):
                return ('wrong output')

        else:
            self.read_output(proc)
        return proc

    def send_command_tool(self, proc):
        """TODO."""
        # give time
        time.sleep(0.2)
        proc.stdin.write((self.arguments_spy[0]+'\n'))
        proc.stdin.flush()
        output = self.read_output(proc)
        return (output)

    def read_output(self, proc):
        """TODO."""
        output = ''
        while True:
            line = proc.stdout.readline()
            if ('Insert a command for Fast DDS Spy:' in line):
                break
            output = output + f'{line}\n'
        return output

    def output_command(self):
        """TODO."""
        return (self.output)

    def valid_guid(self, guid):
        """TODO."""
        pattern = r'^((guid:)\s([0-9a-f]{2}\.){11}[0-9a-f]{2}\|([0-9a-f]\.){3}[0-9a-f]{1,})$'
        id_guid = guid[guid.find('guid:'):]
        if not re.match(pattern, id_guid):
            return False
        return True

    def valid_rate(self, rate):
        """TODO."""
        pattern = r'^((rate:)\s\d{1,}\s(Hz))$'
        id_rate = rate[rate.find('rate:'):]
        if not re.match(pattern, id_rate):
            return False
        return True

    def valid_output(self, output):
        """TODO."""
        if ('Fail' in self.name or (self.is_windows() and
           ('--HelpCommand' == self.name))):
            return True
        expected_output = self.output_command()
        lines_expected_output = expected_output.splitlines()
        lines_output = output.splitlines()
        if expected_output == output:
            return True
        guid = True
        rate = True
        ignore_msgs = ['commit hash:', '- 01.0f', '\x1b[34;1m -> Function \x1b[36m',
                       '\x1b[31;1m[\x1b[37;1mFASTDDSSPY_TOOL\x1b[31;1m Err']
        for i in range(len(lines_expected_output)):
            if 'guid:' in lines_expected_output[i]:
                guid = self.valid_guid(lines_expected_output[i])
            elif 'rate:' in lines_expected_output[i]:
                rate = self.valid_rate(lines_expected_output[i])
            elif ((ignore_msgs[0] in lines_expected_output[i]) or
                  (ignore_msgs[1] in lines_expected_output[i]) or
                  (ignore_msgs[2] in lines_expected_output[i]) or
                  (ignore_msgs[3] in lines_expected_output[i])):
                pass
            elif lines_expected_output[i] != lines_output[i]:
                return False
        return (guid and rate)

    def stop_tool(self, proc):
        """TODO."""
        try:
            proc.communicate(input='exit\n', timeout=15)[0]
        except subprocess.TimeoutExpired:
            proc.kill()

        if not self.is_stop(proc):
            print('ERROR: Fast DDS Spy still running')
            return 1

        return 0

    def stop_dds(self, proc):
        """TODO."""
        try:
            proc.terminate()
            proc.wait(timeout=15)
        except subprocess.TimeoutExpired:
            proc.kill()

        if not self.is_stop(proc):
            print('ERROR: DDS Publisher still running')
            return 1

        return 0


    def is_stop(self, proc):
        """TODO."""
        # give time
        time.sleep(0.2)

        return_code = proc.poll()

        if (return_code is None):
            return False
        return True

    def valid_returncode(self, returncode):
        """TODO."""
        if 'Fail' in self.name:
            return (returncode != 0)
        return (returncode == 0)
