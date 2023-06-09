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

"""Contains a test class for fastddsspy tool tests."""

import re
import subprocess
import time


SLEEP_TIME = 0.2


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

    def run_dds(self):
        """
        @brief Run the DDS publisher tool with the arguments set in the parameter \
        'arguments_dds' of the class.

        @return Returns a subprocess object representing the running DDS publisher.
        """
        if self.dds:
            self.command = [self.exec_dds, 'publisher'] + self.arguments_dds

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
        self.command = [self.exec_spy] + self.arguments_spy

        proc = subprocess.Popen(self.command,
                                stdin=subprocess.PIPE,
                                stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE,
                                encoding='utf8')

        if (self.one_shot):
            output = ''
            try:
                output = proc.communicate(timeout=10)[0]
            except subprocess.TimeoutExpired:
                proc.kill()
            if not self.valid_output(output):
                return None

        else:
            self.read_output(proc)
        return proc

    def send_command_tool(self, proc):
        """
        @brief Send a command to the running Spy.

        @param proc: The subprocess object representing the running Spy.
        @return Returns the output received after sending the command.
        """
        time.sleep(SLEEP_TIME)
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
        max_count = 500

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

    def valid_guid(self, guid) -> bool:
        """
        @brief Check if a GUID has the correct pattern.

        @param guid: The GUID to check.
        @return Returns True if the GUID is valid, False otherwise.
        """
        pattern = r'^((guid:)\s([0-9a-f]{2}\.){11}[0-9a-f]{2}\|([0-9a-f]\.){3}[0-9a-f]{1,})$'
        id_guid = guid[guid.find('guid:'):]
        if not re.match(pattern, id_guid):
            print('Not valid guid: ')
            print(guid)
            return False
        return True

    def valid_rate(self, rate) -> bool:
        """
        @brief Check if a rate is valid.

        @param rate: The rate to check.
        @return Returns True if the rate is valid, False otherwise.
        """
        pattern_1 = r'^((rate:)\s\d*\.?\d*\s(Hz))$'     # rate: 10.5 Hz
        pattern_2 = r'^((rate:)\s(inf)\s(Hz))$'         # rate: inf Hz
        id_rate = rate[rate.find('rate:'):]
        if re.match(pattern_1, id_rate):
            return True
        if re.match(pattern_2, id_rate):
            return True
        print('Not valid rate: ')
        print(rate)
        return False

    def valid_output(self, output) -> bool:
        """
        @brief Check the validity of the output against the expected output.

        @param output: The actual output obtained from executing a command.
        @return Returns True if the output matches the expected output or \
        satisfies specific conditions for lines containing '%%guid%%' or '%%rate%%', \
        False otherwise.

        The function compares the provided 'output' with the expected output.
        It checks each line of the 'output' and 'expected_output' to determine their validity.

        If the entire 'output' matches the 'expected_output', the function returns True.
        Otherwise, it iterates over each line and performs the following checks:
        - If a line in 'expected_output' contains '%%guid%%', it calls the 'valid_guid()'.
        - If a line in 'expected_output' contains '%%rate%%', it calls the 'valid_rate()'.
        - If a line does not contain '%%guid%%' or '%%rate%%', it compares the corresponding \
            lines in 'output' and 'expected_output' for equality.

        If any line does not meet the expected conditions, the function returns False and prints \
        the 'output' and 'expected_output' for debugging purposes.

        """
        expected_output = self.output_command()
        if expected_output == output:
            return True

        lines_expected_output = expected_output.splitlines()
        lines_output = output.splitlines()
        guid = True
        rate = True
        for i in range(len(lines_expected_output)):
            if '%%guid%%' in lines_expected_output[i]:
                guid = self.valid_guid(lines_output[i])
            elif '%%rate%%' in lines_expected_output[i]:
                rate = self.valid_rate(lines_output[i])
            elif lines_expected_output[i] != lines_output[i]:
                print('Output: ')
                print(output)
                print('Expected output: ')
                print(expected_output)
                return False
        return (guid and rate)

    def stop_tool(self, proc) -> int:
        """
        @brief Stop the running Spy.

        @param proc: The subprocess object representing the running Spy.
        @return Returns 1 if the Spy is successfully stopped, 0 otherwise.
        """
        try:
            proc.communicate(input='exit\n', timeout=10)[0]
        except subprocess.TimeoutExpired:
            proc.kill()

        time.sleep(SLEEP_TIME)

        if not self.is_stop(proc):
            print('ERROR: DDS Spy still running')
            return 0

        return 1

    def stop_dds(self, proc) -> int:
        """
        @brief Stop the DDS publisher.

        @param proc: The subprocess object representing the running DDS publisher.
        @return Returns 1 if the DDS publisher is successfully stopped, 0 otherwise.
        """
        if self.dds:
            try:
                proc.terminate()
                proc.communicate(timeout=15)
            except subprocess.TimeoutExpired:
                proc.kill()

            time.sleep(SLEEP_TIME)

            if not self.is_stop(proc):
                print('ERROR: DDS Publisher still running')
                return 0

        return 1

    def is_stop(self, proc) -> bool:
        """
        @brief Check if the subprocess has stopped.

        @param proc: The subprocess to check.
        @return Returns True if the subprocess has stopped,
        False otherwise.
        """
        return_code = proc.poll()

        if (return_code is None):
            return False
        return True
