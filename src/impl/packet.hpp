/*
 * Copyright 2017 DF Automation & Robotics Sdn. Bhd.  All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef ZALPHA_API_IMPL_PACKET_HPP
#define ZALPHA_API_IMPL_PACKET_HPP

#include <zalpha_api/zalpha_api_export.h>


namespace zalpha_api
{

class ZALPHA_API_NO_EXPORT Packet
{
public:
  enum
  {
    /* General */
    VERSION_INFO = 0xFA00,
    /* Differential Base */
    SET_ACCELERATION = 0xFA10,
    GET_ACCELERATION,
    SET_TARGET_SPEED,
    GET_TARGET_SPEED,
    RESET_ENCODER,
    GET_ENCODER,
    /* Safety */
    GET_SAFETY_FLAG = 0xFA30,
    GET_ENCODER_AND_SAFETY_FLAG,
    /* Power */
    GET_BATTERY = 0xFA40,
    SET_CHARGING,
    GET_CHARGING,
    /* IO */
    GET_INPUTS = 0xFA50,
    SET_OUTPUTS,
    GET_OUTPUTS,
    /* Replies */
    RESULT_OK = 0xF900,
    RESULT_ERROR = 0xF901,
    INVALID_REPLY = 0xF910,
    UNKNOWN_ERROR = 0xF911,
    CONNECTED = 0xF920,
    DISCONNECTED = 0xF921,
  };
  enum
  {
    MAX_PAYLOAD = 64,  // must be a multiple of 8.
  };

public:
  uint16_t command;
  uint16_t reserved[3];
  union
  {
    uint8_t u8[MAX_PAYLOAD];
    int8_t s8[MAX_PAYLOAD];
    uint16_t u16[MAX_PAYLOAD / 2];
    int16_t s16[MAX_PAYLOAD / 2];
    uint32_t u32[MAX_PAYLOAD / 4];
    int32_t s32[MAX_PAYLOAD / 4];
    uint64_t u64[MAX_PAYLOAD / 8];
    int64_t s64[MAX_PAYLOAD / 8];
    float f[MAX_PAYLOAD / 4];
    double d[MAX_PAYLOAD / 8];
  } data;
} __attribute__((__packed__));

}  // namespace zalpha_api

#endif  // ZALPHA_API_IMPL_PACKET_HPP