^^^^^^^^^^^^^^^^^^^^^^^^
Changelog for zalpha-api
^^^^^^^^^^^^^^^^^^^^^^^^

0.3.0 (2020-09-15)
------------------
* add the following set of API commands:

  - MOVE_STRAIGHT
  - MOVE_BEZIER
  - ROTATE
  - GET_ACTION_STATUS
  - PAUSE_ACTION
  - RESUME_ACTION
  - STOP_ACTION

* add motor fault and wheel slippage to the safety flags
* increase the number of I/O pins supported from 8 to 16

0.2.0 (2017-05-30)
------------------
* add the following set of API commands:

  - GET_RAW_ENCODER
  - GET_RAW_ENCODER_AND_SAFETY_FLAG

* add laser activation states to the safety flags

0.1.0 (2017-05-11)
------------------
* first public release
* implement the following set of API commands:

  - VERSION_INFO
  - SET_ACCELERATION
  - GET_ACCELERATION
  - SET_TARGET_SPEED
  - GET_TARGET_SPEED
  - RESET_ENCODER
  - GET_ENCODER
  - GET_SAFETY_FLAG
  - GET_ENCODER_AND_SAFETY_FLAG
  - GET_BATTERY
  - SET_CHARGING
  - GET_CHARGING
  - GET_INPUTS
  - SET_OUTPUTS
  - GET_OUTPUTS
