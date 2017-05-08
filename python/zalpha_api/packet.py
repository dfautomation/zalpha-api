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

import ctypes
import struct


class Packet(object):
    # General
    VERSION_INFO = 0xFA00
    # Differential Base
    SET_ACCELERATION = 0xFA10
    GET_ACCELERATION = 0xFA11
    SET_TARGET_SPEED = 0xFA12
    GET_TARGET_SPEED = 0xFA13
    RESET_ENCODER = 0xFA14
    GET_ENCODER = 0xFA15
    # Safety
    GET_SAFETY_FLAG = 0xFA30
    GET_ENCODER_AND_SAFETY_FLAG = 0xFA31
    # Power
    GET_BATTERY = 0xFA40
    SET_CHARGING = 0xFA41
    GET_CHARGING = 0xFA42
    # IO
    GET_INPUTS = 0xFA50
    SET_OUTPUTS = 0xFA51
    GET_OUTPUTS = 0xFA52
    # Replies
    RESULT_OK = 0xF900
    RESULT_ERROR = 0xF901

    # Sizes
    DATA_OFFSET = 8
    MAX_PAYLOAD = 64  # must be a multiple of 8
    SIZE = DATA_OFFSET + MAX_PAYLOAD

    # Structure
    __packet_t = struct.Struct('%is' % SIZE)
    __command_t = struct.Struct('H')
    __data_t = struct.Struct('%is' % MAX_PAYLOAD)
    __u8_t = struct.Struct('B')
    __s8_t = struct.Struct('b')
    __u16_t = struct.Struct('H')
    __s16_t = struct.Struct('h')
    __u32_t = struct.Struct('I')
    __s32_t = struct.Struct('i')
    __u64_t = struct.Struct('Q')
    __s64_t = struct.Struct('q')
    __f_t = struct.Struct('f')
    __d_t = struct.Struct('d')

    # Byte array
    class ByteArray(object):

        def __init__(self, buf, fmt):
            self.__buf = buf
            self.__fmt = fmt

        def __getitem__(self, key):
            offset = self.__get_offset(key)
            return self.__fmt.unpack_from(self.__buf, offset)[0]

        def __setitem__(self, key, value):
            offset = self.__get_offset(key)
            self.__fmt.pack_into(self.__buf, offset, value)

        def __get_offset(self, key):
            if not isinstance(key, int):
                raise TypeError()
            if key < 0 or key >= Packet.MAX_PAYLOAD // self.__fmt.size:
                raise IndexError()
            return Packet.DATA_OFFSET + self.__fmt.size * key

    def __init__(self, raw=None):
        self.__buffer = ctypes.create_string_buffer(self.SIZE)
        if raw is not None:
            if not isinstance(raw, bytes):
                raise TypeError()
            self.__packet_t.pack_into(self.__buffer, 0, raw)

    def raw(self):
        return self.__buffer

    @property
    def command(self):
        return self.__command_t.unpack_from(self.__buffer)[0]

    @command.setter
    def command(self, value):
        return self.__command_t.pack_into(self.__buffer, 0, value)

    @property
    def data(self):
        return self.__data_t.unpack_from(self.__buffer, self.DATA_OFFSET)[0]

    @data.setter
    def data(self, value):
        return self.__data_t.pack_into(self.__buffer, self.DATA_OFFSET, value)

    @property
    def u8(self):
        if not hasattr(self, '_u8'):
            self._u8 = self.ByteArray(self.__buffer, self.__u8_t)
        return self._u8

    @property
    def s8(self):
        if not hasattr(self, '_s8'):
            self._s8 = self.ByteArray(self.__buffer, self.__s8_t)
        return self._s8

    @property
    def u16(self):
        if not hasattr(self, '_u16'):
            self._u16 = self.ByteArray(self.__buffer, self.__u16_t)
        return self._u16

    @property
    def s16(self):
        if not hasattr(self, '_s16'):
            self._s16 = self.ByteArray(self.__buffer, self.__s16_t)
        return self._s16

    @property
    def u32(self):
        if not hasattr(self, '_u32'):
            self._u32 = self.ByteArray(self.__buffer, self.__u32_t)
        return self._u32

    @property
    def s32(self):
        if not hasattr(self, '_s32'):
            self._s32 = self.ByteArray(self.__buffer, self.__s32_t)
        return self._s32

    @property
    def u64(self):
        if not hasattr(self, '_u64'):
            self._u64 = self.ByteArray(self.__buffer, self.__u64_t)
        return self._u64

    @property
    def s64(self):
        if not hasattr(self, '_s64'):
            self._s64 = self.ByteArray(self.__buffer, self.__s64_t)
        return self._s64

    @property
    def f(self):
        if not hasattr(self, '_f'):
            self._f = self.ByteArray(self.__buffer, self.__f_t)
        return self._f

    @property
    def d(self):
        if not hasattr(self, '_d'):
            self._d = self.ByteArray(self.__buffer, self.__d_t)
        return self._d
