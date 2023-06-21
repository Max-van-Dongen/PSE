#include "Gyro.h" 

/**
 * @brief Constructor for the Gyro class.
 * Initializes private member variables.
 */
Gyro::Gyro() {
    gyro.init();
    gyro.enableDefault();
}

/**
  @brief Checks if the device is tilted.
  This function reads the accelerometer values and determines if the device is tilted.
  It compares the X-axis acceleration value to predefined thresholds to determine the tilt direction.
  @return true if the device is tilted, false otherwise.
*/
bool Gyro::isHelling() {
    gyro.readAcc();
    int x = gyro.a.x / 100;
    if (x > 30) {
        Serial.println("helling omhoog");
        return true;
    }
    else if (x < -30) {
        Serial.println("helling omlaag");
        return true;
    }
    else {
        //Serial.println("geen helling");
        return false;
    }

}
