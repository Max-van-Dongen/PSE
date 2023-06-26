#include "LineFollowing.h"

/**
   @brief Constructor for the LineFollowing class.
   Initializes private member variables.
*/
LineFollowing::LineFollowing() : FollowLine(false), calibrated(false), lastLineTime(0), lastLinePrintTime(0), LineSpeed(200), BlackValue(700), lastLinePosition("Middle"), maxleft(0), maxright(0), foundleft(false), foundright(false) {}

/**
   @brief Method to setup line sensors.
   Initializes five line sensors on the Zumo 32U4 robot.
*/
void LineFollowing::setupLine() {
  lineSensors.initFiveSensors();
}

/**
   @brief Calibrate line sensors and motors.
   Uses a fixed routine to calibrate the line sensors and motors based on the environment.
*/
void LineFollowing::calibratee() {
  for (uint16_t i = 0; i < 120; i++)
  {
    if (i > 30 && i <= 90)
    {
      motorss.setSpeeds(-200, 200);
    }
    else
    {
      motorss.setSpeeds(200, -200);
    }

    lineSensors.calibrate();
  }
  motorss.setSpeeds(0, 0);
  calibrated = true;
}




/**
 * @brief Main loop for following lines.
 * 
 * This function is responsible for the top-level control flow in the line
 * following algorithm, including the initial speed setup and conditional
 * checks.
 */
void LineFollowing::loopLine() {
  // Set corner speed to 70% of line speed
  int CornerSpeed = LineSpeed * .7;

  // Check if lines are to be followed
  if (runLines) {
    printLineData();
    processLineData(CornerSpeed);
  }
}

/**
 * @brief Prints line data periodically.
 *
 * This function is used to print the line data every 500ms.
 */
void LineFollowing::printLineData() {
  // Print line data every 500ms
  if ((uint16_t)(millis() - lastLinePrintTime) >= 500) {
    lastLinePrintTime = millis();
  }
}

/**
 * @brief Processes line sensor data periodically.
 *
 * This function processes line sensor data every 10ms and activates line
 * following if necessary. It reads the calibrated line sensor values.
 * @param CornerSpeed Corner speed which is set to 70% of line speed.
 */
void LineFollowing::processLineData(int CornerSpeed) {
  // Process line data every 10ms
  if ((uint16_t)(millis() - lastLineTime) >= 10) {
    bool HasRan = false;
    lastLineTime = millis();
    // Read calibrated line sensor values
    lineSensors.readCalibrated(lineSensorValues);

    // Execute if line following is activated
    if (FollowLine) {
      trackLines();
      implementLineFollowingStrategy(CornerSpeed, HasRan);
    }
  }
}


/**
 * @brief Tracks detected lines based on sensor readings.
 *
 * This function extracts sensor readings for different positions and detects
 * and tracks the far left and right lines.
 */
void LineFollowing::trackLines() {
  // Extract sensor readings for different positions
  uint16_t farleft = lineSensorValues[0];
  uint16_t left = lineSensorValues[1];
  uint16_t middle = lineSensorValues[2];
  uint16_t right = lineSensorValues[3];
  uint16_t farright = lineSensorValues[4];

  // Detect and track the far left line
  if (farleft > 50) {
    foundleft = true;
    if (farleft > maxleft) {
      maxleft = farleft;
    }
  }

  // Detect and track the far right line
  if (farright > 50) {
    foundright = true;
    if (farright > maxright) {
      maxright = farright;
    }
  }
  // Check for specific color (grey) conditions and reset line tracking
  checkColorAndResetTracking(farleft, farright);
}


/**
 * @brief Checks for specific color conditions and resets line tracking.
 *
 * This function checks for specific color (grey and green) conditions,
 * waits until the slope is cleared for grey, and resets line tracking.
 * @param farleft Far left line sensor value.
 * @param farright Far right line sensor value.
 */
