#ifndef AFSTANDSSENSOREN_H
#define AFSTANDSSENSOREN_H

#include <Wire.h>
#include <Zumo32U4.h>

class Afstandssensoren
{
  public:
    Afstandssensoren();
    void Initialiseren();
    void DetectieTest();

  private:
    Zumo32U4Motors motors;
    Zumo32U4ProximitySensors proxSensors;
    bool proxLinksActief;
    bool proxVoorActief;
    bool proxRechtsActief;
    void read();
    void UitlezingenAfdrukken();
};
#endif