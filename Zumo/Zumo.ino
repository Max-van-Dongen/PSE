#include <Wire.h>
#include <Zumo32U4.h>




Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;
Zumo32U4ProximitySensors proxSensors;
Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors;
Zumo32U4Buzzer buzzer;


//Line Sensor Variables
#define NUM_SENSORS 5
unsigned int lineSensorValues[NUM_SENSORS];
static uint16_t lastLineTime = 0;
static uint16_t lastLinePrintTime = 0;
//End Of Line Sensor Variables

//States
bool runLines = false;
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



//Function to calibrate the sensors
//Wait 1 second and then begin automatic sensor calibration
//by rotating in place to sweep the sensors over the line
void calibrateSensors()
{

  delay(1000);
  for (uint16_t i = 0; i < 120; i++)
  {
    if (i > 30 && i <= 90)
    {
      motors.setSpeeds(-200, 200);
    }
    else
    {
      motors.setSpeeds(200, -200);
    }

    lineSensors.calibrate();
  }
  motors.setSpeeds(0, 0);
}
//End Of Calibrate Function



//Function that crashes the arduino, used to hard reset the program in case of an unexpected state or error
void(* resetFunc) (void) = 0;

//Setup to init Serials and to initialize 5 line sensors
void setup() {
  Serial.begin(115200);
  Serial1.begin(115200);
  Serial1.println("*bt:0*");
  lineSensors.initFiveSensors();
  proxSensors.initThreeSensors();
}

//START OF LINE FOLLOWING

void LineTracking() {
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
    if ((uint16_t)(millis() - lastLineTime) >= 200)//runt elke 200ms TODO: optimize??? 50ms mischien beter als we sneller gaan?
    {
      lastLineTime = millis();
      lineSensors.readCalibrated(lineSensorValues);
      //    if (left > 800) {
      //      motors.setSpeeds(0, 400);
      //      //      delay(500);
      //    }
      //
      //    if (right > 800) {
      //      Serial1.println("MOVE RIGHT!");
      //      motors.setSpeeds(400, 0);
      //      //      delay(500);
      //    }
      //
      //    if (middle > 800) {
      //      Serial1.println("MOVE STRAIGHT!");
      //      motors.setSpeeds(400, 400);
      //      //      delay(500);
      //    }
      //    if (left < 400 && middle < 400 && right < 400) {
      //      Serial1.println("MOVE quququ!");
      //      motors.setSpeeds(400, -400);
      //      //      delay(500);
      //    }
    }
  }
}
//END OF LINE FOLLOWING



//Function to write proximity data out to serial
void printReadingsToSerial()
{
  Serial1.println("*PSL:" + (String)proxSensors.countsLeftWithLeftLeds() + "*");
  Serial1.println("*PSMLL:" + (String)proxSensors.countsLeftWithRightLeds() + "*");
  Serial1.println("*PSML:" + (String)proxSensors.countsFrontWithLeftLeds() + "*");
  Serial1.println("*PSMR:" + (String)proxSensors.countsFrontWithRightLeds() + "*");
  Serial1.println("*PSMRR:" + (String)proxSensors.countsRightWithLeftLeds() + "*");
  Serial1.println("*PSR:" + (String)proxSensors.countsRightWithRightLeds() + "*");

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

      if ((proxSensors.countsLeftWithLeftLeds() > 5) || (proxSensors.countsLeftWithRightLeds() > 5) || (proxSensors.countsRightWithLeftLeds() > 5) ||
          (proxSensors.countsRightWithRightLeds() > 5) || (proxSensors.countsFrontWithLeftLeds() > 5) || (proxSensors.countsFrontWithRightLeds() > 5)) {
        //        Serial1.println("COLLISION WARNING!!!");
      }
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
    motors.setLeftSpeed(arg.toInt());
  }
  if (var == "rs") {//RightSpeed
    motors.setRightSpeed(arg.toInt());
  }
  if (var == "hb") {//HeartBeat
    Serial1.println("*hb:" + ((String)ctr) + "*");
    ctr++;
  }
  if (var == "init") {//Init Sensors
    Serial1.println("*initls:" + ((String)runLines) + "*"); //Init Line Sensors
    Serial1.println("*initps:" + ((String)runDistance) + "*"); //Init Prox Sensors
  }
  if (var == "|") {
    motors.setSpeeds(0, 0);
    resetFunc();
  }
  if (var == "c") {
    Serial1.println("Sensors: Start");
    lineSensors.calibrate();
    Serial1.println("Sensors: Calibrate");
    calibrateSensors();
  }
  if (var == "q") {
    runLines = !runLines;
    if (runLines) {
      buzzer.play("g32");
      Serial1.println("Start Line Tracking");
    } else {
      buzzer.play("e32");
      Serial1.println("Stop Line Tracking");
    }
  }
  if (var == "l") {
    runDistance = !runDistance;
    if (runDistance) {
      buzzer.play("g32");
      Serial1.println("Start Distance Tracking");
    } else {
      buzzer.play("e32");
      Serial1.println("Stop Distance Tracking");
    }
  }

}

