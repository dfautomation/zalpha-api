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


if sys.version_info[0] < 3:
    user_input = raw_input
else:
    user_input = input


info = """
------------------------------------------------------------------------------
This demo client can perform the following operations:
------------------------------------------------------------------------------
1. Read version info.
2. Set acceleration and deceleration to 2.0 m/s2.
3. Set acceleration and deceleration to 0.1 m/s2.
4. Read acceleration values.
5. Move agv forward at 0.5 m/s for 3 seconds and stop.
6. Move agv backward at 0.3 m/s for 3 seconds and stop.
7. Rotate agv clockwise at 90 deg/s for 3 seconds and stop.
8. Rotate agv anticlockwise at 60 deg/s for 3 seconds and stop.
9. Forward agv in a straight movement to reach 2 m.
10. Reverse agv in a straight movement to reach -1.5 m.
11. Forward agv in a bezier movement to reach 2 m (front) and -1 m (right).
12. Reverse agv in a bezier movement to reach -2 m (rear) and 1 m (left).
13. Rotate agv clockwise for 90 deg.
14. Rotate agv anticlockwise for 180 deg.
15. Get action status.
16. Pause action.
17. Resume action.
18. Stop action.
19. Reset encoder to zero.
20. Read encoder distance.
21. Read encoder count.
22. Read safety flag.
23. Read encoder distance and safety flag.
24. Read encoder count and safety flag.
25. Read battery percentage.
26. Activate charging.
27. Read charging status.
28. Read inputs.
29. Set all outputs to HIGH.
30. Set all outputs to LOW.
31. Read outputs.
32. Exit.
------"""

option_count = 32


def get_option():
    print(info)
    while True:
        option = user_input('Please enter an option (1-32): ')
        try:
            option = int(option)
        except:
            pass
        if isinstance(option, int) and option >= 1 and option <= option_count:
            break
    print('------')
    return option


def version_info(agv):
    print('API client library version: %s' % zalpha_api.__version__)

    try:
        version = agv.version_info()
        print('API server version: %s (%s)' % (version,
            'compatible' if zalpha_api.version_compatible(version) else
            'incompatible'))
    except Exception as ex:
        print('Failed to obtain API server version: %s' % ex)


def set_acceleration(agv, acc):
    try:
        agv.set_acceleration(acc, acc)
        print('Successfully set acceleration to %s m/s2.' % acc)
    except Exception as ex:
        print('Failed to set acceleration: %s' % ex)


def get_acceleration(agv):
    try:
        acc, dec = agv.get_acceleration()
        print('Acceleration: %s m/s2' % acc)
        print('Deceleration: %s m/s2' % dec)
    except Exception as ex:
        print('Failed to read acceleration values: %s' % ex)


def set_speed(agv, left_speed, right_speed, duration):
    try:
        agv.set_target_speed(left_speed, right_speed)
        print('Successfully set target speed to (%s, %s) m/s.' % (
            left_speed, right_speed))
    except Exception as ex:
        print('Failed to set target speed: %s' % ex)
        return

    print('Waiting for %d seconds before stopping.' % duration)
    stop_time = time.time() + duration
    while time.time() < stop_time:
        try:
            agv.set_target_speed(left_speed, right_speed)
        except:
            pass
        time.sleep(0.01)

    try:
        agv.set_target_speed(0.0, 0.0)
        print('Successfully stop AGV.')
    except Exception as ex:
        print('Failed to stop AGV: %s' % ex)


def move_straight(agv, speed, distance, laser_area):
    try:
        agv.move_straight(speed, distance, laser_area)
        print('Successfully start move_straight action.')
    except Exception as ex:
        print('Failed to start move_straight action: %s' % ex)


def move_bezier(agv, speed, x, y, cp1_x, cp1_y, cp2_x, cp2_y, laser_area):
    try:
        agv.move_bezier(speed, x, y, cp1_x, cp1_y, cp2_x, cp2_y, laser_area)
        print('Successfully start move_bezier action.')
    except Exception as ex:
        print('Failed to start move_bezier action: %s' % ex)


def rotate(agv, speed, angle, laser_area):
    try:
        agv.rotate(speed, angle, laser_area)
        print('Successfully start rotate action.')
    except Exception as ex:
        print('Failed to start rotate action: %s' % ex)


def get_action_status(agv):
    try:
        status = agv.get_action_status()
        if status == agv.AC_COMPLETED:
            print('Action status: Completed')
        elif status == agv.AC_IN_PROGRESS:
            print('Action status: In progress')
        elif status == agv.AC_PAUSED:
            print('Action status: Paused')
        elif status == agv.AC_SAFETY_TRIGGERED:
            print('Action status: Safety triggered')
        else:
            print('Action status: Unknown')
    except Exception as ex:
        print('Failed to get action status: %s' % ex)


