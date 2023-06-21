#ifndef LINE_FOLLOWING_H
#define LINE_FOLLOWING_H

#include <Zumo32U4.h>
#include <Zumo32U4.h>

class Gyro {
  public:
    Gyro();
    void isHelling();
  private:
    Zumo32U4IMU gyro;
};

#endif
