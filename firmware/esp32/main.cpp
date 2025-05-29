#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_ADXL345_U.h>
#include <PulseSensorAmped.h>

// === 1. 핀 정의 및 임계값 ===
const int PIN_PPG       = A0;      // PPG 센서 아날로그 입력
const int PIN_LED       = 26;      // 후미등 LED
const int PIN_BUZZER    = 27;      // 피에조 스피커
const int PPG_THRESHOLD = 550;     // PPG 감지 임계값 (테스트 후 조정)

// === 2. 센서 객체 및 상태 변수 ===
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified();
PulseSensorAmped          pulseSensor;
bool                      collisionDetected = false;
bool                      pulseDetected     = false;

void setup() {
  Serial.begin(115200);

  // LED, Buzzer 핀 모드 설정
  pinMode(PIN_LED,    OUTPUT);
  pinMode(PIN_BUZZER, OUTPUT);

  // I2C 초기화 및 ADXL345 초기화
  Wire.begin();
  if (!accel.begin()) {
    Serial.println("ADXL345 not detected!");
    while (1);
  }
  accel.setRange(ADXL345_RANGE_16_G);

  // PPG 센서 초기화
  pulseSensor.analogInput(PIN_PPG);
  if (!pulseSensor.begin()) {
    Serial.println("PPG sensor not detected!");
    while (1);
  }
}

void loop() {
  readSensors();
  processLogic();
  updateOutputs();
  delay(50);
}

// === 3. 센서 읽기 ===
void readSensors() {
  // 3-1. PPG 센서 샘플 읽기
  int ppgValue = pulseSensor.getLatestSample();
  pulseDetected = (ppgValue > PPG_THRESHOLD);

  // 3-2. ADXL345 가속도 읽기
  sensors_event_t event;
  accel.getEvent(&event);
  collisionDetected = (event.acceleration.z < -19.6f);

  // 디버그용 시리얼 출력
  Serial.print("PPG: ");
  Serial.print(ppgValue);
  Serial.print("  Collision: ");
  Serial.println(collisionDetected ? "YES" : "NO");
}

// === 4. 로직 처리 ===
void processLogic() {
  // 추가 알고리즘이나 필터링이 필요하면 여기에 구현
}

// === 5. 출력 업데이트 ===
void updateOutputs() {
  if (collisionDetected) {
    // 충돌 시: 풀 브라이트 + 부저음 + 시리얼 메시지
    digitalWrite(PIN_LED, HIGH);
    tone(PIN_BUZZER, 1000);
    Serial.println("Collision!");
  }
  else if (pulseDetected) {
    // 심박 감지 시: 느린 깜빡임
    static unsigned long lastToggle = 0;
    if (millis() - lastToggle > 500) {
      digitalWrite(PIN_LED, !digitalRead(PIN_LED));
      lastToggle = millis();
      Serial.println("Pulse detected");
    }
    noTone(PIN_BUZZER);
  }
  else {
    // 평상 시: LED/부저 OFF
    digitalWrite(PIN_LED, LOW);
    noTone(PIN_BUZZER);
  }
}
