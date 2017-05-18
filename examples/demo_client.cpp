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

#include <bitset>
#include <ctime>
#include <iostream>
#include <zalpha_api/zalpha.hpp>

#ifdef _WINDOWS
#include <windows.h>
#else
#include <unistd.h>
#define Sleep(x) usleep((x)*1000)
#endif


const char* info =
  "------------------------------------------------------------------------------\n"
  "This demo client can perform the following operations:\n"
  "------------------------------------------------------------------------------\n"
  "1. Read version info.\n"
  "2. Set acceleration and deceleration to 2.0 m/s2.\n"
  "3. Set acceleration and deceleration to 0.1 m/s2.\n"
  "4. Read acceleration values.\n"
  "5. Move agv forward at 0.5 m/s for 3 seconds and stop.\n"
  "6. Move agv backward at 0.3 m/s for 3 seconds and stop.\n"
  "7. Rotate agv clockwise at 90 deg/s for 3 seconds and stop.\n"
  "8. Rotate agv anticlockwise at 60 deg/s for 3 seconds and stop.\n"
  "9. Reset encoder to zero.\n"
  "10. Read encoder distance.\n"
  "11. Read encoder count.\n"
  "12. Read safety flag.\n"
  "13. Read encoder distance and safety flag.\n"
  "14. Read encoder count and safety flag.\n"
  "15. Read battery percentage.\n"
  "16. Activate charging.\n"
  "17. Read charging status.\n"
  "18. Read inputs.\n"
  "19. Set all outputs to HIGH.\n"
  "20. Set all outputs to LOW.\n"
  "21. Read outputs.\n"
  "22. Exit.\n"
  "------";

int getOption()
{
  std::cout << info << std::endl;
  int option;
  do
  {
    option = -1;
    std::cout << "Please enter an option (1-22): ";
    std::cin >> option;
    std::cin.clear();
    std::cin.ignore(10000, '\n');
  }
  while (option < 1 || option > 22);

  std::cout << "------" << std::endl;
  return option;
}

void versionInfo(zalpha_api::Zalpha& agv)
{
  std::cout << "API client library version: " << zalpha_api::VERSION << std::endl;

  std::string version;
  if (agv.versionInfo(version))
  {
    std::cout << "API server version: " << version << " (";
    std::cout << (zalpha_api::versionCompatible(version) ? "compatible" : "incompatible");
    std::cout << ")" << std::endl;
  }
  else
  {
    std::cerr << "Failed to obtain API server version: " << agv.getErrorMessage() << std::endl;
  }
}

void setAcceleration(zalpha_api::Zalpha& agv, float acc)
{
  if (agv.setAcceleration(acc, acc))
  {
    std::cout << "Successfully set acceleration to " << acc << " m/s2." << std::endl;
  }
  else
  {
    std::cerr << "Failed to set acceleration: " << agv.getErrorMessage() << std::endl;
  }
}

void getAcceleration(zalpha_api::Zalpha& agv)
{
  float acc, dec;
  if (agv.getAcceleration(acc, dec))
  {
    std::cout << "Acceleration: " << acc << " m/s2" << std::endl;
    std::cout << "Deceleration: " << dec << " m/s2" << std::endl;
  }
  else
  {
    std::cerr << "Failed to read acceleration values: " << agv.getErrorMessage() << std::endl;
  }
}

void setSpeed(zalpha_api::Zalpha& agv, float left_speed, float right_speed, int duration)
{
  if (agv.setTargetSpeed(left_speed, right_speed))
  {
    std::cout << "Successfully set target speed to (" << left_speed << ", " << right_speed << ") m/s." << std::endl;
  }
  else
  {
    std::cerr << "Failed to set target speed: " << agv.getErrorMessage() << std::endl;
    return;
  }

  std::cout << "Waiting for " << duration << " seconds before stopping." << std::endl;
  time_t stop_time = time(NULL) + duration;
  while (time(NULL) < stop_time)
  {
    agv.setTargetSpeed(left_speed, right_speed);
    Sleep(10);
  }

  if (agv.setTargetSpeed(0.0, 0.0))
  {
    std::cout << "Successfully stop AGV." << std::endl;
  }
  else
  {
    std::cerr << "Failed to stop AGV: " << agv.getErrorMessage() << std::endl;
  }
}

