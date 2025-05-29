#include <Arduino.h>
#include <Adafruit_ADXL345_U.h>

const int PIN_LED    = 26;
const int PIN_BUZZER = 27;

Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();
bool collisionDetected = false;

void setup() {
  Serial.begin(115200);
  pinMode(PIN_LED,    OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);
  if (!accel.begin()) {
    Serial.println("ADXL345 not detected!");
    while (1);
  }
  accel.setRange(ADXL345_RANGE_16_G);
}

void loop() {
  readSensors();
  updateOutputs();
  delay(50);
}

void readSensors() {
  sensors_event_t event;
  accel.getEvent(&event);
  collisionDetected = (event.acceleration.z < -19.6f);
}

void updateOutputs() {
  if (collisionDetected) {
    digitalWrite(PIN_LED, HIGH);
    tone(PIN_BUZZER, 1000);
    Serial.println("Collision!");
  } else {
    digitalWrite(PIN_LED, LOW);
    noTone(PIN_BUZZER);
  }
}
