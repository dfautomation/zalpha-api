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

#include <iostream>
#include <zalpha_api/zalpha.hpp>

#ifdef _WINDOWS
#include <windows.h>
#else
#include <sys/time.h>
#endif


const int NUM_CYCLES = 1000;


int main(int argc, char** argv)
{
  if (argc != 2)
  {
    std::cout << "Usage: communication_test <server_ip_address>" << std::endl;
    return 0;
  }

  zalpha_api::Zalpha agv;
  if (!agv.connect(argv[1]))
  {
    std::cerr << "Error connecting to API server: " << agv.getErrorMessage() << std::endl;
    return 1;
  }

  std::cout << "Running the commands GET_ENCODER_AND_SAFETY_FLAG and SET_TARGET_SPEED for "
            << NUM_CYCLES << " cycles." << std::endl;

  // capture start time
#ifdef _WINDOWS
  LARGE_INTEGER frequency;
  LARGE_INTEGER t1, t2;
  QueryPerformanceFrequency(&frequency);
  QueryPerformanceCounter(&t1);
#else
  struct timeval t1, t2;
  gettimeofday(&t1, NULL);
#endif

  // perform operation cycles
  double distance_left, distance_right;
  uint16_t safety_flag;
  for (int i = 0; i < NUM_CYCLES; i++)
  {
    if (!agv.getEncoderAndSafetyFlag(distance_left, distance_right, safety_flag))
    {
      std::cerr << "Failed to read encoder and safety flag: " << agv.getErrorMessage() << std::endl;
      return 1;
    }
    if (!agv.setTargetSpeed(0.0f, 0.0f))
    {
      std::cerr << "Failed to set target speed: " << agv.getErrorMessage() << std::endl;
      return 1;
    }
  }

  // compute elapsed time
  double elapsed_time;
#ifdef _WINDOWS
  QueryPerformanceCounter(&t2);
  elapsed_time = (t2.QuadPart - t1.QuadPart) * 1000.0 / frequency.QuadPart;
#else
  gettimeofday(&t2, NULL);
  elapsed_time = (t2.tv_sec - t1.tv_sec) * 1000.0;
  elapsed_time += (t2.tv_usec - t1.tv_usec) / 1000.0;
#endif

  std::cout << "Total elapsed time: " << elapsed_time << " ms." << std::endl;
  std::cout << "Average frequency: " << NUM_CYCLES * 1000.0 / elapsed_time << " Hz." << std::endl;
  return 0;
}
