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

from __future__ import absolute_import
from __future__ import unicode_literals

import zmq

from .packet import Packet


class ZalphaError(RuntimeError):
    pass


class Zalpha(object):

    # Action status
    AC_COMPLETED = 0x00
    AC_IN_PROGRESS = 0x01
    AC_PAUSED = 0x02
    AC_SAFETY_TRIGGERED = 0x03

    # Safety flags
    SF_BUMPER_FRONT = 0x01
    SF_BUMPER_REAR = 0x02
    SF_EMERGENCY_BUTTON = 0x04
    SF_EXTERNAL_INPUT = 0x08
    SF_MOTOR_FAULT = 0x10
    SF_WHEEL_SLIPPAGE = 0x20
    SF_CHARGER_CONNECTED = 0x40
    SF_LASER_FAR_BLOCKED = 0x0100
    SF_LASER_MIDDLE_BLOCKED = 0x0200
    SF_LASER_NEAR_BLOCKED = 0x0400
    SF_LASER_MALFUNCTION = 0x0800

    # Charging flags
    CH_AUTO_MANUAL = 0x01
    CH_CHARGING = 0x02
    CH_BATTERY_FULL = 0x04

    # Error messages
    MSG_RESULT_ERROR_INVALID_COMMAND = 'Invalid parameters in API call.'
    MSG_RESULT_ERROR_BUSY = 'Target is busy.'
    MSG_INVALID_REPLY = 'Invalid reply format.'
    MSG_UNKNOWN_ERROR = 'Unknown error.'
    MSG_CONNECTED = 'Already connected to API server.'
    MSG_DISCONNECTED = 'Disconnected from API server.'

    def __init__(self):
        self.__context = zmq.Context()
        self.__socket = self.__context.socket(zmq.REQ)
        self.__connected = False
        self.__server_url = ''

    def connect(self, server_ip):
        if self.__connected:
            raise ZalphaError(self.MSG_CONNECTED)
        self.__server_url = 'tcp://%s:17167' % server_ip
        self.__socket.connect(self.__server_url)
        self.__connected = True

    def disconnect(self):
        if not self.__connected:
            return
        self.__socket.disconnect(self.__server_url)

    def version_info(self):
        packet = Packet()
        reply = self.__execute_command(packet, Packet.VERSION_INFO)
        reply.s8[Packet.MAX_PAYLOAD - 1] = 0
        return reply.data

    def set_acceleration(self, acceleration, deceleration):
        packet = Packet()
        packet.f[0] = acceleration
        packet.f[1] = deceleration
        self.__check_result(self.__execute_command(packet, Packet.SET_ACCELERATION))

    def get_acceleration(self):
        packet = Packet()
        reply = self.__execute_command(packet, Packet.GET_ACCELERATION)
        return (reply.f[0], reply.f[1])

    def set_target_speed(self, left_speed, right_speed):
        packet = Packet()
        packet.f[0] = left_speed
        packet.f[1] = right_speed
        self.__check_result(self.__execute_command(packet, Packet.SET_TARGET_SPEED))

    def get_target_speed(self):
        packet = Packet()
        reply = self.__execute_command(packet, Packet.GET_TARGET_SPEED)
        return (reply.f[0], reply.f[1])

    def move_straight(self, speed, distance, laser_area):
        packet = Packet()
        packet.f[0] = speed
        packet.f[1] = distance
        packet.u8[8] = laser_area
        self.__check_result(self.__execute_command(packet, Packet.MOVE_STRAIGHT))

    def move_bezier(self, speed, x, y, cp1_x, cp1_y, cp2_x, cp2_y, laser_area):
        packet = Packet()
        packet.f[0] = speed
        packet.f[1] = x
        packet.f[2] = y
        packet.f[3] = cp1_x
        packet.f[4] = cp1_y
        packet.f[5] = cp2_x
        packet.f[6] = cp2_y
        packet.u8[28] = laser_area
        self.__check_result(self.__execute_command(packet, Packet.MOVE_BEZIER))

    def rotate(self, speed, angle, laser_area):
        packet = Packet()
        packet.f[0] = speed
        packet.f[1] = angle
        packet.u8[8] = laser_area
        self.__check_result(self.__execute_command(packet, Packet.ROTATE))

    def get_action_status(self):
        packet = Packet()
        reply = self.__execute_command(packet, Packet.GET_ACTION_STATUS)
        return reply.u8[0]

    def pause_action(self):
        packet = Packet()
        self.__check_result(self.__execute_command(packet, Packet.PAUSE_ACTION))

    def resume_action(self):
        packet = Packet()
        self.__check_result(self.__execute_command(packet, Packet.RESUME_ACTION))

    def stop_action(self):
        packet = Packet()
        self.__check_result(self.__execute_command(packet, Packet.STOP_ACTION))

    def reset_encoder(self):
        packet = Packet()
        self.__check_result(self.__execute_command(packet, Packet.RESET_ENCODER))

    def get_encoder(self):
        packet = Packet()
        reply = self.__execute_command(packet, Packet.GET_ENCODER)
        return (reply.d[0], reply.d[1])

    def get_raw_encoder(self):
        packet = Packet()
        reply = self.__execute_command(packet, Packet.GET_RAW_ENCODER)
        return (reply.s64[0], reply.s64[1])

    def get_safety_flag(self):
        packet = Packet()
        reply = self.__execute_command(packet, Packet.GET_SAFETY_FLAG)
        return reply.u16[0]

    def get_encoder_and_safety_flag(self):
        packet = Packet()
        reply = self.__execute_command(packet, Packet.GET_ENCODER_AND_SAFETY_FLAG)
        return (reply.d[0], reply.d[1], reply.u16[8])

    def get_raw_encoder_and_safety_flag(self):
        packet = Packet()
        reply = self.__execute_command(packet, Packet.GET_RAW_ENCODER_AND_SAFETY_FLAG)
        return (reply.s64[0], reply.s64[1], reply.u16[8])

    def get_battery(self):
        packet = Packet()
        reply = self.__execute_command(packet, Packet.GET_BATTERY)
        return reply.f[0]

    def set_charging(self, enable):
        packet = Packet()
        packet.u8[0] = 1 if enable else 0
        self.__check_result(self.__execute_command(packet, Packet.SET_CHARGING))

    def get_charging(self):
        packet = Packet()
        reply = self.__execute_command(packet, Packet.GET_CHARGING)
        return reply.u8[0]

    def get_inputs(self):
        packet = Packet()
        reply = self.__execute_command(packet, Packet.GET_INPUTS)
        return reply.u32[0]

    def set_outputs(self, outputs, mask):
        packet = Packet()
        packet.u32[0] = outputs
        packet.u32[1] = mask
        self.__check_result(self.__execute_command(packet, Packet.SET_OUTPUTS))

    def get_outputs(self):
        packet = Packet()
        reply = self.__execute_command(packet, Packet.GET_OUTPUTS)
        return reply.u32[0]

    def __execute_command(self, packet, command):
        if not self.__connected:
            raise ZalphaError(self.MSG_DISCONNECTED)

        packet.command = command
        self.__send_request(packet)

        reply_packet = self.__wait_reply()
        if reply_packet.command != command:
            raise ZalphaError(self.MSG_INVALID_REPLY)

        return reply_packet

    def __send_request(self, packet):
        self.__socket.send(packet.raw())

    def __wait_reply(self):
        reply = self.__socket.recv()
        if len(reply) != Packet.SIZE:
            raise ZalphaError(self.MSG_INVALID_REPLY)
        return Packet(reply)

    def __check_result(self, packet):
        if packet.u16[0] == Packet.RESULT_OK:
            return
        elif packet.u16[0] == Packet.RESULT_ERROR_INVALID_COMMAND:
            raise ZalphaError(self.MSG_RESULT_ERROR_INVALID_COMMAND)
        elif packet.u16[0] == Packet.RESULT_ERROR_BUSY:
            raise ZalphaError(self.MSG_RESULT_ERROR_BUSY)
        else:
            raise ZalphaError(self.MSG_UNKNOWN_ERROR)