void resetEncoder(zalpha_api::Zalpha& agv)
{
  if (agv.resetEncoder())
  {
    std::cout << "Successfully reset encoder to zero." << std::endl;
  }
  else
  {
    std::cerr << "Failed to reset encoder: " << agv.getErrorMessage() << std::endl;
  }
}

void getEncoder(zalpha_api::Zalpha& agv)
{
  double left_distance, right_distance;
  if (agv.getEncoder(left_distance, right_distance))
  {
    std::cout << "Encoder distance: (" << left_distance << ", " << right_distance << ") meters." << std::endl;
  }
  else
  {
    std::cerr << "Failed to read encoder distance: " << agv.getErrorMessage() << std::endl;
  }
}

void getRawEncoder(zalpha_api::Zalpha& agv)
{
  int64_t left_count, right_count;
  if (agv.getRawEncoder(left_count, right_count))
  {
    std::cout << "Encoder count: (" << left_count << ", " << right_count << ") pulses." << std::endl;
  }
  else
  {
    std::cerr << "Failed to read encoder count: " << agv.getErrorMessage() << std::endl;
  }
}

void printSafetyFlag(uint8_t safety_flag)
{
  std::cout << "Safety flags:" << std::endl;
  std::cout << " - Bumper front: " << (bool)(safety_flag & zalpha_api::Zalpha::SF_BUMPER_FRONT) << std::endl;
  std::cout << " - Bumper rear: " << (bool)(safety_flag & zalpha_api::Zalpha::SF_BUMPER_REAR) << std::endl;
  std::cout << " - Emergency button: " << (bool)(safety_flag & zalpha_api::Zalpha::SF_EMERGENCY_BUTTON) << std::endl;
  std::cout << " - External input: " << (bool)(safety_flag & zalpha_api::Zalpha::SF_EXTERNAL_INPUT) << std::endl;
  std::cout << " - Charger connected: " << (bool)(safety_flag & zalpha_api::Zalpha::SF_CHARGER_CONNECTED) << std::endl;
}

void getSafetyFlag(zalpha_api::Zalpha& agv)
{
  uint8_t safety_flag;
  if (agv.getSafetyFlag(safety_flag))
  {
    printSafetyFlag(safety_flag);
  }
  else
  {
    std::cerr << "Failed to read safety flag: " << agv.getErrorMessage() << std::endl;
  }
}

void getEncoderAndSafetyFlag(zalpha_api::Zalpha& agv)
{
  double left_distance, right_distance;
  uint8_t safety_flag;
  if (agv.getEncoderAndSafetyFlag(left_distance, right_distance, safety_flag))
  {
    std::cout << "Encoder distance: (" << left_distance << ", " << right_distance << ") meters." << std::endl;
    printSafetyFlag(safety_flag);
  }
  else
  {
    std::cerr << "Failed to read encoder distance and safety flag: " << agv.getErrorMessage() << std::endl;
  }
}

void getRawEncoderAndSafetyFlag(zalpha_api::Zalpha& agv)
{
  int64_t left_count, right_count;
  uint8_t safety_flag;
  if (agv.getRawEncoderAndSafetyFlag(left_count, right_count, safety_flag))
  {
    std::cout << "Encoder count: (" << left_count << ", " << right_count << ") pulses." << std::endl;
    printSafetyFlag(safety_flag);
  }
  else
  {
    std::cerr << "Failed to read encoder count and safety flag: " << agv.getErrorMessage() << std::endl;
  }
}

void getBattery(zalpha_api::Zalpha& agv)
{
  float battery;
  if (agv.getBattery(battery))
  {
    std::cout << "Battery percentage: " << battery << std::endl;
  }
  else
  {
    std::cerr << "Failed to read battery percentage: " << agv.getErrorMessage() << std::endl;
  }
}

void setCharging(zalpha_api::Zalpha& agv, bool enable)
{
  const char* action = (enable) ? "activate" : "deactivate";

  if (agv.setCharging(enable))
  {
    std::cout << "Successfully " << action << " charging." << std::endl;
  }
  else
  {
    std::cerr << "Failed to " << action << " charging: " << agv.getErrorMessage() << std::endl;
  }
}

