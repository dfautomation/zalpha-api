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

#ifndef ZALPHA_API_IMPL_ZALPHA_IMPL_HPP
#define ZALPHA_API_IMPL_ZALPHA_IMPL_HPP

#include <zmq.hpp>

#include <zalpha_api/zalpha_api_export.h>


namespace zalpha_api
{

class ZALPHA_API_NO_EXPORT Packet;

class ZALPHA_API_NO_EXPORT ZalphaImpl
{
public:
  ZalphaImpl();
  virtual ~ZalphaImpl();

  bool connect(const std::string& server_ip);
  void disconnect();

  bool versionInfo(std::string& version);
  bool setAcceleration(float acceleration, float deceleration);
  bool getAcceleration(float& acceleration, float& deceleration);
  bool setTargetSpeed(float left_speed, float right_speed);
  bool getTargetSpeed(float& left_speed, float& right_speed);
  bool resetEncoder();
  bool getEncoder(double& left_encoder, double& right_encoder);
  bool getSafetyFlag(uint8_t& safety_flag);
  bool getEncoderAndSafetyFlag(double& left_encoder, double& right_encoder, uint8_t& safety_flag);
  bool getBattery(float& battery_percentage);
  bool setCharging(bool enable = true);
  bool getCharging(uint8_t& charging_state);
  bool getInputs(uint32_t& inputs);
  bool setOutputs(uint32_t outputs, uint32_t mask);
  bool getOutputs(uint32_t& outputs);

  int getError()
  {
    return errnum_;
  }
  std::string getErrorMessage()
  {
    return errmsg_;
  }

private:
  bool executeCommand(Packet& packet, uint16_t command);
  bool sendRequest(const Packet& packet);
  bool waitReply(Packet& packet);
  bool isResultOk(const Packet& packet);

private:
  zmq::context_t context_;
  zmq::socket_t socket_;

  bool connected_;
  std::string server_url_;

  int errnum_;
  std::string errmsg_;
};

}  // namespace zalpha_api

#endif  // ZALPHA_API_IMPL_ZALPHA_IMPL_HPP
