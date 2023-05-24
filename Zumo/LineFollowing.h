#ifndef LINE_FOLLOWING_H
#define LINE_FOLLOWING_H

#define NUM_SENSORS 5
#include <Zumo32U4.h>

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
    String lastLinePosition = "Middle";
    int BlackValue = 800;
    unsigned int lineSensorValues[NUM_SENSORS];
    uint16_t lastLineTime = 0;
    uint16_t lastLinePrintTime = 0;
};
#endif
