#ifndef LINE_FOLLOWING_H
#define LINE_FOLLOWING_H

#define NUM_SENSORS 5
#include <Zumo32U4.h>
#include "Gyro.h"

class LineFollowing
{
  public:
    void calibratee();
    LineFollowing();
    bool FollowLine = false;
    bool runLines = false;
    bool calibrated = false;
    int LineSpeed = 200;
    void loopLine();
    void setupLine();

  private:
    Gyro gyro;
    String lastLinePosition = "Middle";
    int BlackValue = 700;
    unsigned int lineSensorValues[NUM_SENSORS];
    uint16_t lastLineTime = 0;
    uint16_t lastLinePrintTime = 0;
    Zumo32U4LineSensors lineSensors;
    Zumo32U4Motors motorss;
};
#endif
