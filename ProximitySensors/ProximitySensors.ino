#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4ProximitySensors proxSensors;
Zumo32U4Motors motors;
Zumo32U4ButtonA buttonA;

bool proxLeftActive;
bool proxFrontActive;
bool proxRightActive;

void setup()
{
  proxSensors.initThreeSensors();
  buttonA.waitForButton();
  delay(1000);
}

void printReadingsToSerial()
{
  static char buffer[80];
  sprintf(buffer, "%d %d %d %d %d %d \n",
    proxSensors.countsLeftWithLeftLeds(),
    proxSensors.countsLeftWithRightLeds(),
    proxSensors.countsFrontWithLeftLeds(),
    proxSensors.countsFrontWithRightLeds(),
    proxSensors.countsRightWithLeftLeds(),
    proxSensors.countsRightWithRightLeds()
  );
  Serial.print(buffer);
  
  if (proxSensors.countsLeftWithLeftLeds() > 5) {
  Serial.print("danger\n");
  }

}

void loop()
{
  static uint16_t lastSampleTime = 0;

  if ((uint16_t)(millis() - lastSampleTime) >= 100)
  {
    lastSampleTime = millis();
    proxSensors.read();
    proxLeftActive = proxSensors.readBasicLeft();
    proxFrontActive = proxSensors.readBasicFront();
    proxRightActive = proxSensors.readBasicRight();
    printReadingsToSerial();
  }

  if (proxSensors.countsFrontWithLeftLeds() < 5) {
  motors.setSpeeds(200, 200);
  }
  if (proxSensors.countsFrontWithLeftLeds() > 4) {
  motors.setSpeeds(0, 0);
  }

}