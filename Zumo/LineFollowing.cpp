#include "LineFollowing.h"
Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motorss;

LineFollowing::LineFollowing() : FollowLine(false), calibrated(false), lastLineTime(0), lastLinePrintTime(0) {}

void LineFollowing::setupLine() {
  lineSensors.initFiveSensors();
}

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

int maxleft = 0;
bool foundleft = false;
int maxright = 0;
bool foundright = false;
void LineFollowing::loopLine() {

  int CornerSpeed = LineSpeed * .7;
  if (runLines) {
    if ((uint16_t)(millis() - lastLinePrintTime) >= 500)//runt elke 500ms
    {
      lastLinePrintTime = millis();
      Serial1.println("*LSL:" + (String)lineSensorValues[0] + "*");
      Serial1.println("*LSML:" + (String)lineSensorValues[1] + "*");
      Serial1.println("*LSM:" + (String)lineSensorValues[2] + "*");
      Serial1.println("*LSMR:" + (String)lineSensorValues[3] + "*");
      Serial1.println("*LSR:" + (String)lineSensorValues[4] + "*");
    }
    if ((uint16_t)(millis() - lastLineTime) >= 10)//runt elke 200ms TODO: optimize??? 50ms mischien beter als we sneller gaan?
    {
      bool HasRan = false;
      lastLineTime = millis();
      lineSensors.readCalibrated(lineSensorValues);
      if (FollowLine) {
        uint16_t farleft = lineSensorValues[0];
        uint16_t left = lineSensorValues[1];
        uint16_t middle = lineSensorValues[2];
        uint16_t right = lineSensorValues[3];
        uint16_t farright = lineSensorValues[4];
        if (farleft > 50) {
          foundleft = true;
          if (farleft > maxleft) {
            maxleft = farleft;
          }
        }
        
        if (farright > 50) {
          foundright = true;
          if (farright > maxright) {
            maxright = farright;
          }
        }
        
        if ((foundleft && farleft < 50) || (foundright && farright < 50)) {
          if (foundleft && foundright) {
            if (maxleft >= 250 && maxleft <= 285) {//grijs
              Serial1.println("GRIJS");
              Serial1.println("GRIJS");
              Serial1.println("GRIJS");
              motorss.setSpeeds(0, 0);
              delay(2000);
            }
            maxleft = 0;
            foundleft = false;
            foundright = false;
            maxright = 0;

            
          }
          if (foundleft) {
            Serial1.println("!!AA" + (String)maxleft + "");
            if (maxleft >= 50 && maxleft <= 100) {//greer
              Serial1.println("GROEEEEEEEEN");
              Serial1.println("GROEEEEEEEEN");
              Serial1.println("GROEEEEEEEEN");
            }
            maxleft = 0;
            foundleft = false;
          }
          
          if (foundright) {
            Serial1.println("!!AA" + (String)maxright + "");
            if (maxright >= 50 && maxright <= 100) {//greer
              Serial1.println("GROEEEEEEEEN");
              Serial1.println("GROEEEEEEEEN");
              Serial1.println("GROEEEEEEEEN");
            }
            foundright = false;
            maxright = 0;
          }
        }
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
      }
    }
  }
}
