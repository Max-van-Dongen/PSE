#include <Zumo32U4.h>
#include "HellingSensor.h"

Zumo32U4IMU sensor; 

HellingSensor::HellingSensor() {
    sensor.init();
    sensor.enableDefault();
}

void HellingSensor::leesHelling() {
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