def pause_action(agv):
    try:
        agv.pause_action()
        print('Successfully pause action.')
    except Exception as ex:
        print('Failed to pause action: %s' % ex)


def resume_action(agv):
    try:
        agv.resume_action()
        print('Successfully resume action.')
    except Exception as ex:
        print('Failed to resume action: %s' % ex)


def stop_action(agv):
    try:
        agv.stop_action()
        print('Successfully stop action.')
    except Exception as ex:
        print('Failed to stop action: %s' % ex)


def reset_encoder(agv):
    try:
        agv.reset_encoder()
        print('Successfully reset encoder to zero.')
    except Exception as ex:
        print('Failed to reset encoder: %s' % ex)


def get_encoder(agv):
    try:
        left_distance, right_distance = agv.get_encoder()
        print('Encoder distance: (%s, %s) meters.' % (left_distance, right_distance))
    except Exception as ex:
        print('Failed to read encoder distance: %s' % ex)


def get_raw_encoder(agv):
    try:
        left_count, right_count = agv.get_raw_encoder()
        print('Encoder count: (%s, %s) pulses.' % (left_count, right_count))
    except Exception as ex:
        print('Failed to read encoder count: %s' % ex)


def print_safety_flag(safety_flag):
    print('Safety flags (Critical):')
    print(' - Bumper front: %s' % bool(safety_flag & zalpha_api.Zalpha.SF_BUMPER_FRONT))
    print(' - Bumper rear: %s' % bool(safety_flag & zalpha_api.Zalpha.SF_BUMPER_REAR))
    print(' - Emergency button: %s' % bool(safety_flag & zalpha_api.Zalpha.SF_EMERGENCY_BUTTON))
    print(' - External input: %s' % bool(safety_flag & zalpha_api.Zalpha.SF_EXTERNAL_INPUT))
    print(' - Motor fault: %s' % bool(safety_flag & zalpha_api.Zalpha.SF_MOTOR_FAULT))
    print(' - Wheel slippage: %s' % bool(safety_flag & zalpha_api.Zalpha.SF_WHEEL_SLIPPAGE))
    print(' - Charger connected: %s' % bool(safety_flag & zalpha_api.Zalpha.SF_CHARGER_CONNECTED))
    print('Safety flags (Non-critical):')
    print(' - Laser far area blocked: %s' % bool(safety_flag & zalpha_api.Zalpha.SF_LASER_FAR_BLOCKED))
    print(' - Laser middle area blocked: %s' % bool(safety_flag & zalpha_api.Zalpha.SF_LASER_MIDDLE_BLOCKED))
    print(' - Laser near area blocked: %s' % bool(safety_flag & zalpha_api.Zalpha.SF_LASER_NEAR_BLOCKED))
    print(' - Laser malfunction: %s' % bool(safety_flag & zalpha_api.Zalpha.SF_LASER_MALFUNCTION))


def get_safety_flag(agv):
    try:
        safety_flag = agv.get_safety_flag()
        print_safety_flag(safety_flag)
    except Exception as ex:
        print('Failed to read safety flag: %s' % ex)


def get_encoder_and_safety_flag(agv):
    try:
        left_distance, right_distance, safety_flag = agv.get_encoder_and_safety_flag()
        print('Encoder distance: (%s, %s) meters.' % (left_distance, right_distance))
        print_safety_flag(safety_flag)
    except Exception as ex:
        print('Failed to read encoder distance and safety flag: %s' % ex)


def get_raw_encoder_and_safety_flag(agv):
    try:
        left_count, right_count, safety_flag = agv.get_raw_encoder_and_safety_flag()
        print('Encoder count: (%s, %s) pulses.' % (left_count, right_count))
        print_safety_flag(safety_flag)
    except Exception as ex:
        print('Failed to read encoder count and safety flag: %s' % ex)


def get_battery(agv):
    try:
        battery = agv.get_battery()
        print('Battery percentage: %s' % battery)
    except Exception as ex:
        print('Failed to read battery percentage: %s' % ex)


def set_charging(agv, enable):
    action = 'activate' if enable else 'deactivate'
    try:
        agv.set_charging(enable)
        print('Successfully %s charging.' % action)
    except Exception as ex:
        print('Failed to %s charging: %s' % (action, ex))


