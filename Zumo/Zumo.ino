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
bool FollowLine = false;
bool runDistance = false;
bool isCalibrated = false;
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
  isCalibrated = true;
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
  uint16_t BLevels[] = {1, 2, 4, 9, 15, 23, 32, 42, 55, 70, 85, 100, 120, 135, 150, 170};   // default is { 4, 15, 32, 55, 85, 120 }
  proxSensors.setBrightnessLevels(BLevels, 16);
}

//START OF LINE FOLLOWING

String lastLinePosition = "";
int LineSpeed = 200;
int BlackValue = 800;

void LineTracking() {
  
int CornerSpeed = LineSpeed*.7;
  if (runLines) {
    if ((uint16_t)(millis() - lastLinePrintTime) >= 500)//runt elke 500ms
    {
      lastLinePrintTime = millis();
      Serial1.println("*LSL:" + (String)lineSensorValues[0] + "*");
      //      Serial1.println("*LSML:" + (String)lineSensorValues[1] + "*");
      Serial1.println("*LSM:" + (String)lineSensorValues[2] + "*");
      //      Serial1.println("*LSMR:" + (String)lineSensorValues[3] + "*");
      Serial1.println("*LSR:" + (String)lineSensorValues[4] + "*");
    }
    if ((uint16_t)(millis() - lastLineTime) >= 1)//runt elke 200ms TODO: optimize??? 50ms mischien beter als we sneller gaan?
    {
      bool HasRan = false;
      lastLineTime = millis();
      lineSensors.readCalibrated(lineSensorValues);
      if (FollowLine) {
        uint16_t left = lineSensorValues[0];
        uint16_t middle = lineSensorValues[2];
        uint16_t right = lineSensorValues[4];
        
        if (!HasRan && middle > BlackValue && right > BlackValue && left > BlackValue) {
          motors.setSpeeds(CornerSpeed, 0);
          lastLinePosition = "Right";
          HasRan = true;
        }
        
        if (!HasRan && middle > BlackValue && right > BlackValue) {
          motors.setSpeeds(CornerSpeed, 0);
          lastLinePosition = "Right";
          HasRan = true;
        }
        
        if (!HasRan && middle > BlackValue && left > BlackValue) {
          motors.setSpeeds(0, CornerSpeed);
          lastLinePosition = "Left";
          HasRan = true;
        }
        if (!HasRan && left > BlackValue) {
          motors.setSpeeds(0, CornerSpeed);
          lastLinePosition = "Left";
          HasRan = true;
        }

        if (!HasRan && right > BlackValue) {
          motors.setSpeeds(CornerSpeed, 0);
          lastLinePosition = "Right";
          HasRan = true;
        }

        if (!HasRan && middle > BlackValue) {
          motors.setSpeeds(LineSpeed, LineSpeed);
          lastLinePosition = "Middle";
          HasRan = true;
        }
        if (!HasRan && left < BlackValue && middle < BlackValue && right < BlackValue) {
          HasRan = true;
          if (lastLinePosition == "Left") {
            motors.setSpeeds(0, CornerSpeed);
          }
          if (lastLinePosition == "Right") {
            motors.setSpeeds(CornerSpeed, 0);
          }

          if (lastLinePosition == "Middle") {
//            motors.setSpeeds(-200, -200);
          }
        }
      }

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
    Serial1.println("*initfls:" + ((String)FollowLine) + "*"); //Init Prox Sensors
    Serial1.println("*initclb:" + ((String)isCalibrated) + "*"); //Init Prox Sensors
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
      Serial1.println("Start Line Tracking");
    } else {
      Serial1.println("Stop Line Tracking");
    }
  }
  if (var == "lspd") {
    LineSpeed = arg.toInt();
  }
  if (var == "fl") {
    FollowLine = !FollowLine;
    if (FollowLine) {
      Serial1.println("Start Line Following");
    } else {
      Serial1.println("Stop Line Following");
      motors.setSpeeds(0,0);
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
    buzzer.play("!T240 V7 L8 a gafaeada c+adaeafa >aa>bac#ada c#adaeaf4");
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
