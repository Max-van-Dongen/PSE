#include <Zumo32U4.h>
#include "XbeeCommunication.h"
#include "LineFollowing.h"
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4Motors motors;
Zumo32U4Buzzer buzzer;
Zumo32U4IMU gyro; 
XbeeCommunication xbee;
LineFollowing LineFollower;



//States
bool runDistance = false;
//End Of States

//Distance Sensor Variables
static uint16_t lastDistanceTime = 0;
bool proxLeftActive;
bool proxFrontActive;
bool proxRightActive;
//End Of Distance Sensor Variables

//gyro vars
int gyroCounter = 0;
//end of gyro vars

//Setup to init Serials and to initialize 5 line sensors
void setup() {
  Serial.begin(115200);
  xbee.setup();
  LineFollower.setupLine();
  Serial1.println("*bt:0*");
  proxSensors.initThreeSensors();
  uint16_t BLevels[] = {1, 2, 4, 9, 15, 23, 32, 42, 55, 70, 85, 100, 120, 135, 150, 170};   // default is { 4, 15, 32, 55, 85, 120 }
  proxSensors.setBrightnessLevels(BLevels, 16);
}


void loop() {

  //START OF REMOTE CONTROL
  xbee.loop();
  //END OF REMOTE CONTROL

  //Handle External Functions
  LineFollower.loopLine();
  //End Of External Functions
}
