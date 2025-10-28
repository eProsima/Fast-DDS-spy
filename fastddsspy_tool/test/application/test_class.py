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
import signal
import os


SLEEP_TIME = 0.2


def safe_interrupt(p):
    if os.name == 'nt':
        try:
            # On Windows, use CTRL_C_EVENT to interrupt the process
            os.kill(p.pid, signal.CTRL_C_EVENT)
        except Exception:
            p.terminate()
    else:
        p.send_signal(signal.SIGINT)


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

        creationflags = subprocess.CREATE_NEW_PROCESS_GROUP if os.name == 'nt' else 0

        proc = subprocess.Popen(self.command,
                                stdin=subprocess.PIPE,
                                stdout=subprocess.PIPE,
                                stderr=subprocess.PIPE,
                                encoding='utf8',
                                creationflags=creationflags)

        if self.one_shot:
            sleep_time = 3 if self.arguments_spy[:2] == ['show', 'all'] else 1
            time.sleep(sleep_time)
            output = ''
            if self.arguments_spy[:2] == ['show', 'all']:
                safe_interrupt(proc)
            try:
                output = proc.communicate(timeout=10)[0]
            except subprocess.TimeoutExpired:
                proc.kill()
                output = ''
            if not self.valid_output(output):
                return None

        else:
            time.sleep(1)
            self.read_command_output(proc)
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

        output = self.read_command_output(proc)
        return (output)

    def send_commands_tool(self, proc):
        """
        @brief Send all the commands to the running Spy.

        @param proc: The subprocess object representing the running Spy.
        @return Returns the output received after sending the last command.
        """

        outputs = []

        for cmd in self.commands_spy:
            time.sleep(SLEEP_TIME)
            proc.stdin.write(cmd + '\n')
            proc.stdin.flush()
            # read_command_output waits until the 'Insert a command...' prompt, so it
            # returns the output for this command
            outputs.append(self.read_command_output(proc))

        return outputs[-1]

    def read_command_output(self, proc):
        """
        @brief Read the output from the subprocess.

        @param proc: The subprocess object representing the running process.
        @return Returns the accumulated output read from the subprocess.
        """
        output = ''
        count = 0
        # max number of loops while can take to avoid waiting forever if something goes wrong
        max_count = 250

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
        pattern = r'^(([0-9a-f]{2}\.){11}[0-9a-f]{2}\|([0-9a-f]\.){3}[0-9a-f]{1,})$'
        if not re.match(pattern, guid):

            # check if the guid contains it partitions
            pattern = (
                r'^(([0-9a-f]{2}\.){11}[0-9a-f]{2})\|'
                r'(([0-9a-f]\.){3}[0-9a-f]{1,})\s+\[".*"\]$'
            )
            if not re.match(pattern, guid):
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
        # Just find the number or inf
        pattern = r'^(inf|\d+(\.\d+)?)'

        match = re.match(pattern, rate)

        if match:
            return match.group() == 'inf' or float(match.group()) > 0

        print('Not valid rate: ')
        print(rate)
        return False

    def extract_cli_output(self, output: str) -> str:
        # Remove ANSI escape sequences
        ansi_escape = re.compile(r'\x1B\[[0-?]*[ -/]*[@-~]')
        no_ansi = ansi_escape.sub('', output)

        # Find the last occurrence of '>>'
        last_prompt = no_ansi.rfind('>>')
        if last_prompt == -1:
            return no_ansi

        remainder = no_ansi[last_prompt + 2:]

        # Split the remainder into lines and remove the first line
        lines = remainder.splitlines()
        if len(lines) > 1:
            result = '\n'.join(lines[1:]).strip()
        else:
            result = ''

        return result + '\n'

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
        clean_output = self.extract_cli_output(output)
        expected_output = self.output_command()
        if expected_output == clean_output:
            return True

        lines_expected_output = expected_output.splitlines()
        lines_output = clean_output.splitlines()

        # TODO (Raul): If guid and rate are on the same line this will not work.
        for i in range(len(lines_expected_output)):
            if '%%guid%%' in lines_expected_output[i]:
                start_guid_position = lines_expected_output[i].find('%%guid%%')

                if not self.valid_guid(lines_output[i][start_guid_position:]):
                    return False

            elif '%%rate%%' in lines_expected_output[i]:
                start_rate_position = lines_expected_output[i].find('%%rate%%')

                if not self.valid_rate(lines_output[i][start_rate_position:]):
                    return False

            elif lines_expected_output[i] != lines_output[i]:
                print('Output: ')
                print(clean_output)
                print('Expected output: ')
                print(expected_output)
                return False

        return True

    def stop_tool(self, proc) -> bool:
        """
        @brief Stop the running Spy.

        @param proc: The subprocess object representing the running Spy.
        @return Returns True if the Spy is successfully stopped, False otherwise.
        """
        try:
            proc.communicate(input='exit\n', timeout=10)[0]
        except subprocess.TimeoutExpired:
            proc.kill()

        time.sleep(SLEEP_TIME)

        if not self.is_stop(proc):
            print('ERROR: DDS Spy still running')
            return False

        return True

    def stop_dds(self, proc) -> bool:
        """
        @brief Stop the DDS publisher.

        @param proc: The subprocess object representing the running DDS publisher.
        @return Returns True if the DDS publisher is successfully stopped, False otherwise.
        """
        if self.dds:
            try:
                proc.terminate()
                proc.communicate(timeout=13)
            except subprocess.TimeoutExpired:
                proc.kill()

            time.sleep(SLEEP_TIME)

            if not self.is_stop(proc):
                print('ERROR: DDS Publisher still running')
                return False

        return True

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
