#include "Gyro.h"
Zumo32U4IMU gyro; 

/**
 * @brief Constructor for the Gyro class.
 * Initializes private member variables.
 */
Gyro::Gyro() {
    sensor.init();
    sensor.enableDefault();
}

/**
  @brief Checks if the device is tilted.
  This function reads the accelerometer values and determines if the device is tilted.
  It compares the X-axis acceleration value to predefined thresholds to determine the tilt direction.
  @return true if the device is tilted, false otherwise.
*/
bool isHelling() {
    gyro.readAcc();
    int x = sensor.a.x / 100;
    if (x > 30) {
        Serial.println("helling omhoog");
        return true;
    }
    else if (x < -30) {
        Serial.println("helling omlaag");
        return truel
    }
    else {
        //Serial.println("geen helling");
        return false;
    }

}