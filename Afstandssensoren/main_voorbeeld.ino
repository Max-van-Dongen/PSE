#include <Wire.h>
#include <Zumo32U4.h>
#include "Afstandssensoren.h"

Zumo32U4ButtonA buttonA;
Afstandssensoren prox;

void setup()
{
  prox.Initialiseren(); //initialiseert de afstandssensoren
  buttonA.waitForButton(); //zodra A knop wordt gedrukt start de DetectieTest
  delay(1000); //1 seconde wachten met starten DetectieTest na drukken knop
}

void loop()
{
 prox.DetectieTest(); //roept de DetectieTest methode uit voor het testen en configureren van de afstandssensoren
}