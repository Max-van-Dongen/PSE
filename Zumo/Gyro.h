#ifndef GYRO_H
#define GYRO_H

#include <Zumo32U4.h>

class Gyro {
  public:
    Gyro();
    bool isHelling();
  private:
    Zumo32U4IMU internalGyro;
};

#endif
