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

import logging
import sys
import time
from utils import *

# Sleep time to let process init and finish
SLEEP_TIME = 2


if __name__ == '__main__':

    args = parse_options()

    # Create a custom logger
    logger = logging.getLogger('SYS_TEST')
    # Create handlers
    l_handler = logging.StreamHandler()
    # Create formatters and add it to handlers
    l_format = '[%(asctime)s][%(name)s][%(levelname)s] %(message)s'
    l_format = logging.Formatter(l_format)
    l_handler.setFormatter(l_format)
    # Add handlers to the logger
    logger.addHandler(l_handler)
    # Set log level
    if args.debug:
        logger.setLevel(logging.DEBUG)
    else:
        logger.setLevel(logging.INFO)

    if args.exe is None:
        logger.error(
            'Executable binary file does not exist or has no '
            'executable permissions.')
        sys.exit(1)

    spy = test_spy_start(args.exe, logger)

    # sleep to let the spy run
    time.sleep(SLEEP_TIME)

    sys.exit(
        test_spy_command(
            spy, 'exit', logger))           # Path to executable
