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

    def __init__(self, name, one_shot, command, dds, config,
                 arguments_dds, arguments_spy, commands_spy, output):
        """
        @brief Initialize the object.

        @param name: The name of the test.
        @param one_shot: A boolean indicating if the object runs in one-shot mode.
        @param command: The command associated with the object.
        @param dds: A boolean indicating if the object launch a DDS Publisher.
        @param config: The configuration of the DDS Spy.
        @param arguments_dds: A list of arguments for the DDS Publisher.
        @param arguments_spy: A list of arguments for the DDS Spy.
        @param commands_spy: A list of commands for the DDS Spy interactive application.
        @param output: The expected output associated with the object.
        """
        self.name = name
        self.one_shot = one_shot
        self.command = command
        self.dds = dds
        self.config = config
        self.arguments_dds = arguments_dds
        self.arguments_spy = arguments_spy
        self.commands_spy = commands_spy
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

    def is_linux(self):
        """
        @brief Check if the script is running in a Linux environment.

        @return: True if the script is running in a Linux environment, False otherwise.
        """
        return os.name == 'posix'

    def is_windows(self):
        """
        @brief Check if the script is running in a Windows environment.

        @return: True if the script is running in a Windows environment, False otherwise.
        """
        return os.name == 'nt'

    def run_dds(self):
        """
        @brief Run the DDS publisher tool with the arguments set in the parameter \
        'arguments_dds' of the class.

        @return Returns a subprocess object representing the running DDS publisher.
        """
        if self.dds:
            self.logger.info('Run tool')
            self.command = [self.exec_dds, 'publisher'] + self.arguments_dds

            self.logger.info('Executing command: ' + str(self.command))

            proc = subprocess.Popen(self.command,
                                    stdin=subprocess.PIPE,
                                    stdout=subprocess.PIPE,
                                    stderr=subprocess.PIPE)

            return proc

    def run_tool(self):
        """
        @brief Run Fast DDS Spy as one_shot if the one_shot parameter is set to true, \
        otherwise. as an interactive application with the arguments specified in the \
        parameter 'arguments_spy' of the class.

        @return Returns the subprocess object representing the running Spy.
        """
        self.logger.info('Run tool')

        self.command = [self.exec_spy] + self.arguments_spy

        self.logger.info('Executing command: ' + str(self.command))

        proc = subprocess.Popen(self.command,
                                stdin=subprocess.PIPE,
                                stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE,
                                encoding='utf8')

        if (self.one_shot):
            output = ''
            try:
                output = proc.communicate(timeout=18)[0]
            except subprocess.TimeoutExpired:
                proc.kill()
            if not self.valid_output(output):
                return ('wrong output')

        else:
            self.read_output(proc)
        return proc

    def send_command_tool(self, proc):
        """
        @brief Send a command to the running Spy.

        @param proc: The subprocess object representing the running Spy.
        @return Returns the output received after sending the command.
        """
        time.sleep(0.2)
        proc.stdin.write((self.commands_spy[0]+'\n'))
        proc.stdin.flush()

        output = self.read_output(proc)
        return (output)

    def read_output(self, proc):
        """
        @brief Read the output from the subprocess.

        @param proc: The subprocess object representing the running process.
        @return Returns the accumulated output read from the subprocess.
        """
        output = ''
        count = 0
        max_count = 1000

        while True:
            count += 1

            if count > max_count:
                break

            line = proc.stdout.readline()

            if ('Insert a command for Fast DDS Spy:' in line):
                break

            output = output + f'{line}\n'
        return output

    def output_command(self):
        """
        @brief Get the expected output.

        @return Returns the expected output.
        """
        return (self.output)

    def valid_guid(self, guid):
        """
        @brief Check if a GUID has the correct pattern.

        @param guid: The GUID to check.
        @return Returns True if the GUID is valid, False otherwise.
        """
        pattern = r'^((guid:)\s([0-9a-f]{2}\.){11}[0-9a-f]{2}\|([0-9a-f]\.){3}[0-9a-f]{1,})$'
        id_guid = guid[guid.find('guid:'):]
        if not re.match(pattern, id_guid):
            return False
        return True

    def valid_rate(self, rate):
        """
        @brief Check if a rate is valid.

        @param rate: The rate to check.
        @return Returns True if the rate is valid, False otherwise.
        """
        pattern = r'^((rate:)\s\d{1,}\s(Hz))$'
        id_rate = rate[rate.find('rate:'):]
        if not re.match(pattern, id_rate):
            return False
        return True

    def valid_output(self, output):
        """Check the output ."""
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
        """
        @brief Stop the running Spy.

        @param proc: The subprocess object representing the running Spy.
        @return Returns 1 if the Spy is successfully stopped, 0 otherwise.
        """
        try:
            proc.communicate(input='exit\n', timeout=18)[0]
        except subprocess.TimeoutExpired:
            proc.kill()

    def stop_dds(self, proc):
        """
        @brief Stop the DDS publisher.

        @param proc: The subprocess object representing the running DDS publisher.
        @return Returns 1 if the DDS publisher is successfully stopped, 0 otherwise.
        """
        if self.dds:
            proc.kill()

            if not self.is_stop(proc):
                print('ERROR: DDS Publisher still running')
                return 0

        return 1

    def is_stop(self, proc):
        """
        @brief Check if the subprocess has stopped.

        @param proc: The subprocess to check.
        @return Returns True if the subprocess has stopped,
        False otherwise.
        """
        # give time to stop
        time.sleep(0.5)

        return_code = proc.poll()

        if (return_code is None):
            return False
        return True

    def valid_returncode(self, returncode):
        """
        @brief Check if the return code indicates that
        process has finished cleanly.

        @param returncode: The return code of the subprocess.
        @return Returns True if the return code is zero,
        indicating success, False otherwise. If the process
        name contains the string 'Fail' return True if the return
        code is not zero, False otherwise.
        """
        if 'Fail' in self.name:
            return (returncode != 0)
        return (returncode == 0)
