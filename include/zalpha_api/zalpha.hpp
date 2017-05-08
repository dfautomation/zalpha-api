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

#ifndef ZALPHA_API_ZALPHA_HPP
#define ZALPHA_API_ZALPHA_HPP

#include <memory>
#include <string>

#include <zalpha_api/zalpha_api_export.h>


namespace zalpha_api
{

extern const char* VERSION ZALPHA_API_EXPORT;
bool versionCompatible(std::string api_server_version);


class ZALPHA_API_NO_EXPORT ZalphaImpl;

class ZALPHA_API_EXPORT Zalpha
{
public:
  enum
  {
    SF_BUMPER_FRONT = 0x01,
    SF_BUMPER_REAR = 0x02,
    SF_EMERGENCY_BUTTON = 0x04,
    SF_EXTERNAL_INPUT = 0x08,
    SF_CHARGER_CONNECTED = 0x20,
  };

  enum
  {
    CH_AUTO_MANUAL = 0x01,
    CH_CHARGING = 0x02,
    CH_BATTERY_FULL = 0x04,
  };

public:
  Zalpha();
  virtual ~Zalpha();

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

  int getError();
  std::string getErrorMessage();

private:
  std::auto_ptr<ZalphaImpl> pimpl_;
};

}  // namespace zalpha_api

#endif  // ZALPHA_API_ZALPHA_HPP
