#include "ProximitySensors.h"

ProximitySensors::ProximitySensors()
{
}

void ProximitySensors::init()
{
  proxSensors.initThreeSensors();
}

void ProximitySensors::read()
{
  proxSensors.read();
  proxLeftActive = proxSensors.readBasicLeft();
  proxFrontActive = proxSensors.readBasicFront();
  proxRightActive = proxSensors.readBasicRight();
}

bool ProximitySensors::readBasicLeft()
{
  return proxLeftActive;
}

bool ProximitySensors::readBasicFront()
{
  return proxFrontActive;
}

bool ProximitySensors::readBasicRight()
{
  return proxRightActive;
}

uint16_t ProximitySensors::countsLeftWithLeftLeds()
{
  return proxSensors.countsLeftWithLeftLeds();
}

uint16_t ProximitySensors::countsLeftWithRightLeds()
{
  return proxSensors.countsLeftWithRightLeds();
}

uint16_t ProximitySensors::countsFrontWithLeftLeds()
{
  return proxSensors.countsFrontWithLeftLeds();
}

uint16_t ProximitySensors::countsFrontWithRightLeds()
{
  return proxSensors.countsFrontWithRightLeds();
}

uint16_t ProximitySensors::countsRightWithLeftLeds()
{
  return proxSensors.countsRightWithLeftLeds();
}

uint16_t ProximitySensors::countsRightWithRightLeds()
{
  return proxSensors.countsRightWithRightLeds();
}