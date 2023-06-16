#include <Wire.h>
#include <Zumo32U4.h>
#include "MotorAansturing.h"

Zumo32U4ProximitySensors proxSensors;
Zumo32U4ButtonA buttonA;
MotorAansturing motor;
Zumo32U4Encoders encoders;
Zumo32U4LineSensors lineSensor;

bool proxLeftActive;
bool proxFrontActive;
bool proxRightActive;

unsigned int lineSensorValues[5];

int vooraan_links = -1;
int vooraan_rechts = -2;
int left = 0;
int right = 0;

void setup() {
  Serial.begin(9600);
  proxSensors.initFrontSensor();
  // Aanpassen van de brightness levels van de proximity sensors voor een nauwkeurigere meting
  uint16_t brightnesslevelsprox[] = {384,386,388,390,392,394,396,398,400,402,404,406,408,410,412,414,416,418,420, 422,424,426};
  proxSensors.setBrightnessLevels(brightnesslevelsprox, sizeof(brightnesslevelsprox)/2);
  lineSensor.initFiveSensors();
  lineSensor.calibrate();
  buttonA.waitForButton();
}

void loop() {
  //roep de methode InCirkel. 
  //Na koppeling met de andere codes moet hier een conditie komen, InCirkel wordt aangeroepen als de Zumo bruine lijn ziet
  InCirkel();
 // delay wordt hier alleen gebruikt omdat de code nog los staat, zonder koppeling met de rest van de code
  delay(10000);
}

//InCirkel laat de zumo precies 20 cm rijden met behulp van de encoder, vervolgens wordt de methode zoekEindBlok aangeroepen
void InCirkel() {
  left += encoders.getCountsAndResetLeft();
  Serial.println(left);
  right += encoders.getCountsAndResetRight();
  Serial.println(right);     
  while (left < 2092) {// 2092 counts = 20cm - de Zumo rijdt vooruit totdat er 2092 counts zijn geteld, dan is de afgelegde afstand 20cm
    motor.begin(160, 160); 
    MotorCalibratie(); //roept MotorCalibratie aan om te zorgen dat beide motors echt met dezelfde snelheid draaien 
  }
  motor.noodstop(); // motor stopt onmiddellijk
  zoekEindblok(); //roept zoekEindblok om de bolk te vinden

}

//methode MotorCalibratie zorgt ervoor dat de Zumo een rechte lijn rijdt, omdat de motoren niet even altijd hetzelfde vermogen hebben
void MotorCalibratie(){  
  left += encoders.getCountsAndResetLeft();
  Serial.println(left);
  right += encoders.getCountsAndResetRight();
  Serial.println(right);     
  while (left != right){ // blijft snelheden van de motors aanpassen totdat beide motors dezelfde counts geven
    if(left < right) {
      Serial.println("left < right"); 
      left += encoders.getCountsAndResetLeft();
      Serial.println(left);
      right += encoders.getCountsAndResetRight();
      Serial.println(right);       
      int l = motor.geef_snelheid_links() + 1;
      int r = motor.geef_snelheid_rechts();
      motor.verander_snelheid(l, r);
    }
    
    if (left > right){    
      Serial.println("left > right");
      left += encoders.getCountsAndResetLeft();
      Serial.println(left);
      right += encoders.getCountsAndResetRight();
      Serial.println(right);    
      int l = motor.geef_snelheid_links();
      int r = motor.geef_snelheid_rechts() + 1;
      motor.verander_snelheid(l, r);
    }
  }
}

//printReadingsToSerial print de metingen van de proximity sensors naar de Serial Monitor
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
  
  if ((proxSensors.countsLeftWithLeftLeds() > 5) || (proxSensors.countsLeftWithRightLeds() > 5)) {
  Serial.print("COLLISION WARNING!!!\n");
  }

}

//zoekEindblok laat de Zumo in een richting draaien totdat hij het einblok ziet, 
//dan rijdt de Zumo vooruit om het blokje uit de cirkel te duwen totdat hij de lijn van de cirkel ziet
void zoekEindblok() {
  Serial.println("zoeken");
  motor.verander_snelheid(200, 200); // stel de snelheid van de motoren in op 200
  motor.links(); // laat de Zumo om zijn eigen as naar links draaien
  static uint16_t lastSampleTime = 0;
  vooraan_links = -1;
  vooraan_rechts = -2;
  while (!((vooraan_links == vooraan_rechts) && ((vooraan_links > 0) && (vooraan_rechts > 0)))) // blijft zoeken voor de bolk totdat hij het vindt
  {
    vooraan_links = proxSensors.countsFrontWithLeftLeds();
    vooraan_rechts = proxSensors.countsFrontWithRightLeds();
    printReadingsToSerial();
    if ((uint16_t)(millis() - lastSampleTime) >= 100) //runt elke 100ms, leest de waarde van de proximity sensors uit
    {
      lastSampleTime = millis();
      proxSensors.read();
      proxLeftActive = proxSensors.readBasicLeft();
      proxFrontActive = proxSensors.readBasicFront();
      proxRightActive = proxSensors.readBasicRight();
      printReadingsToSerial();
    }
  }
  vooraan_links = -1;
  vooraan_rechts = -2;
  motor.noodstop(); //als de Zumo het blok vindt, stopt hij met draaien zodat het blok direct voor hem staat

  lineSensor.read(lineSensorValues); //leest waarde van de linesensor uit
  uint16_t middle = lineSensorValues[2];
  
  while (middle < 690){ //Zumo rijdt vooruit (richting het blok) totdat hij de lijn van de cirkel ziet
    motor.vooruit();
    lineSensor.read(lineSensorValues);
    middle = lineSensorValues[2];
  }
  
  motor.noodstop();//de zumo stopt
}