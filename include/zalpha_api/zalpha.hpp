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

/** @file */
#ifndef ZALPHA_API_ZALPHA_HPP
#define ZALPHA_API_ZALPHA_HPP

#include <memory>
#include <string>

#include <zalpha_api/zalpha_api_export.h>


/**
 * \brief The zalpha_api namespace
 */
namespace zalpha_api
{

/**
 * \brief The version string of this client library.
 */
extern const char* VERSION ZALPHA_API_EXPORT;
/**
 * \brief Checks whether the API server version is compatilble with the client library version.
 *
 * The version number consists of three parts: {major}.{minor}.{patch}.
 * The set of API commands will be similar and compatible as long as the major and minor version numbers remain the same.
 *
 * @param api_server_version The version string of the API server.
 * @return                   A boolean indicating whether this client libary version is compatible.
 */
bool versionCompatible(std::string api_server_version);


/**
 * \brief Internal implementation class
 */
class ZALPHA_API_NO_EXPORT ZalphaImpl;

/**
 * \brief Zalpha is the main class that provides the interface to %Zalpha API.
 */
class ZALPHA_API_EXPORT Zalpha
{
public:
  /**
   * \brief Action status
   *
   * This is the definition of the status code returned from the function getActionStatus().
   */
  enum ActionStatus
  {
    AC_COMPLETED = 0,          ///< Action has been completed
    AC_IN_PROGRESS = 1,        ///< Action is in progress
    AC_PAUSED = 2,             ///< Action is being paused
    AC_SAFETY_TRIGGERED = 3,   ///< Action is blocked by a safety trigger
  };

  /**
   * \brief Safety flags
   *
   * The safety flags indicate what safety are being triggered on the %Zalpha AGV.
   *
   * Multiple safety flag can be triggered at the same time, and each flag occupies a single bit.
   *
   * Bit 15-8 consist of non-critical safety flags (laser near/middle/far blocked & laser malfunction).
   * These flags do not cause the agv to stop.
   *
   * Bit 7-0 consist of critical safety flags (bumper front/rear, emergency button, external input,
   * motor fault, wheel slippage, and charger connected). These flags will stop the agv movement.
   *
   * <table>
   * <tr>
   *   <th colspan="8">Bit 15-8</th><th>&nbsp;</th>
   *   <th colspan="8">Bit 7-0</th>
   * </tr>
   * <tr>
   *   <td>x</td><td>x</td><td>x</td><td>x</td><td>LMF</td><td>LNB</td><td>LMB</td><td>LFB</td><td>&nbsp;</td>
   *   <td>x</td><td>CC</td><td>WS</td><td>MF</td><td>EI</td><td>EB</td><td>BR</td><td>BF</td>
   * </tr>
   * </table>
   *
   * The definition below serves as a bit mask to test for a specific safety flag.
   *
   * For example, the following code checks whether the front bumper is triggered:
   *
   * ~~~{.cpp}
   * bool bumper_front_triggered = (bool)(safety_flag & SF_BUMPER_FRONT);
   * ~~~
   *
   * This representation is used in the results of the functions getSafetyFlag() and getEncoderAndSafetyFlag().
   */
  enum SafetyFlag
  {
    SF_BUMPER_FRONT = 0x01,        ///< Front bumper is triggered
    SF_BUMPER_REAR = 0x02,         ///< Rear bumper is triggered
    SF_EMERGENCY_BUTTON = 0x04,    ///< Emergency button is pressed
    SF_EXTERNAL_INPUT = 0x08,      ///< External safety input is triggered
    SF_MOTOR_FAULT = 0x10,         ///< Motor fault is triggered
    SF_WHEEL_SLIPPAGE = 0x20,      ///< Wheel slippage is detected
    SF_CHARGER_CONNECTED = 0x40,   ///< Charger is plugged in manually
    SF_LASER_FAR_BLOCKED = 0x0100,     ///< Far zone of laser is blocked
    SF_LASER_MIDDLE_BLOCKED = 0x0200,  ///< Middle zone of laser is blocked
    SF_LASER_NEAR_BLOCKED = 0x0400,    ///< Near zone of laser is blocked
    SF_LASER_MALFUNCTION = 0x0800,     ///< Laser has malfunction
  };

