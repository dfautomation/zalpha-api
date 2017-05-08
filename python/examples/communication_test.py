# -*- coding: utf-8 -*-
# Copyright 2017 DF Automation & Robotics Sdn. Bhd.  All rights reserved.
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

from __future__ import unicode_literals

import sys
import time
import zalpha_api

NUM_CYCLES = 1000


if __name__ == '__main__':
    if len(sys.argv) != 2:
        print('Usage: communication_test.py <server_ip_address>')
        sys.exit(0)

    agv = zalpha_api.Zalpha()
    try:
        agv.connect(sys.argv[1])
    except Exception as ex:
        print('Error connecting to API server: %s' % ex)
        sys.exit(1)

    print('Running the commands GET_ENCODER_AND_SAFETY_FLAG and SET_TARGET_SPEED for %s cycles' % NUM_CYCLES)

    # capture start time
    t1 = time.time()

    # perform operation cycles
    for i in range(NUM_CYCLES):
        try:
            left, right, safety_flag = agv.get_encoder_and_safety_flag()
        except Exception as ex:
            print('Failed to read encoder and safety flag: %s' % ex)
            sys.exit(1)

        try:
            agv.set_target_speed(0.0, 0.0)
        except Exception as ex:
            print('Failed to set target speed: %s' % ex)
            sys.exit(1)

    # compute elapsed time
    t2 = time.time()
    elapsed_time = (t2 - t1) * 1000.0

    print('Total elapsed time: %s ms.' % elapsed_time)
    print('Average frequency: %s Hz.' % (NUM_CYCLES * 1000.0 / elapsed_time))
