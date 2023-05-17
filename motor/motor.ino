#include <Wire.h>
#include <Zumo32U4.h>
#include "MotorAansturing.h"

Zumo32U4ButtonA buttonA;
MotorAansturing motor;

int a = 100;

void setup() {
  Serial.begin(9600);
  
  buttonA.waitForButton();
  delay(1000);
}

void loop() {
  motor.begin(a, a);
  Serial.println(motor.geef_snelheid_links());
  Serial.println(motor.geef_snelheid_rechts());
  delay(3000);
  motor.versnellen_tot(200);
  Serial.println(motor.geef_snelheid_links());
  Serial.println(motor.geef_snelheid_rechts());
  delay(3000);
  motor.langzaam_stop();
  Serial.println(motor.geef_snelheid_links());
  Serial.println(motor.geef_snelheid_rechts());
  delay(1000);
  motor.vooruit();
  Serial.println("vooruit");
  Serial.println(motor.geef_snelheid_links());
  Serial.println(motor.geef_snelheid_rechts());
  delay(3000);
  motor.noodstop();
  Serial.println(motor.geef_snelheid_links());
  Serial.println(motor.geef_snelheid_rechts());
  delay(3000);
  
  /*motor.links();
  delay(3000);
  motor.vooruit();
  delay(3000);
  motor.vooruit_rechts();
  delay(3000);
  motor.achteruit_links();
  Serial.println(motor.geef_snelheid_links());
  Serial.println(motor.geef_snelheid_rechts());
  delay(3000);
  motor.stop(); //dit was de methode voor stoppen voordat ik de langzaam_stop en noodstop gemaakt
  delay(3000);
  
  if ((lineSensorValues[0] <=290) && (lineSensorValues[2] <=120) && (lineSensorValues[4] <=220)){
    Serial.println("wit");
  }
  else if ((lineSensorValues[0] >=560) && (lineSensorValues[0] <=690) && (lineSensorValues[2] >=295) && (lineSensorValues[2] <=350) && (lineSensorValues[4] >=450) && (lineSensorValues[4] <=540)){
    Serial.println("grijs");
  }
  else if ((lineSensorValues[0] >=400) && (lineSensorValues[0] <=490) && (lineSensorValues[2] >=195) && (lineSensorValues[2] <=285) && (lineSensorValues[4] >=300) && (lineSensorValues[4] <=390)){
    Serial.println("groen");
  }
  else if ((lineSensorValues[0] >=300) && (lineSensorValues[0] <=390) && (lineSensorValues[2] >=130) && (lineSensorValues[2] <=185) && (lineSensorValues[4] >=230) && (lineSensorValues[4] <=290)){
    Serial.println("blauw");
  }
  else if ((lineSensorValues[0] >=710) && (lineSensorValues[0] <=800) && (lineSensorValues[2] >=370) && (lineSensorValues[2] <=440) && (lineSensorValues[4] >=560) && (lineSensorValues[4] <=800)){
    Serial.println("bruin");
  }
  else if ((lineSensorValues[0] >=1480) && (lineSensorValues[2] >=1090) && (lineSensorValues[4] >=1090)){
    Serial.println("zwart");
  }
  */
}