  /**
   *  \brief Charging state
   *
   * The charging state is represented by 3 bits.
   *
   * <table>
   * <tr>
   *   <th colspan="8">Bit 7-0</th>
   * </tr>
   * <tr>
   *   <td>x</td><td>x</td><td>x</td><td>x</td><td>x</td><td>BF</td><td>CH</td><td>\f$\bar{A}/M\f$</td>
   * </tr>
   * </table>
   *
   * The definition below serves as a bit mask to test for different states as shown below.
   *
   * For example, the following code checks whether the AGV is charged using auto charger:
   *
   * ~~~{.cpp}
   * bool use_auto_charger = !(bool)(charging_state & CH_AUTO_MANUAL);
   * ~~~
   *
   * This representation is used in the results of the function getCharging().
   */
  enum ChargingState
  {
    CH_AUTO_MANUAL = 0x01,         ///< \f$\bar{A}/M\f$ - Using auto charger (0) or manual charger (1)
    CH_CHARGING = 0x02,            ///< Charging is active
    CH_BATTERY_FULL = 0x04,        ///< Battery is fully charged
  };

public:
  /**
   *  \brief Constructor
   */
  Zalpha();
  virtual ~Zalpha();

  /**
   * \brief Connect to API server.
   * @param server_ip        The %Zalpha AGV's ip address, for eg: "192.168.100.1"
   * @return                 A boolean indicating whether the connection is successful
   */
  bool connect(const std::string& server_ip);
  /**
   * \brief Disconnect from API server.
   */
  void disconnect();

