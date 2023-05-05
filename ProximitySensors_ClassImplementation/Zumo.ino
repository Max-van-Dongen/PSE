#include <Wire.h>
#include <Zumo32U4.h>
#include "ProximitySensors.h"

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
  Serial.print("Afstand: ");
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
  
  if ((proxSensors.countsLeftWithLeftLeds() > 4) || (proxSensors.countsLeftWithRightLeds() > 4) || (proxSensors.countsRightWithLeftLeds() > 4) || (proxSensors.countsRightWithRightLeds() > 4) || (proxSensors.countsFrontWithLeftLeds() > 4) || (proxSensors.countsFrontWithRightLeds() > 4)) {
    Serial.print("COLLISION WARNING!!!\n");
  }
}

static uint16_t lastSampleTime = 0;

void loop()
{
  if ((uint16_t)(millis() - lastSampleTime) >= 100)
  {
    lastSampleTime = millis();
    proxSensors.read();
    proxLeftActive = proxSensors.readBasicLeft();
    proxFrontActive = proxSensors.readBasicFront();
    proxRightActive = proxSensors.readBasicRight();
    printReadingsToSerial();
  }

  if ((proxSensors.countsFrontWithLeftLeds() < 5) || (proxSensors.countsFrontWithRightLeds() < 5)) {
    motors.setSpeeds(200, 200);
  }
  else {
    motors.setSpeeds(0, 0);
  }
}