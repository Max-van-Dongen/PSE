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
//End Of Line Sensor Variables

//States
bool runLines = false;
bool runDistance = false;
//End Of States

//Distance Sensor Variables
static uint16_t lastSampleTime = 0;
bool proxLeftActive;
bool proxFrontActive;
bool proxRightActive;
//End Of Distance Sensor Variables

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

//Debug function to write proximity data out to serial
void printReadingsToSerial()
{
  Serial1.print("Afstand: ");
  static char buffer[80];
  sprintf(buffer, "%d %d %d %d %d %d \n",
          proxSensors.countsLeftWithLeftLeds(),
          proxSensors.countsLeftWithRightLeds(),
          proxSensors.countsFrontWithLeftLeds(),
          proxSensors.countsFrontWithRightLeds(),
          proxSensors.countsRightWithLeftLeds(),
          proxSensors.countsRightWithRightLeds()
         );
  Serial1.print(buffer);

  if ((proxSensors.countsLeftWithLeftLeds() > 5) || (proxSensors.countsLeftWithRightLeds() > 5) || (proxSensors.countsRightWithLeftLeds() > 5) || (proxSensors.countsRightWithRightLeds() > 5) || (proxSensors.countsFrontWithLeftLeds() > 5) || (proxSensors.countsFrontWithRightLeds() > 5)) {
    Serial1.println("COLLISION WARNING!!!");
  }
}
//End Of Proximity Function

//Function that crashes the arduino, used to hard reset the program in case of an unexpected state or error
void(* resetFunc) (void) = 0;

//Setup to init Serials and to initialize 5 line sensors
void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial1.println("BOOT");
  lineSensors.initFiveSensors();
}

//START OF LINE FOLLOWING
void LineTracking() {
  if (runLines) {
    uint8_t left = lineSensorValues[0];
    uint8_t mleft = lineSensorValues[1];
    uint8_t middle = lineSensorValues[2];
    uint8_t mright = lineSensorValues[3];
    uint8_t right = lineSensorValues[4];
    lineSensors.readCalibrated(lineSensorValues);
    Serial1.print("Left: ");
    Serial1.print(left);
    Serial1.print("\n");
    Serial1.print("mleft: ");
    Serial1.print(mleft);
    Serial1.print("\n");
    Serial1.print("middle: ");
    Serial1.print(middle);
    Serial1.print("\n");
    Serial1.print("mright: ");
    Serial1.print(mright);
    Serial1.print("\n");
    Serial1.print("right: ");
    Serial1.print(right);
    Serial1.print("\n");
    delay(200);
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
//END OF LINE FOLLOWING


//START OF DISTANCE SENSORS
void DistanceTracking() {
  //(gestolen van martin)
  if (runDistance) {
    if ((uint16_t)(millis() - lastSampleTime) >= 200)
    {
      lastSampleTime = millis();
      proxSensors.read();
      proxLeftActive = proxSensors.readBasicLeft();
      proxFrontActive = proxSensors.readBasicFront();
      proxRightActive = proxSensors.readBasicRight();
      printReadingsToSerial();
    }
  }
}
//END OF DISTANCE SENSORS


void loop() {

  //START OF REMOTE CONTROL
  if (Serial1.available()) { // Check if there is any incoming data
    char incomingChar = Serial1.read(); // Read the incoming byte
    buzzer.play("g32");
    switch (incomingChar) {
      case 'w'://Forward
        motors.setSpeeds(200, 200);
        break;
      case 'a'://Left
        motors.setSpeeds(-200, 200);
        break;
      case 's'://Back
        motors.setSpeeds(-200, -200);
        break;
      case 'd'://Right
        motors.setSpeeds(200, -200);
        break;
      case 'W'://Fast Forward
        motors.setSpeeds(400, 400);
        break;
      case 'A'://Fast Left
        motors.setSpeeds(-400, 400);
        break;
      case 'S'://Fast Back
        motors.setSpeeds(-400, -400);
        break;
      case 'D'://Fast Right
        motors.setSpeeds(400, -400);
        break;
      case '|'://Crash arduino
        motors.setSpeeds(0, 0);
        resetFunc();
        break;
      case 'e':
      case 'E'://Stop motors
        motors.setSpeeds(0, 0);
        break;
      case 'c'://Start Sensors Calibration
        Serial1.println("Sensors: Start");
        lineSensors.calibrate();
        Serial1.println("Sensors: Calibrate");
        calibrateSensors();
        break;
      case 'q'://Toggle Running Of Line Function
        runLines = !runLines;
        if (runDistance) {
          buzzer.play("g32");
          Serial1.println("Start Line Tracking");
        } else {
          buzzer.play("e32");
          Serial1.println("Stop Line Tracking");
        }
        break;
      case 'l'://Toggle Running Of Distance Fuction
        runDistance = !runDistance;
        if (runDistance) {
          buzzer.play("g32");
          Serial1.println("Start Distance Tracking");
        } else {
          buzzer.play("e32");
          Serial1.println("Stop Distance Tracking");
        }
        break;
    }
  }
  //END OF REMOTE CONTROL

  //START OF DEBUG BUTTONS
  if (buttonB.getSingleDebouncedPress()) {
    motors.setSpeeds(200, 200);
  }
  if (buttonC.getSingleDebouncedPress()) {
    motors.setSpeeds(0, 0);
  }
  //END OF DEBUG BUTTONS


  //Handle External Functions
  LineTracking();//No Delay
  DistanceTracking();//No Delay
  //End Of External Functions



}
