#include "LineFollowing.h"

/**
   @brief Constructor for the LineFollowing class.
   Initializes private member variables.
*/
LineFollowing::LineFollowing() : FollowLine(false), calibrated(false), lastLineTime(0), lastLinePrintTime(0) {}

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
   @brief Main loop for line following logic.
   Contains the core algorithm for line following, sensor readings, motor control, etc.
*/
void LineFollowing::loopLine() {
  // Set corner speed to 70% of line speed
  int CornerSpeed = LineSpeed * .7;

  // Check if lines are to be followed
  if (runLines) {

    // Print line data every 500ms
    if ((uint16_t)(millis() - lastLinePrintTime) >= 500) {
      lastLinePrintTime = millis();
    }

    // Process line data every 10ms
    if ((uint16_t)(millis() - lastLineTime) >= 10) {
      bool HasRan = false;
      lastLineTime = millis();
      // Read calibrated line sensor values
      lineSensors.readCalibrated(lineSensorValues);

      // Execute if line following is activated
      if (FollowLine) {
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

        // Check for specific color (grey) conditions and wait until slope is cleared
        // Reset line tracking when both lines have been found
        if ((foundleft && farleft < 50) || (foundright && farright < 50)) {
          if (foundleft && foundright) {
            if (maxleft >= 250 && maxleft <= 285) {//grey
              Serial1.println("GREY");
              Serial1.println("GREY");
              Serial1.println("GREY");
              while (gyro.isHelling()) {}//wait until slope is cleared
            }
            maxleft = 0;
            foundleft = false;
            foundright = false;
            maxright = 0;
          }

          // Check for specific color (green) conditions on left and reset tracking
          if (foundleft) {
            if (maxleft >= 50 && maxleft <= 100) {//green left
              Serial1.println("GREEN");
              Serial1.println("GREEN");
              Serial1.println("GREEN");
            }
            maxleft = 0;
            foundleft = false;
          }

          // Check for specific color (green) conditions on right and reset tracking
          if (foundright) {
            if (maxright >= 50 && maxright <= 100) {//green right
              Serial1.println("GREEN");
              Serial1.println("GREEN");
              Serial1.println("GREEN");
            }
            foundright = false;
            maxright = 0;
          }
        }

        // Implement different strategies based on line positions and sensor readings
        // The strategies include turning and moving in straight line
        if (!HasRan && middle > BlackValue && right > BlackValue && left > BlackValue) {
          motorss.setSpeeds(CornerSpeed, 0);
          lastLinePosition = "Right";
          HasRan = true;
        }
        if (!HasRan && middle > BlackValue && right > BlackValue) {
          motorss.setSpeeds(CornerSpeed, 0);
          lastLinePosition = "Right";
          HasRan = true;
        }

        if (!HasRan && middle > BlackValue && left > BlackValue) {
          motorss.setSpeeds(0, CornerSpeed);
          lastLinePosition = "Left";
          HasRan = true;
        }
        if (!HasRan && left > BlackValue) {
          motorss.setSpeeds(0, CornerSpeed);
          lastLinePosition = "Left";
          HasRan = true;
        }
        if (!HasRan && right > BlackValue) {
          motorss.setSpeeds(CornerSpeed, 0);
          lastLinePosition = "Right";
          HasRan = true;
        }
        if (!HasRan && middle > BlackValue) {
          motorss.setSpeeds(LineSpeed, LineSpeed);
          lastLinePosition = "Middle";
          HasRan = true;
        }
        if (!HasRan && left < BlackValue && middle < BlackValue && right < BlackValue) {
          HasRan = true;
          if (lastLinePosition == "Left") {
            motorss.setSpeeds(0, CornerSpeed);
          }
          if (lastLinePosition == "Right") {
            motorss.setSpeeds(CornerSpeed, 0);
          }
          if (lastLinePosition == "Middle") {
            motorss.setSpeeds(200, 200);
          }
        }
        // The robot is made to continue in its last known path if no line is detected
        // TODO: Optimize the conditions and strategies for better line following
      }
    }
  }

}
