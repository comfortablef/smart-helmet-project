#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ADXL345_U.h>
#include <PulseSensorAmped.h>
#include <NimBLEDevice.h>

// === 1. 핀 정의 및 임계값 ===
const int PIN_PPG       = A0;      // PPG 센서 아날로그 입력
const int PIN_LED       = 26;      // 후미등 LED
const int PIN_BUZZER    = 27;      // 피에조 스피커
const int PPG_THRESHOLD = 550;     // PPG 감지 임계값

// === 2. 센서 객체 및 상태 변수 ===
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();
PulseSensorAmped          pulseSensor;
bool                      collisionDetected = false;
bool                      pulseDetected     = false;

// === 3. BLE UUID / 객체 선언 ===
#define SERVICE_UUID         "12345678-1234-1234-1234-1234567890ab"
#define CHAR_UUID_HR         "12345678-1234-1234-1234-1234567890ac"
#define CHAR_UUID_COLLISION  "12345678-1234-1234-1234-1234567890ad"

NimBLEServer*         pServer        = nullptr;
NimBLEService*        pService       = nullptr;
NimBLECharacteristic* pCharHR        = nullptr;
NimBLECharacteristic* pCharCollision = nullptr;

void setup() {
  Serial.begin(115200);

  // --- LED / Buzzer 핀 모드 ---
  pinMode(PIN_LED,    OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  // --- ADXL345 초기화 ---
  Wire.begin();
  if (!accel.begin()) {
    Serial.println("ADXL345 not detected!");
    while (1);
  }
  accel.setRange(ADXL345_RANGE_16_G);

  // --- PPG 센서 초기화 ---
  pulseSensor.analogInput(PIN_PPG);
  if (!pulseSensor.begin()) {
    Serial.println("PPG sensor not detected!");
    while (1);
  }

  // --- BLE 초기화 ---
  NimBLEDevice::init("SmartHelmet");
  pServer  = NimBLEDevice::createServer();
  pService = pServer->createService(SERVICE_UUID);

  pCharHR = pService->createCharacteristic(
    CHAR_UUID_HR,
    NIMBLE_PROPERTY::NOTIFY
  );
  pCharCollision = pService->createCharacteristic(
    CHAR_UUID_COLLISION,
    NIMBLE_PROPERTY::NOTIFY
  );

  pService->start();
  NimBLEAdvertising* pAdv = NimBLEDevice::getAdvertising();
  pAdv->addServiceUUID(SERVICE_UUID);
  pAdv->start();
  Serial.println("BLE advertising started");
}

void loop() {
  readSensors();
  updateOutputs();
  bleNotify();
  delay(50);
}

// === 4. 센서 읽기 ===
void readSensors() {
  // PPG
  int ppgValue = pulseSensor.getLatestSample();
  pulseDetected = (ppgValue > PPG_THRESHOLD);

  // 충돌
  sensors_event_t event;
  accel.getEvent(&event);
  collisionDetected = (event.acceleration.z < -19.6f);

  // 디버그 시리얼
  Serial.print("PPG: ");
  Serial.print(ppgValue);
  Serial.print("  Collision: ");
  Serial.println(collisionDetected ? "YES" : "NO");
}

// === 5. 출력 제어 ===
void updateOutputs() {
  if (collisionDetected) {
    digitalWrite(PIN_LED, HIGH);
    tone(PIN_BUZZER, 1000);
    Serial.println("Collision!");
  }
  else if (pulseDetected) {
    static unsigned long lastToggle = 0;
    if (millis() - lastToggle > 500) {
      digitalWrite(PIN_LED, !digitalRead(PIN_LED));
      lastToggle = millis();
      Serial.println("Pulse detected");
    }
    noTone(PIN_BUZZER);
  }
  else {
    digitalWrite(PIN_LED, LOW);
    noTone(PIN_BUZZER);
  }
}

// === 6. BLE Notify ===
void bleNotify() {
  static bool lastPulse = false;
  static bool lastCollision = false;

  if (pulseDetected != lastPulse) {
    lastPulse = pulseDetected;
    uint8_t val = pulseDetected ? 1 : 0;
    pCharHR->setValue(&val, 1);
    pCharHR->notify();
  }
  if (collisionDetected != lastCollision) {
    lastCollision = collisionDetected;
    uint8_t val = collisionDetected ? 1 : 0;
    pCharCollision->setValue(&val, 1);
    pCharCollision->notify();
  }
}
