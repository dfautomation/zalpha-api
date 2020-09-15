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

#include <sstream>

#include "zalpha_impl.hpp"
#include "packet.hpp"


namespace zalpha_api
{

ZalphaImpl::ZalphaImpl() :
  context_(1),
  socket_(context_, ZMQ_REQ),
  connected_(false), errnum_(0)
{
}

ZalphaImpl::~ZalphaImpl()
{
}

bool ZalphaImpl::connect(const std::string& server_ip)
{
  if (connected_)
  {
    errnum_ = Packet::CONNECTED;
    errmsg_ = "Already connected to API server.";
    return false;
  }

  std::ostringstream oss;
  oss << "tcp://" << server_ip << ":17167";
  server_url_ = oss.str();

  try
  {
    socket_.connect(server_url_.c_str());
  }
  catch (const zmq::error_t& ex)
  {
    errnum_ = ex.num();
    errmsg_ = ex.what();
    return false;
  }

  connected_ = true;
  return true;
}

void ZalphaImpl::disconnect()
{
  if (!connected_) return;

  try
  {
    socket_.disconnect(server_url_.c_str());
  }
  catch (const zmq::error_t& ex)
  {
    errnum_ = ex.num();
    errmsg_ = ex.what();
  }

  connected_ = false;
}

bool ZalphaImpl::versionInfo(std::string& version)
{
  Packet packet;
  if (!executeCommand(packet, Packet::VERSION_INFO))
  {
    return false;
  }
  packet.data.s8[Packet::MAX_PAYLOAD - 1] = 0;
  version = std::string((const char*) &packet.data.s8[0]);
  return true;
}

bool ZalphaImpl::setAcceleration(float acceleration, float deceleration)
{
  Packet packet;
  packet.data.f[0] = acceleration;
  packet.data.f[1] = deceleration;
  return executeCommand(packet, Packet::SET_ACCELERATION) && isResultOk(packet);
}

bool ZalphaImpl::getAcceleration(float& acceleration, float& deceleration)
{
  Packet packet;
  if (!executeCommand(packet, Packet::GET_ACCELERATION))
  {
    return false;
  }
  acceleration = packet.data.f[0];
  deceleration = packet.data.f[1];
  return true;
}

bool ZalphaImpl::setTargetSpeed(float left_speed, float right_speed)
{
  Packet packet;
  packet.data.f[0] = left_speed;
  packet.data.f[1] = right_speed;
  return executeCommand(packet, Packet::SET_TARGET_SPEED) && isResultOk(packet);
}

bool ZalphaImpl::getTargetSpeed(float& left_speed, float& right_speed)
{
  Packet packet;
  if (!executeCommand(packet, Packet::GET_TARGET_SPEED))
  {
    return false;
  }
  left_speed = packet.data.f[0];
  right_speed = packet.data.f[1];
  return true;
}

bool ZalphaImpl::moveStraight(float speed, float distance, uint8_t laser_area)
{
  Packet packet;
  packet.data.f[0] = speed;
  packet.data.f[1] = distance;
  packet.data.u8[8] = laser_area;
  return executeCommand(packet, Packet::MOVE_STRAIGHT) && isResultOk(packet);
}

bool ZalphaImpl::moveBezier(float speed, float x, float y, float cp1_x, float cp1_y, float cp2_x, float cp2_y, uint8_t laser_area)
{
  Packet packet;
  packet.data.f[0] = speed;
  packet.data.f[1] = x;
  packet.data.f[2] = y;
  packet.data.f[3] = cp1_x;
  packet.data.f[4] = cp1_y;
  packet.data.f[5] = cp2_x;
  packet.data.f[6] = cp2_y;
  packet.data.u8[28] = laser_area;
  return executeCommand(packet, Packet::MOVE_BEZIER) && isResultOk(packet);
}

bool ZalphaImpl::rotate(float speed, float angle, uint8_t laser_area)
{
  Packet packet;
  packet.data.f[0] = speed;
  packet.data.f[1] = angle;
  packet.data.u8[8] = laser_area;
  return executeCommand(packet, Packet::ROTATE) && isResultOk(packet);
}

bool ZalphaImpl::getActionStatus(uint8_t& status)
{
  Packet packet;
  if (!executeCommand(packet, Packet::GET_ACTION_STATUS))
  {
    return false;
  }
  status = packet.data.u8[0];
  return true;
}

bool ZalphaImpl::pauseAction()
{
  Packet packet;
  return executeCommand(packet, Packet::PAUSE_ACTION) && isResultOk(packet);
}

bool ZalphaImpl::resumeAction()
{
  Packet packet;
  return executeCommand(packet, Packet::RESUME_ACTION) && isResultOk(packet);
}

bool ZalphaImpl::stopAction()
{
  Packet packet;
  return executeCommand(packet, Packet::STOP_ACTION) && isResultOk(packet);
}

bool ZalphaImpl::resetEncoder()
{
  Packet packet;
  return executeCommand(packet, Packet::RESET_ENCODER) && isResultOk(packet);
}

bool ZalphaImpl::getEncoder(double& left_distance, double& right_distance)
{
  Packet packet;
  if (!executeCommand(packet, Packet::GET_ENCODER))
  {
    return false;
  }
  left_distance = packet.data.d[0];
  right_distance = packet.data.d[1];
  return true;
}

bool ZalphaImpl::getRawEncoder(int64_t& left_count, int64_t& right_count)
{
  Packet packet;
  if (!executeCommand(packet, Packet::GET_RAW_ENCODER))
  {
    return false;
  }
  left_count = packet.data.s64[0];
  right_count = packet.data.s64[1];
  return true;
}

bool ZalphaImpl::getSafetyFlag(uint16_t& safety_flag)
{
  Packet packet;
  if (!executeCommand(packet, Packet::GET_SAFETY_FLAG))
  {
    return false;
  }
  safety_flag = packet.data.u16[0];
  return true;
}

bool ZalphaImpl::getEncoderAndSafetyFlag(double& left_distance, double& right_distance, uint16_t& safety_flag)
{
  Packet packet;
  if (!executeCommand(packet, Packet::GET_ENCODER_AND_SAFETY_FLAG))
  {
    return false;
  }
  left_distance = packet.data.d[0];
  right_distance = packet.data.d[1];
  safety_flag = packet.data.u16[8];
  return true;
}

bool ZalphaImpl::getRawEncoderAndSafetyFlag(int64_t& left_count, int64_t& right_count, uint16_t& safety_flag)
{
  Packet packet;
  if (!executeCommand(packet, Packet::GET_RAW_ENCODER_AND_SAFETY_FLAG))
  {
    return false;
  }
  left_count = packet.data.s64[0];
  right_count = packet.data.s64[1];
  safety_flag = packet.data.u16[8];
  return true;
}

bool ZalphaImpl::getBattery(float& battery_percentage)
{
  Packet packet;
  if (!executeCommand(packet, Packet::GET_BATTERY))
  {
    return false;
  }
  battery_percentage = packet.data.f[0];
  return true;
}

bool ZalphaImpl::setCharging(bool enable)
{
  Packet packet;
  packet.data.u8[0] = enable ? 1 : 0;
  return executeCommand(packet, Packet::SET_CHARGING) && isResultOk(packet);
}

bool ZalphaImpl::getCharging(uint8_t& charging_state)
{
  Packet packet;
  if (!executeCommand(packet, Packet::GET_CHARGING))
  {
    return false;
  }
  charging_state = packet.data.u8[0];
  return true;
}

bool ZalphaImpl::getInputs(uint32_t& inputs)
{
  Packet packet;
  if (!executeCommand(packet, Packet::GET_INPUTS))
  {
    return false;
  }
  inputs = packet.data.u32[0];
  return true;
}

bool ZalphaImpl::setOutputs(uint32_t outputs, uint32_t mask)
{
  Packet packet;
  packet.data.u32[0] = outputs;
  packet.data.u32[1] = mask;
  return executeCommand(packet, Packet::SET_OUTPUTS) && isResultOk(packet);
}

bool ZalphaImpl::getOutputs(uint32_t& outputs)
{
  Packet packet;
  if (!executeCommand(packet, Packet::GET_OUTPUTS))
  {
    return false;
  }
  outputs = packet.data.u32[0];
  return true;
}

bool ZalphaImpl::executeCommand(Packet& packet, uint16_t command)
{
  if (!connected_)
  {
    errnum_ = Packet::DISCONNECTED;
    errmsg_ = "Disconnected from API server.";
    return false;
  }

  packet.command = command;
  if (!sendRequest(packet))
  {
    return false;
  }

  packet.command = 0;
  if (!waitReply(packet))
  {
    return false;
  }
  if (packet.command != command)
  {
    errnum_ = Packet::INVALID_REPLY;
    errmsg_ = "Invalid reply format.";
    return false;
  }
  return true;
}

bool ZalphaImpl::sendRequest(const Packet& packet)
{
  zmq::message_t request(sizeof(Packet));
  const char* src = (const char*) &packet;
  std::copy(src, src + sizeof(Packet), (char*) request.data());

  try
  {
    if (!socket_.send(request))
    {
      throw zmq::error_t();
    }
  }
  catch (const zmq::error_t& ex)
  {
    errnum_ = ex.num();
    errmsg_ = ex.what();
    return false;
  }
  return true;
}

bool ZalphaImpl::waitReply(Packet& packet)
{
  zmq::message_t reply;
  try
  {
    if (!socket_.recv(&reply))
    {
      throw zmq::error_t();
    }
  }
  catch (const zmq::error_t& ex)
  {
    errnum_ = ex.num();
    errmsg_ = ex.what();
    return false;
  }

  if (reply.size() != sizeof(Packet))
  {
    errnum_ = Packet::INVALID_REPLY;
    errmsg_ = "Invalid reply format.";
    return false;
  }

  char* src = (char*) reply.data();
  std::copy(src, src + sizeof(Packet), (char*) &packet);
  return true;
}

bool ZalphaImpl::isResultOk(const Packet& packet)
{
  if (packet.data.u16[0] == Packet::RESULT_OK)
  {
    return true;
  }
  else if (packet.data.u16[0] == Packet::RESULT_ERROR_INVALID_COMMAND)
  {
    errnum_ = Packet::RESULT_ERROR_INVALID_COMMAND;
    errmsg_ = "Invalid parameters in API call.";
  }
  else if (packet.data.u16[0] == Packet::RESULT_ERROR_BUSY)
  {
    errnum_ = Packet::RESULT_ERROR_BUSY;
    errmsg_ = "Target is busy.";
  }
  else
  {
    errnum_ = Packet::UNKNOWN_ERROR;
    errmsg_ = "Unknown error.";
  }
  return false;
}

}  // namespace zalpha_api
