#include <Zumo32U4.h>
#include "XbeeCommunication.h"
#include "LineFollowing.h" 
#include "Gyro.h" 
XbeeCommunication xbee;
LineFollowing LineFollower;

//Setup to init Serials and to initialize 5 line sensors
void setup() {
  Serial.begin(115200);
  xbee.setup();
  LineFollower.setupLine();
}


void loop() {

  //START OF REMOTE CONTROL
  xbee.loop();
  //END OF REMOTE CONTROL

  //Handle External Functions
  LineFollower.loopLine();
  //End Of External Functions
}
