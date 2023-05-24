#include <Wire.h>
#include "HellingSensor.h"

HellingSensor Gyro;

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial1.println("BOOT");

    Wire.begin();
 
}

void loop()
{ 
    Gyro.leesHelling();   
    delay(100);
}

/*
 * z is whatever
 * x is front
 * y is sides
 * rechtop is tussen -20 en 20
 * helling op is >20
 * helling af 01 <-20
 */
