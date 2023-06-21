#ifndef PROXIMITY_SENSORS_H
#define PROXIMITY_SENSORS_H

#include <Zumo32U4.h>

class ProximitySensors
{
  public:
    ProximitySensors();
    void init();
    void read();
    bool readBasicLeft();
    bool readBasicFront();
    bool readBasicRight();
    uint16_t countsLeftWithLeftLeds();
    uint16_t countsLeftWithRightLeds();
    uint16_t countsFrontWithLeftLeds();
    uint16_t countsFrontWithRightLeds();
    uint16_t countsRightWithLeftLeds();
    uint16_t countsRightWithRightLeds();

  private:
    Zumo32U4ProximitySensors proxSensors;
    bool proxLeftActive;
    bool proxFrontActive;
    bool proxRightActive;
};
#endif
