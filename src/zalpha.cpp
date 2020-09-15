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

#include <zalpha_api/zalpha.hpp>
#include "impl/zalpha_impl.hpp"


namespace zalpha_api
{

const char* VERSION = zalpha_api_VERSION;

bool versionCompatible(std::string api_server_version)
{
  std::string lib_version(VERSION);

  // strip last dot (patch version)
  size_t pos = lib_version.rfind('.');
  if (pos != std::string::npos)
  {
    lib_version.resize(pos);
  }

  // strip last dot (patch version)
  pos = api_server_version.rfind('.');
  if (pos != std::string::npos)
  {
    api_server_version.resize(pos);
  }

  return lib_version == api_server_version;
}

Zalpha::Zalpha() :
  pimpl_(new ZalphaImpl())
{
}

Zalpha::~Zalpha()
{
}

bool Zalpha::connect(const std::string& server_ip)
{
  return pimpl_->connect(server_ip);
}

void Zalpha::disconnect()
{
  return pimpl_->disconnect();
}

bool Zalpha::versionInfo(std::string& version)
{
  return pimpl_->versionInfo(version);
}

bool Zalpha::setAcceleration(float acceleration, float deceleration)
{
  return pimpl_->setAcceleration(acceleration, deceleration);
}

bool Zalpha::getAcceleration(float& acceleration, float& deceleration)
{
  return pimpl_->getAcceleration(acceleration, deceleration);
}

bool Zalpha::setTargetSpeed(float left_speed, float right_speed)
{
  return pimpl_->setTargetSpeed(left_speed, right_speed);
}

bool Zalpha::getTargetSpeed(float& left_speed, float& right_speed)
{
  return pimpl_->getTargetSpeed(left_speed, right_speed);
}

bool Zalpha::moveStraight(float speed, float distance, uint8_t laser_area)
{
  return pimpl_->moveStraight(speed, distance, laser_area);
}

bool Zalpha::moveBezier(float speed, float x, float y, float cp1_x, float cp1_y, float cp2_x, float cp2_y, uint8_t laser_area)
{
  return pimpl_->moveBezier(speed, x, y, cp1_x, cp1_y, cp2_x, cp2_y, laser_area);
}

bool Zalpha::rotate(float speed, float angle, uint8_t laser_area)
{
  return pimpl_->rotate(speed, angle, laser_area);
}

bool Zalpha::getActionStatus(uint8_t& status)
{
  return pimpl_->getActionStatus(status);
}

bool Zalpha::pauseAction()
{
  return pimpl_->pauseAction();
}

bool Zalpha::resumeAction()
{
  return pimpl_->resumeAction();
}

bool Zalpha::stopAction()
{
  return pimpl_->stopAction();
}

bool Zalpha::resetEncoder()
{
  return pimpl_->resetEncoder();
}

bool Zalpha::getEncoder(double& left_distance, double& right_distance)
{
  return pimpl_->getEncoder(left_distance, right_distance);
}

bool Zalpha::getRawEncoder(int64_t& left_count, int64_t& right_count)
{
  return pimpl_->getRawEncoder(left_count, right_count);
}

bool Zalpha::getSafetyFlag(uint16_t& safety_flag)
{
  return pimpl_->getSafetyFlag(safety_flag);
}

bool Zalpha::getEncoderAndSafetyFlag(double& left_distance, double& right_distance, uint16_t& safety_flag)
{
  return pimpl_->getEncoderAndSafetyFlag(left_distance, right_distance, safety_flag);
}

bool Zalpha::getRawEncoderAndSafetyFlag(int64_t& left_count, int64_t& right_count, uint16_t& safety_flag)
{
  return pimpl_->getRawEncoderAndSafetyFlag(left_count, right_count, safety_flag);
}

bool Zalpha::getBattery(float& battery_percentage)
{
  return pimpl_->getBattery(battery_percentage);
}

bool Zalpha::setCharging(bool enable)
{
  return pimpl_->setCharging(enable);
}

bool Zalpha::getCharging(uint8_t& charging_state)
{
  return pimpl_->getCharging(charging_state);
}

bool Zalpha::getInputs(uint32_t& inputs)
{
  return pimpl_->getInputs(inputs);
}

bool Zalpha::setOutputs(uint32_t outputs, uint32_t mask)
{
  return pimpl_->setOutputs(outputs, mask);
}

bool Zalpha::getOutputs(uint32_t& outputs)
{
  return pimpl_->getOutputs(outputs);
}

int Zalpha::getError()
{
  return pimpl_->getError();
}

std::string Zalpha::getErrorMessage()
{
  return pimpl_->getErrorMessage();
}

}  // namespace zalpha_api
