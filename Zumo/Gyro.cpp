#include "Gyro.h"

/**
   @brief Constructor for the Gyro class.
   Initializes private member variables.
*/
Gyro::Gyro() {
  internalGyro.init();
  internalGyro.enableDefault();
}

/**
  @brief Checks if the device is tilted.
  This function reads the accelerometer values and determines if the device is tilted.
  It compares the X-axis acceleration value to predefined thresholds to determine the tilt direction.
  @return true if the device is tilted, false otherwise.
*/
bool Gyro::isHelling() {
  // Read the acceleration data from the internal gyroscope
  internalGyro.readAcc();

  // Calculate the acceleration along the x-axis by dividing the raw value by 100
  int x = internalGyro.a.x / 100;

  // If acceleration in the x direction is greater than 30, interpret this as an upward slope
  if (x > 30) {
    //    Serial.println("Upward slope"); // Can print to the serial monitor for debugging
    return true; // Indicate presence of an upward slope
  }
  // If acceleration in the x direction is less than -30, interpret this as a downward slope
  else if (x < -30) {
    //    Serial.println("Downward slope"); // Can print to the serial monitor for debugging
    return true; // Indicate presence of a downward slope
  }
  else {
    //    Serial.println("No slope"); // Can print to the serial monitor for debugging
    return false; // Indicate no slope detected
  }


}
