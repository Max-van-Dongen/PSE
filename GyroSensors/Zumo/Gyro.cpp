#include <Wire.h>
#include <Zumo32U4.h>

Zumo32U4IMU sensor; 

Gyro::Gyro() {}

string leesHelling() {
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

}