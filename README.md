# Smart Helmet Project

라이더의 안전 강화를 위해 PPG 센서와 가속도 센서를 활용하여 상태를 감지하고, BLE를 통해 모바일 앱과 연동하는 스마트 헬멧 프로토타입입니다.

## Overview
- PPG 센서를 통해 생체 신호 변화를 감지
- 가속도 센서를 통해 충돌 이벤트 감지
- LED 및 부저로 현장 경고 제공
- BLE를 통해 모바일 앱에 상태 알림 전송

## Tech Stack
- ESP32 / Arduino
- ADXL345
- PPG Sensor
- NimBLE
- Python (simulation)

## My Role
- 센서 연동 및 펌웨어 로직 구현
- 충돌/상태 판단 로직 작성
- BLE 알림 기능 구현
- 시뮬레이터 작성 및 테스트

## Limitations
- 심박수 BPM 계산이 아닌 이벤트 감지 중심 구현
- 단일 축 기반 충돌 판단으로 오탐 가능성 존재
- 향후 다축 센서 융합 및 모바일 앱 고도화 필요