  /**
   * \brief Read the version string of API server.
   * @param version          The variable to store the version string
   * @return                 A boolean indicating whether the operation is successful
   */
  bool versionInfo(std::string& version);
  /**
   * \brief Set the motor acceleration.
   * @param acceleration     The acceleration, specified in \f$ms^{-2}\f$
   * @param deceleration     The deceleration, specified in \f$ms^{-2}\f$
   * @return                 A boolean indicating whether the operation is successful
   */
  bool setAcceleration(float acceleration, float deceleration);
  /**
   * \brief Read the motor acceleration.
   * @param acceleration     The variable to store the acceleration, specified in \f$ms^{-2}\f$
   * @param deceleration     The variable to store the deceleration, specified in \f$ms^{-2}\f$
   * @return                 A boolean indicating whether the operation is successful
   */
  bool getAcceleration(float& acceleration, float& deceleration);
  /**
   * \brief Set the target motor speed.
   *
   * When given a target motor speed, the motor controller will accelerate or decelerate the motor,
   * based on the acceleration value given in setAcceleration(),
   * until the target speed is reached.
   *
   * @param left_speed       The speed of the left motor, specified in \f$ms^{-1}\f$
   * @param right_speed      The speed of the right motor, specified in \f$ms^{-1}\f$
   * @return                 A boolean indicating whether the operation is successful
   */
  bool setTargetSpeed(float left_speed, float right_speed);
  /**
   * \brief Read the target motor speed.
   *
   * Note that this is not the current active speed of the motor.
   * Instead, it is the target speed provided in setTargetSpeed().
   *
   * @param left_speed       The variable to store the speed of the left motor, specified in \f$ms^{-1}\f$
   * @param right_speed      The variable to store the speed of the right motor, specified in \f$ms^{-1}\f$
   * @return                 A boolean indicating whether the operation is successful
   */
  bool getTargetSpeed(float& left_speed, float& right_speed);
  /**
   * \brief Perform a straight movement.
   * @param speed            The maximum speed, specified in \f$ms^{-1}\f$
   * @param distance         The distance to move forward (+ve) or backward (-ve), specified in \f$m\f$
   * @param laser_area       The ID of the laser area to set as the safety zone
   * @return                 A boolean indicating whether the operation is successful
   */
  bool moveStraight(float speed, float distance, uint8_t laser_area);
  /**
   * \brief Perform a bezier movement.
   * @param speed            The maximum speed, specified in \f$ms^{-1}\f$
   * @param x                The X coordinate of endpoint from current position, in \f$m\f$
   * @param y                The Y coordinate of endpoint from current position, in \f$m\f$
   * @param cp1_x            The X coordinate of control point 1 from current position, in \f$m\f$
   * @param cp1_y            The Y coordinate of control point 1 from current position, in \f$m\f$
   * @param cp2_x            The X coordinate of control point 2 from current position, in \f$m\f$
   * @param cp2_y            The Y coordinate of control point 2 from current position, in \f$m\f$
   * @param laser_area       The ID of the laser area to set as the safety zone
   * @return                 A boolean indicating whether the operation is successful
   */
  bool moveBezier(float speed, float x, float y, float cp1_x, float cp1_y, float cp2_x, float cp2_y, uint8_t laser_area);
  /**
   * \brief Perform a rotational movement.
   * @param speed            The maximum rotational speed, specified in \f$rad/s\f$
   * @param angle            The angle to rotate counter-clockwise (+ve) or clockwise (-ve), specified in \f$rad\f$
   * @param laser_area       The ID of the laser area to set as the safety zone
   * @return                 A boolean indicating whether the operation is successful
   */
  bool rotate(float speed, float angle, uint8_t laser_area);
  /**
   * \brief Get the status of the current action.
   *
   * The action refers to the straight, bezier or rotational movement.
   *
   * @param status           The variable to store the status
   * @return                 A boolean indicating whether the operation is successful
   */
  bool getActionStatus(uint8_t& status);
  /**
   * \brief Pause the current action.
   *
   * The action refers to the straight, bezier or rotational movement.
   *
   * @return                 A boolean indicating whether the operation is successful
   */
  bool pauseAction();
  /**
   * \brief Resume the current action.
   *
   * The action refers to the straight, bezier or rotational movement.
   *
   * @return                 A boolean indicating whether the operation is successful
   */
  bool resumeAction();
  /**
   * \brief Abort the current action.
   *
   * The action refers to the straight, bezier or rotational movement.
   *
   * @return                 A boolean indicating whether the operation is successful
   */
  bool stopAction();
  /**
   * \brief Reset the encoder distance and raw encoder count to zero.
   * @return                 A boolean indicating whether the operation is successful
   */
  bool resetEncoder();
  /**
   * \brief Read the current encoder distance
   * @param left_distance    The variable to store the left encoder distance, specified in \f$m\f$
   * @param right_distance   The variable to store the right encoder distance, specified in \f$m\f$
   * @return                 A boolean indicating whether the operation is successful
   */
  bool getEncoder(double& left_distance, double& right_distance);
  /**
   * \brief Read the current raw encoder count
   * @param left_count       The variable to store the left encoder count, specified in pulses
   * @param right_count      The variable to store the right encoder count, specified in pulses
   * @return                 A boolean indicating whether the operation is successful
   */
  bool getRawEncoder(int64_t& left_count, int64_t& right_count);
  /**
   * \brief Read the safety flag
   * @param safety_flag      The variable to store the safety flag.
   * @return                 A boolean indicating whether the operation is successful
   * @sa                     SafetyFlag
   */
  bool getSafetyFlag(uint16_t& safety_flag);
  /**
   * \brief Read the encoder distance and safety flag
   * @param left_distance    The variable to store the left encoder distance, specified in \f$m\f$
   * @param right_distance   The variable to store the right encoder distance, specified in \f$m\f$
   * @param safety_flag      The variable to store the safety flag.
   * @return                 A boolean indicating whether the operation is successful
   * @sa                     SafetyFlag
   */
  bool getEncoderAndSafetyFlag(double& left_distance, double& right_distance, uint16_t& safety_flag);
  /**
   * \brief Read the raw encoder count and safety flag
   * @param left_count       The variable to store the left encoder count, specified in pulses
   * @param right_count      The variable to store the right encoder count, specified in pulses
   * @param safety_flag      The variable to store the safety flag.
   * @return                 A boolean indicating whether the operation is successful
   * @sa                     SafetyFlag
   */
  bool getRawEncoderAndSafetyFlag(int64_t& left_count, int64_t& right_count, uint16_t& safety_flag);
  /**
   * \brief Read the battery percentage
   * @param battery_percentage   The variable to store the battery percentage. The value is in between 0 - 100%.
   * @return                     A boolean indicating whether the operation is successful
   */
  bool getBattery(float& battery_percentage);
  /**
   * \brief Activate or deactivate charging
   * @param enable           When this is true, the internal charging relay will be activated to allow charging. When this is false, the relay will be deactivated.
   * @return                 A boolean indicating whether the operation is successful
   */
  bool setCharging(bool enable = true);
  /**
   * \brief Read the charging status
   * @param charging_state   The variable to store the charging state.
   * @return                 A boolean indicating whether the operation is successful
   * @sa                     ChargingState
   */
  bool getCharging(uint8_t& charging_state);
  /**
   * \brief Read the digital inputs.
   *
   * Currently only 16 inputs are active. The inputs (I0 - I15) are stored in the lower bits.
   *
   * Addtionally, the start and stop button are mapped to the two most significant bits.
   *
   * The remaining bits are reserved and not in use.
   *
   * <table>
   * <tr>
   *   <th colspan="8">Bit 31-24</th><th>&nbsp;</th>
   *   <th colspan="8">Bit 23-16</th><th>&nbsp;</th>
   *   <th colspan="8">Bit 15-8</th><th>&nbsp;</th>
   *   <th colspan="8">Bit 7-0</th>
   * </tr>
   * <tr>
   *   <td>\f$\bar{Stop}\f$</td><td>\f$\bar{Start}\f$</td><td>x</td><td>x</td><td>x</td><td>x</td><td>x</td><td>x</td><td></td>
   *   <td>x</td><td>x</td><td>x</td><td>x</td><td>x</td><td>x</td><td>x</td><td>x</td><td></td>
   *   <td>I15</td><td>I14</td><td>I13</td><td>I12</td><td>I11</td><td>I10</td><td>I9</td><td>I8</td><td></td>
   *   <td>I7</td><td>I6</td><td>I5</td><td>I4</td><td>I3</td><td>I2</td><td>I1</td><td>I0</td>
   * </tr>
   * </table>
   *
   *
   * @param inputs           The variable to store the inputs.
   * @return                 A boolean indicating whether the operation is successful
   */
  bool getInputs(uint32_t& inputs);
  /**
   * \brief Set the digital outputs.
   *
   * Currently only 16 outputs are active. The outputs (O0 - O15) are stored in the lower bits.
   *
   * The remaining bits are reserved and not in use.
   *
   * <table>
   * <tr>
   *   <th colspan="8">Bit 31-24</th><th>&nbsp;</th>
   *   <th colspan="8">Bit 23-16</th><th>&nbsp;</th>
   *   <th colspan="8">Bit 15-8</th><th>&nbsp;</th>
   *   <th colspan="8">Bit 7-0</th>
   * </tr>
   * <tr>
   *   <td>x</td><td>x</td><td>x</td><td>x</td><td>x</td><td>x</td><td>x</td><td>x</td><td></td>
   *   <td>x</td><td>x</td><td>x</td><td>x</td><td>x</td><td>x</td><td>x</td><td>x</td><td></td>
   *   <td>O15</td><td>O14</td><td>O13</td><td>O12</td><td>O11</td><td>O10</td><td>O9</td><td>O8</td><td></td>
   *   <td>O7</td><td>O6</td><td>O5</td><td>O4</td><td>O3</td><td>O2</td><td>O1</td><td>O0</td>
   * </tr>
   * </table>
   *
   * @return                 A boolean indicating whether the operation is successful
   */
  bool setOutputs(uint32_t outputs, uint32_t mask);
  /**
   * \brief Read the digital outputs.
   *
   * Currently only 16 outputs are active. The outputs (O0 - O15) are stored in the lower bits.
   *
   * The remaining bits are reserved and not in use.
   *
   * <table>
   * <tr>
   *   <th colspan="8">Bit 31-24</th><th>&nbsp;</th>
   *   <th colspan="8">Bit 23-16</th><th>&nbsp;</th>
   *   <th colspan="8">Bit 15-8</th><th>&nbsp;</th>
   *   <th colspan="8">Bit 7-0</th>
   * </tr>
   * <tr>
   *   <td>x</td><td>x</td><td>x</td><td>x</td><td>x</td><td>x</td><td>x</td><td>x</td><td></td>
   *   <td>x</td><td>x</td><td>x</td><td>x</td><td>x</td><td>x</td><td>x</td><td>x</td><td></td>
   *   <td>O15</td><td>O14</td><td>O13</td><td>O12</td><td>O11</td><td>O10</td><td>O9</td><td>O8</td><td></td>
   *   <td>O7</td><td>O6</td><td>O5</td><td>O4</td><td>O3</td><td>O2</td><td>O1</td><td>O0</td>
   * </tr>
   * </table>
   *
   * @return                 A boolean indicating whether the operation is successful
   */
  bool getOutputs(uint32_t& outputs);

  /**
   * \brief Get the last error code.
   * @return                 The error code.
   */
  int getError();
  /**
   * \brief Get the last error message.
   *
   * Use this function to obtain a human-readable error message, instead of the error code from getError().
   *
   * @return                 The error message.
   */
  std::string getErrorMessage();

private:
  std::auto_ptr<ZalphaImpl> pimpl_;
};

}  // namespace zalpha_api

#endif  // ZALPHA_API_ZALPHA_HPP