void getCharging(zalpha_api::Zalpha& agv)
{
  uint8_t state;
  if (agv.getCharging(state))
  {
    std::cout << "Charging status:" << std::endl;
    std::cout << " - Plug type: ";
    std::cout << (state & zalpha_api::Zalpha::CH_AUTO_MANUAL ? "Manual" : "Auto") << std::endl;
    std::cout << " - Charging: " << (bool)(state & zalpha_api::Zalpha::CH_CHARGING) << std::endl;
    std::cout << " - Battery full: " << (bool)(state & zalpha_api::Zalpha::CH_BATTERY_FULL) << std::endl;
  }
  else
  {
    std::cerr << "Failed to read charging status: " << agv.getErrorMessage() << std::endl;
  }
}

void printBits(uint32_t bits)
{
  std::cout << std::bitset<8>(bits >> 24) << " ";
  std::cout << std::bitset<8>(bits >> 16) << " ";
  std::cout << std::bitset<8>(bits >> 8) << " ";
  std::cout << std::bitset<8>(bits);
}

void getInputs(zalpha_api::Zalpha& agv)
{
  uint32_t inputs;
  if (agv.getInputs(inputs))
  {
    std::cout << "Inputs: (MSB) ";
    printBits(inputs);
    std::cout << " (LSB)" << std::endl;
  }
  else
  {
    std::cerr << "Failed to read inputs: " << agv.getErrorMessage() << std::endl;
  }
}

void setOutputs(zalpha_api::Zalpha& agv, uint32_t outputs, uint32_t mask)
{
  if (agv.setOutputs(outputs, mask))
  {
    std::cout << "Successfully set outputs." << std::endl;
  }
  else
  {
    std::cerr << "Failed to set outputs: " << agv.getErrorMessage() << std::endl;
  }
}

void getOutputs(zalpha_api::Zalpha& agv)
{
  uint32_t outputs;
  if (agv.getOutputs(outputs))
  {
    std::cout << "Outputs: (MSB) ";
    printBits(outputs);
    std::cout << " (LSB)" << std::endl;
  }
  else
  {
    std::cerr << "Failed to read outputs: " << agv.getErrorMessage() << std::endl;
  }
}


void execute(zalpha_api::Zalpha& agv, int option)
{
  switch (option)
  {
  case 1:
    versionInfo(agv);
    break;
  case 2:
    setAcceleration(agv, 2.0f);
    break;
  case 3:
    setAcceleration(agv, 0.1f);
    break;
  case 4:
    getAcceleration(agv);
    break;
  case 5:
    setSpeed(agv, 0.5f, 0.5f, 3);
    break;
  case 6:
    setSpeed(agv, -0.3f, -0.3f, 3);
    break;
  case 7:
  {
    const float baseWidth = 0.51f;
    float linearSpeed = 90 / 180.0 * 3.14159 * baseWidth / 2.0;
    setSpeed(agv, linearSpeed, -linearSpeed, 3);
    break;
  }
  case 8:
  {
    const float baseWidth = 0.51f;
    float linearSpeed = 60 / 180.0 * 3.14159 * baseWidth / 2.0;
    setSpeed(agv, -linearSpeed, linearSpeed, 3);
    break;
  }
  case 9:
    resetEncoder(agv);
    break;
  case 10:
    getEncoder(agv);
    break;
  case 11:
    getRawEncoder(agv);
    break;
  case 12:
    getSafetyFlag(agv);
    break;
  case 13:
    getEncoderAndSafetyFlag(agv);
    break;
  case 14:
    getRawEncoderAndSafetyFlag(agv);
    break;
  case 15:
    getBattery(agv);
    break;
  case 16:
    setCharging(agv, true);
    break;
  case 17:
    getCharging(agv);
    break;
  case 18:
    getInputs(agv);
    break;
  case 19:
    setOutputs(agv, 0xff, 0xff);
    break;
  case 20:
    setOutputs(agv, 0x00, 0xff);
    break;
  case 21:
    getOutputs(agv);
    break;
  }

  std::cout << std::endl << "Press enter to continue...";
  std::cin.ignore(10000, '\n');
  std::cout << std::endl << std::endl;
}

int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cout << "Usage: demo_client <server_ip_address>" << std::endl;
    return 0;
  }

  zalpha_api::Zalpha agv;
  if (!agv.connect(argv[1]))
  {
    std::cerr << "Error connecting to API server: " << agv.getErrorMessage() << std::endl;
    return 1;
  }

  while (true)
  {
    int option = getOption();
    if (option == 22) break;
    execute(agv, option);
  }

  std::cout << "Bye!" << std::endl;
  return 0;
}
