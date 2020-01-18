#include "I2Cdev.h"
#include "MPU6050.h"
#include "Wire.h"

MPU6050 accelgyro;

int16_t ax, ay, az;

bool landscape_mode = true;
int weight = 10;

void setup() {
    Wire.begin();
    Serial.begin(9600);
    accelgyro.initialize();
    if (accelgyro.testConnection()) {
        Serial.println("MPU6050 connection failed");
    }
    pinMode(LED_BUILTIN, OUTPUT);
}

void loop() {
    bool orientation = readOrientation();
    if (orientation != landscape_mode) {
        if (++weight >= 10) {
            weight = 0;
            landscape_mode = orientation;
            if (landscape_mode) {
                Serial.println("l");
            } else {
                Serial.println("p");
            }
        }
    } else if (weight > 0) {
        weight--;
    }

    digitalWrite(LED_BUILTIN, !landscape_mode);
    delay(10);
}

void serialEvent() {
  while (Serial.available() > 0) {
    char incoming = (char) Serial.read();
    if (incoming == '?') {
        landscape_mode = readOrientation();
        if (landscape_mode) {
            Serial.println("l");
        } else {
            Serial.println("p");
        }
    }
  }
}

bool readOrientation() {
    accelgyro.getAcceleration(&ax, &ay, &az);
    return ax > 10000;
}
