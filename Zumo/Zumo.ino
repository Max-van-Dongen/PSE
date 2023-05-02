#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4ButtonA buttonA;
Zumo32U4ButtonB buttonB;
Zumo32U4ButtonC buttonC;
Zumo32U4LineSensors lineSensors;
Zumo32U4Motors motors;
Zumo32U4Buzzer buzzer;



#define NUM_SENSORS 5
unsigned int lineSensorValues[NUM_SENSORS];
bool runLines = false;
int motorSpeed = 0.5;
void calibrateSensors()
{

  // Wait 1 second and then begin automatic sensor calibration
  // by rotating in place to sweep the sensors over the line
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

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial1.begin(9600);
  Serial1.println("BOOT");
  lineSensors.initFiveSensors();
}

void loop() {
  if (Serial1.available()) { // Check if there is any incoming data
    char incomingChar = Serial1.read(); // Read the incoming byte
    buzzer.play("g32");
//    Serial.print(incomingChar); // Print the incoming byte on the Serial Monitor
    switch (incomingChar) {
      case 'w':
        motors.setSpeeds(200, 200);
        break;
      case 't':
        motors.setSpeeds(400, 400);
        break;
      case 'a':
        motors.setSpeeds(-200, 200);
        break;
      case 's':
        motors.setSpeeds(-200, -400);
        break;
      case 'd':
        motors.setSpeeds(200, -200);
        break;
      case 'e':
        motors.setSpeeds(0, 0);
        break;
      case 'c':
        Serial1.println("Sensors: Start");
        lineSensors.calibrate();
        Serial1.println("Sensors: Calibrate");
        calibrateSensors();
        break;
      case 'q':
        motors.setSpeeds(0, 400);
        runLines = !runLines;
        if (runLines) {
          buzzer.play("g32");
        } else {
          buzzer.play("e32");
        }
        break;
    }
  }
  if (buttonB.getSingleDebouncedPress()) {
      motors.setSpeeds(200, 200);
  }
  if (buttonC.getSingleDebouncedPress()) {
      motors.setSpeeds(0, 0);
  }

  if (runLines) {
  uint8_t left = lineSensorValues[0];
  uint8_t middle = lineSensorValues[2];
  uint8_t right = lineSensorValues[4];
  lineSensors.readCalibrated(lineSensorValues);
    Serial1.println("Left: " + left);
    Serial1.println("Middle: " + middle);
    Serial1.println("Right: " + right);
    if (left > 800) {
      motors.setSpeeds(0, 400);
      //      delay(500);
    }

    if (right > 800) {
      Serial1.println("MOVE RIGHT!");
      motors.setSpeeds(400, 0);
      //      delay(500);
    }

    if (middle > 800) {
      Serial1.println("MOVE STRAIGHT!");
      motors.setSpeeds(400, 400);
      //      delay(500);
    }
    if (left < 400 && middle < 400 && right < 400) {
      Serial1.println("MOVE quququ!");
      motors.setSpeeds(400, -400);
      //      delay(500);
    }
  }
}