void LineFollowing::checkColorAndResetTracking(uint16_t farleft, uint16_t farright) {
  if ((foundleft && farleft < 50) || (foundright && farright < 50)) {
    if (foundleft && foundright) {
      if (maxleft >= 250 && maxleft <= 285) { // grey
        Serial1.println("GREY");
        while (gyro.isHelling()) {} // wait until slope is cleared
        resetLineTracking();
      }
      if (maxleft >= 50 && maxleft <= 100) { // green left
        Serial1.println("GREEN");
        resetLineTracking();
      }
      if (maxright >= 50 && maxright <= 100) { // green right
        Serial1.println("GREEN");
        resetLineTracking();
      }
    }
  }
}


/**
 * @brief Resets line tracking variables.
 *
 * This function is used to reset the variables related to line tracking.
 */
void LineFollowing::resetLineTracking() {
  maxleft = 0;
  foundleft = false;
  foundright = false;
  maxright = 0;
}


/**
 * @brief Implements the strategy for line following.
 *
 * This function implements different strategies based on line positions
 * and sensor readings. The strategies include turning and moving in a straight line.
 * @param CornerSpeed Corner speed which is set to 70% of line speed.
 * @param HasRan Reference to a boolean flag to check if a line following strategy has been implemented.
 */
void LineFollowing::implementLineFollowingStrategy(int CornerSpeed, bool &HasRan) {
  uint16_t middle = lineSensorValues[2];
  uint16_t left = lineSensorValues[1];
  uint16_t right = lineSensorValues[3];

  // Implement different strategies based on line positions and sensor readings
  if (!HasRan && middle > BlackValue && right > BlackValue && left > BlackValue) {
    setMotorSpeedsAndLastPosition(CornerSpeed, 0, "Right", HasRan);
  }

  if (!HasRan && middle > BlackValue && right > BlackValue) {
    setMotorSpeedsAndLastPosition(CornerSpeed, 0, "Right", HasRan);
  }

  if (!HasRan && middle > BlackValue && left > BlackValue) {
    setMotorSpeedsAndLastPosition(0, CornerSpeed, "Left", HasRan);
  }

  if (!HasRan && left > BlackValue) {
    setMotorSpeedsAndLastPosition(0, CornerSpeed, "Left", HasRan);
  }

  if (!HasRan && right > BlackValue) {
    setMotorSpeedsAndLastPosition(CornerSpeed, 0, "Right", HasRan);
  }

  if (!HasRan && middle > BlackValue) {
    setMotorSpeedsAndLastPosition(LineSpeed, LineSpeed, "Middle", HasRan);
  }

  if (!HasRan && left < BlackValue && middle < BlackValue && right < BlackValue) {
    HasRan = true;
    if (lastLinePosition == "Left") {
      setMotorSpeedsAndLastPosition(0, CornerSpeed, "Left", HasRan);
    }
    if (lastLinePosition == "Right") {
      setMotorSpeedsAndLastPosition(CornerSpeed, 0, "Right", HasRan);
    }
    if (lastLinePosition == "Middle") {
      setMotorSpeedsAndLastPosition(200, 200, "Middle", HasRan);
    }
  }
  // The robot is made to continue in its last known path if no line is detected
  // TODO: Optimize the conditions and strategies for better line following
}


/**
 * @brief Sets the motor speeds and updates the last known line position.
 *
 * This function sets the motor speeds and updates the last known line position.
 * @param leftSpeed Desired speed for the left motor.
 * @param rightSpeed Desired speed for the right motor.
 * @param position Position of the detected line ("Left", "Right", "Middle").
 * @param HasRan Reference to a boolean flag to check if a line following strategy has been implemented.
 */
void LineFollowing::setMotorSpeedsAndLastPosition(int leftSpeed, int rightSpeed, String position, bool &HasRan) {
  motorss.setSpeeds(leftSpeed, rightSpeed);
  lastLinePosition = position;
  HasRan = true;
}
