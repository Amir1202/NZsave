#define LED_R 44
#define LED_G 45
#define LED_B 46

// #define IN1 2
// #define PWM1 3
// #define IN2 4
// #define PWM2 5

#include "GyverMotor.h"
GMotor motorL(DRIVER2WIRE, 2, 3, HIGH);
GMotor motorR(DRIVER2WIRE, 4, 5, HIGH);

#include "Parser.h"
#include "AsyncStream.h"               // асинхронное чтение сериал
AsyncStream<50> serial(&Serial, ';');  // указываем обработчик и стоп символ



void setup() {
  Serial.begin(115200);
  pinMode(13, 1);
  pinMode(LED_R, 1);
  pinMode(LED_G, 1);
  pinMode(LED_B, 1);
  // pinMode(IN1, 1);
  // pinMode(PWM1, 1);
  // pinMode(IN2, 1);
  // pinMode(PWM2, 1);
  motorL.setMinDuty(110);
  motorR.setMinDuty(110);
}

void loop() {
  parsing();
  static uint32_t tmr = 0;
  if (millis() - tmr > 100) {
    tmr = millis();
  }
}

// функция парсинга, опрашивать в лупе
void parsing() {
  if (serial.available()) {
    Parser data(serial.buf, ',');  // отдаём парсеру
    int ints[10];                  // массив для численных данных
    data.parseInts(ints);  // парсим в него
    switch (ints[0]) {
      case 0:
        digitalWrite(13, ints[1]);
        break;
      case 1:
        analogWrite(LED_R, ints[1]);
        analogWrite(LED_G, ints[2]);
        analogWrite(LED_B, ints[3]);
        break;
      case 2:
        motorL.setMode(FORWARD);
        motorL.setSpeed(255);
        motorR.setMode(FORWARD);
        motorR.setSpeed(255);
        break;
      case 3:
        motorL.setMode(BACKWARD);
        motorL.setSpeed(255);
        motorR.setMode(BACKWARD);
        motorR.setSpeed(255);
        break;
      case 4:
        motorL.setMode(STOP);
        motorR.setMode(STOP);
        break;
      case 5:

        break;
      case 6:
          motorL.setMode(AUTO);
          motorL.setSpeed(ints[1]);
        break;
      case 7:
          motorR.setMode(AUTO);
          motorR.setSpeed(ints[1]);
        break;
    }
  }
}
      // if (ints[1] > 0){
      //   analogWrite(IN1, ints[1]);
      //   analogWrite(IN2, ints[1]);
      //   digitalWrite(PWM1, 0);
      //   digitalWrite(PWM2, 0);
      // } else if (ints[1] < 0){
      //   analogWrite(IN1, 255 + ints[1]);
      //   analogWrite(IN2, 255 + ints[1]);
      //   digitalWrite(PWM1, 1);
      //   digitalWrite(PWM2, 1);
      // } else {
      //   digitalWrite(IN1, 0);
      //   digitalWrite(PWM1, 0);
      //   digitalWrite(IN2, 0);
      //   digitalWrite(PWM2, 0);
      // }