#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4IMU sensor; 

void setup()
{
    // put your setup code here, to run once:
    Serial.begin(9600);
    Serial1.println("BOOT");

    Wire.begin();
  
    // Set up the inertial sensors.
    sensor.init();
    sensor.enableDefault();
  
}

void loop()
{
    sensor.readAcc();
    int x = sensor.a.x / 100;
    if (x > 30) {
        Serial.println("helling omhoog");
    }
    else if (x < -30) {
        Serial.println("helling omlaag");
    }
    else {
        Serial.println("geen helling");
    }
    Serial.println(x);
    
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
