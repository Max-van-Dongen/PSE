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
 uint16_t brightnesslevelsprox[] = {384,386,388,390,392,394,396,398,400,402,404,406,408,410,412,414,416,418,420, 422,424,426};
 //395,400,405,410,415,420,425,430,435,440,445,450,455,460,465,470,475,480,485,490,495,500,505,510,515,520,525,530,535,540,545,550,555,560,565,570,575,580,585,590,595,600,605,610,615,620,625,630,635,640,645,650,655,660,665,670,675,680,685,690,695
  proxSensors.setBrightnessLevels(brightnesslevelsprox, sizeof(brightnesslevelsprox)/2);
  lineSensor.initFiveSensors();
  lineSensor.calibrate();
  buttonA.waitForButton();
}

void loop() {
  InCirkel();
 /* lineSensor.read(lineSensorValues);
  uint16_t middle = lineSensorValues[2];
  
  while (middle < 690){
    Serial.println(middle);
    motor.begin(50,50);
    lineSensor.read(lineSensorValues);
    middle = lineSensorValues[2];
  }
  motor.noodstop();*/
  delay(10000);
}

void InCirkel() {
  Serial.print("links: ");
  Serial.println(motor.geef_snelheid_links());
  Serial.print("rechts: ");
  Serial.println(motor.geef_snelheid_rechts());
  left += encoders.getCountsAndResetLeft();
  Serial.println(left);
  right += encoders.getCountsAndResetRight();
  Serial.println(right);     
  while (left < 2092) {//20cm: 2092
    motor.begin(160, 160); //(160, 173)

    MotorCalibratie();
  }
  motor.noodstop();
  zoekEindblok();

}

void MotorCalibratie(){  
 // int snleheid = motor.geef_snelheid_links();
  left += encoders.getCountsAndResetLeft();
  Serial.println(left);
  right += encoders.getCountsAndResetRight();
  Serial.println(right);     
  while (left != right){
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

void zoekEindblok() {
  Serial.println("zoeken");
  motor.verander_snelheid(200, 200);
  motor.links();
  static uint16_t lastSampleTime = 0;
  vooraan_links = -1;
  vooraan_rechts = -2;
  while (!((vooraan_links == vooraan_rechts) && ((vooraan_links > 0) && (vooraan_rechts > 0)))) 
  {
    vooraan_links = proxSensors.countsFrontWithLeftLeds();
    vooraan_rechts = proxSensors.countsFrontWithRightLeds();
    printReadingsToSerial();
    if ((uint16_t)(millis() - lastSampleTime) >= 100)
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
  motor.noodstop();

  lineSensor.read(lineSensorValues);
  uint16_t middle = lineSensorValues[2];
  
  while (middle < 690){
    motor.vooruit();
    lineSensor.read(lineSensorValues);
    middle = lineSensorValues[2];
  }
  
  // left += encoders.getCountsAndResetLeft();
  // Serial.println(left);
  // right += encoders.getCountsAndResetRight();
  // Serial.println(right);
  // MotorCalibratie();
  //delay(3000);
  motor.noodstop();
}