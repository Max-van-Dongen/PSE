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
void calibrateSensors()
{

  // Wait 1 second and then begin automatic sensor calibration
  // by rotating in place to sweep the sensors over the line
  delay(1000);
  for (uint16_t i = 0; i < 120; i++)
  {
    if (i > 30 && i <= 90)
    {
      motors.setSpeeds(-100, 100);
    }
    else
    {
      motors.setSpeeds(100, -100);
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
  Serial1.println("Sensors: Start");
  lineSensors.calibrate();
  Serial1.println("Sensors: Calibrate");

  buttonA.waitForButton();
  calibrateSensors();

}

void loop() {
  if (Serial1.available()) { // Check if there is any incoming data
    char incomingChar = Serial1.read(); // Read the incoming byte
    buzzer.play("g32");
    Serial.print(incomingChar); // Print the incoming byte on the Serial Monitor
  }
  // put your main code here, to run repeatedly:
  if (buttonA.isPressed())
  {
    // Whenever the button is pressed, turn on the yellow LED.
    runLines = !runLines;
    if (runLines) {
      buzzer.play("g32");
    } else {
      buzzer.play("e32");
    }
  }

  //  if (buttonB.isPressed())
  //  {
  //    // Whenever the button is pressed, turn on the yellow LED.
  //    motors.setLeftSpeed(40);
  //    motors.setRightSpeed(40);
  //  }
  //
  //  if (buttonC.isPressed())
  //  {
  //    // Whenever the button is pressed, turn on the yellow LED.
  //    motors.setLeftSpeed(0);
  //    motors.setRightSpeed(40);
  //  }
  //  if (!buttonC.isPressed() && !buttonB.isPressed() && !buttonA.isPressed())
  //  {
  //    motors.setLeftSpeed(0);
  //    motors.setRightSpeed(0);
  //
  //  }
  uint8_t left = lineSensorValues[0];
  uint8_t middle = lineSensorValues[2];
  uint8_t right = lineSensorValues[4];
  lineSensors.readCalibrated(lineSensorValues);
  if (runLines) {
    if (left > 800) {
      motors.setSpeeds(0, 400);
    }

    if (right > 800) {
      motors.setSpeeds(400, 0);
    }

    if (middle > 800) {
      motors.setSpeeds(400, 400);
    }
    if (left < 400 && middle < 400 && right < 400) {
      motors.setSpeeds(400,-400);
    }
  }
  for (uint8_t i = 0; i < NUM_SENSORS; i++)
  {
    //    uint8_t barHeight = map(lineSensorValues[i], 0, 1000, 0, 8);
    Serial1.println(lineSensorValues[i]);
    //    Serial.println(lineSensors.readCalibrated(lineSensorValues[i]));
    //    uint8_t  lines = lineSensors.readCalibrated(lineSensorValues[1]);
    //    seri
  }
  //  Serial.println("POS: " + position);
  Serial1.println("end");
  delay(100);
}