String messagePart = "";
bool receiving = false;
bool beforesemicolon = true;
String recievedVar = "";
String recievedArg = "";
void HandleSpecialMessages(const String &msg) {
  if (msg.length() == 0) return;
  if (msg == "*") {
    receiving = !receiving;
    //    Serial1.println("REC! " + (String)receiving);
    if (!beforesemicolon) {
      beforesemicolon = true;
      recievedArg = messagePart;
      messagePart = "";
      //      Serial1.println(recievedVar);
      //      Serial1.println(recievedArg);
      InternalFunctions(recievedVar, recievedArg);
    }
    return;
  }
  if (msg == ":") {
    beforesemicolon = !beforesemicolon;
    //    Serial1.println("SEMI!" + (String)beforesemicolon);
    recievedVar = messagePart;
    messagePart = "";
    return;
  }
  if (receiving) {
    messagePart += msg;
  }

}
//END OF INCOMING MESSAGES

void loop() {

  //START OF REMOTE CONTROL
  if (Serial1.available()) { // Check if there is any incoming data
    char incomingChar = Serial1.read(); // Read the incoming byte
    HandleSpecialMessages((String)incomingChar);
    //    buzzer.play("g32");
    //    switch (incomingChar) {
    //      case 'w'://Forward
    //        motors.setSpeeds(200, 200);
    //        break;
    //      case 'a'://Left
    //        motors.setSpeeds(-200, 200);
    //        break;
    //      case 's'://Back
    //        motors.setSpeeds(-200, -200);
    //        break;
    //      case 'd'://Right
    //        motors.setSpeeds(200, -200);
    //        break;
    //      case 'W'://Fast Forward
    //        motors.setSpeeds(400, 400);
    //        break;
    //      case 'A'://Fast Left
    //        motors.setSpeeds(-400, 400);
    //        break;
    //      case 'S'://Fast Back
    //        motors.setSpeeds(-400, -400);
    //        break;
    //      case 'D'://Fast Right
    //        motors.setSpeeds(400, -400);
    //        break;
    //      case '|'://Crash arduino
    //        motors.setSpeeds(0, 0);
    //        resetFunc();
    //        break;
    //      case 'e':
    //      case 'E'://Stop motors
    //        motors.setSpeeds(0, 0);
    //        break;
    //      case 'c'://Start Sensors Calibration
    //        Serial1.println("Sensors: Start");
    //        lineSensors.calibrate();
    //        Serial1.println("Sensors: Calibrate");
    //        calibrateSensors();
    //        break;
    //      case 'q'://Toggle Running Of Line Function
    //        runLines = !runLines;
    //        if (runLines) {
    //          buzzer.play("g32");
    //          Serial1.println("Start Line Tracking");
    //        } else {
    //          buzzer.play("e32");
    //          Serial1.println("Stop Line Tracking");
    //        }
    //        break;
    //      case 'l'://Toggle Running Of Distance Fuction
    //        runDistance = !runDistance;
    //        if (runDistance) {
    //          buzzer.play("g32");
    //          Serial1.println("Start Distance Tracking");
    //        } else {
    //          buzzer.play("e32");
    //          Serial1.println("Stop Distance Tracking");
    //        }
    //        break;
    //    }
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
  LineTracking();//No Delay
  DistanceTracking();//No Delay
  //End Of External Functions



}
