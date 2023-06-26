#ifndef LINE_FOLLOWING_H
#define LINE_FOLLOWING_H

#include <Zumo32U4.h>
#include "Gyro.h"

class LineFollowing
{
  public:
    void calibratee();
    LineFollowing();
    bool FollowLine;
    bool runLines;
    bool calibrated;
    int LineSpeed;
    void loopLine();
    void setupLine();

  private:
    Gyro gyro;
    String lastLinePosition;
    int BlackValue;
    unsigned int lineSensorValues[5];
    uint16_t lastLineTime;
    uint16_t lastLinePrintTime;
    Zumo32U4LineSensors lineSensors;
    Zumo32U4Motors motorss;
    int maxleft;
    bool foundleft;
    int maxright;
    bool foundright;
    void printLineData();
    void processLineData(int CornerSpeed);
    void trackLines();
    void checkColorAndResetTracking(uint16_t farleft, uint16_t farright);
    void resetLineTracking();
    void implementLineFollowingStrategy(int CornerSpeed, bool &HasRan);
    void setMotorSpeedsAndLastPosition(int leftSpeed, int rightSpeed, String position, bool &HasRan);
};
#endif
