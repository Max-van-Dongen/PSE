#include "Afstandssensoren.h"

Afstandssensoren::Afstandssensoren()
{
}

void Afstandssensoren::Initialiseren() //initialiseert de afstandssensoren
{
  proxSensors.initThreeSensors();
}

void Afstandssensoren::read() //zorgt ervoor dat de afstandssensoren actief zijn en waardes uitlezen
{
  proxSensors.read();
  proxLinksActief = proxSensors.readBasicLeft();
  proxVoorActief = proxSensors.readBasicFront();
  proxRechtsActief = proxSensors.readBasicRight();
}

void Afstandssensoren::UitlezingenAfdrukken(){ //zorgt ervoor dat de waardes van de afstandssensoren worden geprint op de terminal, geeft waarschuwing nabij obstakel
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
  
  if ((proxSensors.countsLeftWithLeftLeds() > 5) || (proxSensors.countsLeftWithRightLeds() > 5) || (proxSensors.countsRightWithLeftLeds() > 5) ||
      (proxSensors.countsRightWithRightLeds() > 5) || (proxSensors.countsFrontWithLeftLeds() > 5) || (proxSensors.countsFrontWithRightLeds() > 5)){
    Serial.print("WAARSCHUWING! Obstakel nabij!\n");
  }
}

void Afstandssensoren::DetectieTest(){ //laat de zumo vooruit rijden totdat een obstakel gedectecteerd wordt
  
  static uint16_t laatsteCheckTijd = 0;

  if ((uint16_t)(millis() - laatsteCheckTijd) >= 100)
  {
    laatsteCheckTijd = millis();
    read();
    proxLinksActief = proxSensors.readBasicLeft();
    proxVoorActief = proxSensors.readBasicFront();
    proxRechtsActief = proxSensors.readBasicRight();
    UitlezingenAfdrukken();
  }

  if ((proxSensors.countsFrontWithLeftLeds() < 5) || (proxSensors.countsFrontWithRightLeds() < 5)) {
    motors.setSpeeds(200, 200);
  }
  else {
    motors.setSpeeds(0, 0);
  }
}