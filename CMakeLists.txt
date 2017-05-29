#
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
#

cmake_minimum_required(VERSION 2.8.3)
project(zalpha_api)

set(zalpha_api_VERSION_MAJOR 0)
set(zalpha_api_VERSION_MINOR 2)
set(zalpha_api_VERSION_PATCH 0)
set(zalpha_api_VERSION "${zalpha_api_VERSION_MAJOR}.${zalpha_api_VERSION_MINOR}.${zalpha_api_VERSION_PATCH}")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(GenerateExportHeader)


###########
## Build ##
###########

find_package(ZMQ REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++0x")

add_definitions("-Dzalpha_api_VERSION=\"${zalpha_api_VERSION}\"")

include_directories(
  include
  ${CMAKE_CURRENT_BINARY_DIR}
  ${ZMQ_INCLUDE_DIRS})

set(zalpha_api_srcs
  ${CMAKE_CURRENT_BINARY_DIR}/zalpha_api/zalpha_api_export.h
  include/zalpha_api/zalpha.hpp
  src/impl/packet.hpp
  src/impl/zalpha_impl.cpp
  src/impl/zalpha_impl.hpp
  src/zalpha.cpp)

add_library(zalpha_api ${zalpha_api_srcs})
generate_export_header(zalpha_api EXPORT_FILE_NAME ${CMAKE_CURRENT_BINARY_DIR}/zalpha_api/zalpha_api_export.h)
target_link_libraries(zalpha_api ${ZMQ_LIBRARIES})

add_subdirectory(examples)
add_subdirectory(doc)


#############
## Install ##
#############

install(TARGETS zalpha_api DESTINATION lib)

install(DIRECTORY include DESTINATION . FILES_MATCHING PATTERN "*.h" PATTERN "*.hpp")

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/zalpha_api/zalpha_api_export.h DESTINATION include/zalpha_api)

install(FILES LICENSE README.md DESTINATION .)

include(CPackConfig)
