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

    # Safety flags
    SF_BUMPER_FRONT = 0x01
    SF_BUMPER_REAR = 0x02
    SF_EMERGENCY_BUTTON = 0x04
    SF_EXTERNAL_INPUT = 0x08
    SF_CHARGER_CONNECTED = 0x20

    # Charging flags
    CH_AUTO_MANUAL = 0x01
    CH_CHARGING = 0x02
    CH_BATTERY_FULL = 0x04

    # Error messages
    MSG_RESULT_ERROR = 'Invalid parameters in API call.'
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

    def reset_encoder(self):
        packet = Packet()
        self.__check_result(self.__execute_command(packet, Packet.RESET_ENCODER))

    def get_encoder(self):
        packet = Packet()
        reply = self.__execute_command(packet, Packet.GET_ENCODER)
        return (reply.d[0], reply.d[1])

    def get_safety_flag(self):
        packet = Packet()
        reply = self.__execute_command(packet, Packet.GET_SAFETY_FLAG)
        return reply.u8[0]

    def get_encoder_and_safety_flag(self):
        packet = Packet()
        reply = self.__execute_command(packet, Packet.GET_ENCODER_AND_SAFETY_FLAG)
        return (reply.d[0], reply.d[1], reply.u8[16])

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
        elif packet.u16[0] == Packet.RESULT_ERROR:
            raise ZalphaError(self.MSG_RESULT_ERROR)
        else:
            raise ZalphaError(self.MSG_UNKNOWN_ERROR)