def get_charging(agv):
    try:
        state = agv.get_charging()
        print('Charging status:')
        print(' - Plug type: %s' % ('Manual' if state & zalpha_api.Zalpha.CH_AUTO_MANUAL else 'Auto'))
        print(' - Charging: %s' % bool(state & zalpha_api.Zalpha.CH_CHARGING))
        print(' - Battery full: %s' % bool(state & zalpha_api.Zalpha.CH_BATTERY_FULL))
    except Exception as ex:
        print('Failed to read charging status: %s' % ex)


def format_bits(bits):
    return '{0:08b} {1:08b} {2:08b} {3:08b}'.format(
        bits >> 24 & 0xff,
        bits >> 16 & 0xff,
        bits >> 8 & 0xff,
        bits & 0xff)


def get_inputs(agv):
    try:
        inputs = agv.get_inputs()
        print('Inputs: (MSB) %s (LSB)' % format_bits(inputs))
    except Exception as ex:
        print('Failed to read inputs: %s' % ex)


def set_outputs(agv, outputs, mask):
    try:
        agv.set_outputs(outputs, mask)
        print('Successfully set outputs.')
    except Exception as ex:
        print('Failed to set outputs: %s' % ex)


def get_outputs(agv):
    try:
        outputs = agv.get_outputs()
        print('Outputs: (MSB) %s (LSB)' % format_bits(outputs))
    except Exception as ex:
        print('Failed to read outputs: %s' % ex)


def execute(agv, option):
    if option == 1:
        version_info(agv)

    elif option == 2:
        set_acceleration(agv, 2.0)

    elif option == 3:
        set_acceleration(agv, 0.1)

    elif option == 4:
        get_acceleration(agv)

    elif option == 5:
        set_speed(agv, 0.5, 0.5, 3)

    elif option == 6:
        set_speed(agv, -0.3, -0.3, 3)

    elif option == 7:
        base_width = 0.51
        linear_speed = 90.0 / 180.0 * 3.14159 * base_width / 2.0
        set_speed(agv, linear_speed, -linear_speed, 3)

    elif option == 8:
        base_width = 0.51
        linear_speed = 60.0 / 180.0 * 3.14159 * base_width / 2.0
        set_speed(agv, -linear_speed, linear_speed, 3)

    elif option == 9:
        distance = 2.0
        move_straight(agv, 0.6, distance, 1)

    elif option == 10:
        distance = -1.5
        move_straight(agv, 0.6, distance, 1)

    elif option == 11:
        end = (2, -1)
        cp1 = (1, 0)
        cp2 = (1, -1)
        move_bezier(agv, 0.4, end[0], end[1], cp1[0], cp1[1], cp2[0], cp2[1], 1)

    elif option == 12:
        end = (-2, 1)
        cp1 = (-1, 0)
        cp2 = (-1, 1)
        move_bezier(agv, 0.4, end[0], end[1], cp1[0], cp1[1], cp2[0], cp2[1], 1)

    elif option == 13:
        angle = -90.0 / 180.0 * 3.14159
        rotate(agv, 0.4, angle, 1)

    elif option == 14:
        angle = 180.0 / 180.0 * 3.14159
        rotate(agv, 0.4, angle, 1)

    elif option == 15:
        get_action_status(agv)

    elif option == 16:
        pause_action(agv)

    elif option == 17:
        resume_action(agv)

    elif option == 18:
        stop_action(agv)

    elif option == 19:
        reset_encoder(agv)

    elif option == 20:
        get_encoder(agv)

    elif option == 21:
        get_raw_encoder(agv)

    elif option == 22:
        get_safety_flag(agv)

    elif option == 23:
        get_encoder_and_safety_flag(agv)

    elif option == 24:
        get_raw_encoder_and_safety_flag(agv)

    elif option == 25:
        get_battery(agv)

    elif option == 26:
        set_charging(agv, True)

    elif option == 27:
        get_charging(agv)

    elif option == 28:
        get_inputs(agv)

    elif option == 29:
        set_outputs(agv, 0xffff, 0xffff)

    elif option == 30:
        set_outputs(agv, 0x0000, 0xffff)

    elif option == 31:
        get_outputs(agv)

    user_input('\nPress enter to continue...')
    print('\n')


if __name__ == '__main__':
    if len(sys.argv) != 2:
        print('Usage: demo_client.py <server_ip_address>')
        sys.exit(0)

    agv = zalpha_api.Zalpha()
    try:
        agv.connect(sys.argv[1])
    except Exception as ex:
        print('Error connecting to API server: %s' % ex)
        sys.exit(1)

    while True:
        option = get_option()
        if option == option_count:
            break
        execute(agv, option)

    print('Bye!')
