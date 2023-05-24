#include "Gyro.h"
#include "Zumo32U4IMU.h"

using namespace std;

Zumo32U4IMU sensor; 

Gyro::Gyro() {}

int Gyro::getLastError() {
  int lastError = sensor.getLastError();
  return lastError;
}

bool Gyro::getStatus() {
    if (sensor.accDataReady() && 
        sensor.gyroDataReady() && 
        sensor.magDataReady()) {
        return 1;
    } else {
        return 0;
    }
}

bool Gyro::enableDefaultSensorSettings() {
    sensor.enableDefault();
    try { 
        sensor.enableDefault();
        return 1;
    } catch (exception e) { 
        return 0;
    }
    
}

int Gyro::readAcc() {
    sensor.readAcc();
    int read = sensor.a;
    return read;
}

int Gyro::readGyro() {
    int read = sensor.readGyro();
    return read;
}

int Gyro::readMag() {
    int read = sensor.readMag(); 
    return read;
}

bool Gyro::isDriving() {
    if (Gyro::readAcc() > 0) {
        return 1;
    } else {
        return 0;
    }
}

Gyro::~Gyro() {} 






