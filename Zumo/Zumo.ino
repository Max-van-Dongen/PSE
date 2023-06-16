#include <Zumo32U4.h>
#include "XbeeCommunication.h"
#include "LineFollowing.h"
Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4Motors motors;
Zumo32U4Buzzer buzzer;
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



struct ArgVal {
  String arg;
  String val;
};


//Function that crashes the arduino, used to hard reset the program in case of an unexpected state or error
void(* resetFunc) (void) = 0;

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



//Function to write proximity data out to serial
void printReadingsToSerial()
{
//  Serial1.println("*PSL:" + (String)proxSensors.countsLeftWithLeftLeds() + "*");
//  Serial1.println("*PSMLL:" + (String)proxSensors.countsLeftWithRightLeds() + "*");
  Serial1.println("*PSML:" + (String)proxSensors.countsFrontWithLeftLeds() + "*");
  Serial1.println("*PSMR:" + (String)proxSensors.countsFrontWithRightLeds() + "*");
//  Serial1.println("*PSMRR:" + (String)proxSensors.countsRightWithLeftLeds() + "*");
//  Serial1.println("*PSR:" + (String)proxSensors.countsRightWithRightLeds() + "*");

}
//End Of Proximity Function


//START OF DISTANCE SENSORS
void DistanceTracking() {
  //(gestolen van martin)
  if (runDistance) {
    if ((uint16_t)(millis() - lastDistanceTime) >= 500)//runt elke 500ms
    {
      lastDistanceTime = millis();
      proxSensors.read();
      proxLeftActive = proxSensors.readBasicLeft();
      proxFrontActive = proxSensors.readBasicFront();
      proxRightActive = proxSensors.readBasicRight();
      printReadingsToSerial();
    }
  }
}
//END OF DISTANCE SENSORS

//INCOMING MESSAGES
int ctr = 0;
void InternalFunctions(String &var, String &arg) {
  if (var == "AAA") {
    buzzer.play("g32");
    delay(arg.toInt());
    buzzer.play("e32");

  }
  if (var == "BBB") {
    buzzer.play("e32");
    delay(arg.toInt());
    buzzer.play("f32");
  }
  if (var == "w") {
    motors.setSpeeds(arg.toInt(), arg.toInt());
  }
  if (var == "a") {
    motors.setSpeeds(-arg.toInt(), arg.toInt());
  }
  if (var == "s") {
    motors.setSpeeds(-arg.toInt(), -arg.toInt());
  }
  if (var == "d") {
    motors.setSpeeds(arg.toInt(), -arg.toInt());
  }
  if (var == "e") {
    motors.setSpeeds(0, 0);
  }
  if (var == "ls") {//LeftSpeed
    motors.setLeftSpeed(arg.toInt()?arg.toInt():0);
  }
  if (var == "rs") {//RightSpeed
    motors.setRightSpeed(arg.toInt()?arg.toInt():0);
  }
  if (var == "hb") {//HeartBeat
    Serial1.println("*hb:" + ((String)ctr) + "*");
    ctr++;
  }
  if (var == "init") {//Init Sensors
    Serial1.println("*initls:" + ((String)LineFollower.runLines) + "*"); //Init Line Sensors
    Serial1.println("*initps:" + ((String)runDistance) + "*"); //Init Prox Sensors
    Serial1.println("*initfls:" + ((String)LineFollower.FollowLine) + "*"); //Init Prox Sensors
    Serial1.println("*initclb:" + ((String)LineFollower.calibrated) + "*"); //Init Prox Sensors
  }
  if (var == "|") {
    motors.setSpeeds(0, 0);
    resetFunc();
  }
  if (var == "c") {
    Serial1.println("Sensors: Start");
    Serial1.println("Sensors: Calibrate");
    LineFollower.calibratee();
  }
  if (var == "q") {
    LineFollower.runLines = !LineFollower.runLines;
    if (LineFollower.runLines) {
      Serial1.println("Start Line Tracking");
    } else {
      Serial1.println("Stop Line Tracking");
    }
  }
  if (var == "lspd") {
    LineFollower.LineSpeed = arg.toInt();
  }
  if (var == "fl") {
    LineFollower.FollowLine = !LineFollower.FollowLine;
    if (LineFollower.FollowLine) {
      Serial1.println("Start Line Following");
    } else {
      Serial1.println("Stop Line Following");
      motors.setSpeeds(0, 0);
    }
  }
  if (var == "l") {
    runDistance = !runDistance;
    if (runDistance) {
      Serial1.println("Start Distance Tracking");
    } else {
      Serial1.println("Stop Distance Tracking");
    }
  }
  if (var == "h") {
    buzzer.play("!T240 V10 L8 a gafaeada c+adaeafa >aa>bac#ada c#adaeaf4");
  }

}

//END OF INCOMING MESSAGES

void loop() {

  //START OF REMOTE CONTROL
  xbee.loop();
  if (xbee.gotMessage) {
    xbee.gotMessage = false;
    InternalFunctions(xbee.recievedVar, xbee.recievedArg);
  }
  //END OF REMOTE CONTROL

  //START OF DEBUG BUTTONS
  if (buttonB.getSingleDebouncedPress()) {
    motors.setSpeeds(200, 200);
  }
  if (buttonC.getSingleDebouncedPress()) {
    motors.setSpeeds(0, 0);
  }
  if (buttonA.getSingleDebouncedPress()) {
    Serial1.println("*testvar:testdata*");
    Serial1.println("*testvar2:500*");
  }
  //END OF DEBUG BUTTONS


  //Handle External Functions
  LineFollower.loopLine();
//  LineTracking();//No Delay
  DistanceTracking();//No Delay
  //End Of External Functions
}
