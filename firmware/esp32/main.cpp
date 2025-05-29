#include <Arduino.h>

// === 1. 핀 정의 ===
const int PIN_PPG    = A0;   // PPG 센서 (PulseSensor)
const int PIN_LED    = 26;   // 후미등 LED
const int PIN_BUZZER = 27;   // 피에조 스피커

// === 2. 함수 선언 (틀) ===
void readSensors();
void processLogic();
void updateOutputs();

void setup() {
  Serial.begin(115200);

  // 핀 모드 설정
  pinMode(PIN_LED,    OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  // TODO: 센서·BLE 초기화 자리
}

void loop() {
  readSensors();
  processLogic();
  updateOutputs();
  delay(50);
}

// === 3. 센서 읽기 ===
void readSensors() {
  // TODO: PPG 값 읽기 (analogRead)
  // TODO: 가속도 센서 읽기 (I2C)
}

// === 4. 로직 처리 ===
void processLogic() {
  // TODO: 심박 임계값, 충돌 감지 로직
}

// === 5. 출력 업데이트 ===
void updateOutputs() {
  // TODO: LED 점멸/상시점등 제어
  // TODO: 부저 경고음 제어
}
