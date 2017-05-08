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

bool Zalpha::resetEncoder()
{
  return pimpl_->resetEncoder();
}

bool Zalpha::getEncoder(double& left_encoder, double& right_encoder)
{
  return pimpl_->getEncoder(left_encoder, right_encoder);
}

bool Zalpha::getSafetyFlag(uint8_t& safety_flag)
{
  return pimpl_->getSafetyFlag(safety_flag);
}

bool Zalpha::getEncoderAndSafetyFlag(double& left_encoder, double& right_encoder, uint8_t& safety_flag)
{
  return pimpl_->getEncoderAndSafetyFlag(left_encoder, right_encoder, safety_flag);
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
