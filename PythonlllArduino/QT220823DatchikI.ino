#include "Parser.h"                    //Библиотека Гайвера для парсинга данных
#include "AsyncStream.h"               // асинхронное чтение сериал
AsyncStream<50> serial(&Serial, ';');  // указываем обработчик и стоп символ


#define LED_R 6
#define LED_G 9
#define LED_B 11

//Для датчика температуры - используется "DIGITAL" пин 34
#include "microDS18B20.h"
MicroDS18B20<4> ds;

//Для моторов - используется "DIGITAL" !!ШИМ!! пин 2,3,4,5 (Можно использовать один шим пин для одного мотора PWM)
#include "GyverMotor.h"
GMotor motorL(DRIVER2WIRE, 2, 3, HIGH);
GMotor motorR(DRIVER2WIRE, 4, 5, HIGH);

void setup() {
  Serial.begin(115200);
  pinMode(13, 1);
  pinMode(LED_R, 1);
  pinMode(LED_G, 1);
  pinMode(LED_B, 1);
  motorL.setMinDuty(110);  //Минимальная Скорость Вращения Левого Мотора
  motorR.setMinDuty(110);  //Минимальная Скорость Вращения Правого Мотора
}

void loop() {
  parsing();
  static uint32_t tmr = 0;  
  if (millis() - tmr > 1000) {
    tmr = millis();
    Serial.print(0);
    Serial.print(',');
    ds.requestTemp();  //ЗапросТемпературы.
    ds.readTemp();     //Читаем, если прочитано - выводим.
    Serial.print(ds.getTemp());
    Serial.print(',');
    Serial.println("1000");
  }
}
// функция парсинга, опрашивать в лупе
void parsing() {
  if (serial.available()) {
    Parser data(serial.buf, ',');  // отдаём парсеру
    int ints[10];                  // массив для численных данных
    data.parseInts(ints);          // парсим в него
    switch (ints[0]) {
      case 0:
        digitalWrite(13, ints[1]);
        break;
      case 1:
        analogWrite(LED_R, ints[1]);
        analogWrite(LED_G, ints[2]);
        analogWrite(LED_B, ints[3]);
        break;
      case 2:  //Вперёд
        motorL.setMode(FORWARD);
        motorL.setSpeed(255);
        motorR.setMode(FORWARD);
        motorR.setSpeed(255);
        break;
      case 3:  //Назад
        motorL.setMode(BACKWARD);
        motorL.setSpeed(255);
        motorR.setMode(BACKWARD);
        motorR.setSpeed(255);
        break;
      case 4:  //СТОП
        motorL.setMode(STOP);
        motorR.setMode(STOP);
        break;
      case 5:
        motorL.setMode(FORWARD);  //Налево
        motorL.setSpeed(255);
        motorR.setMode(STOP);
        break;
      case 6:
        motorR.setMode(FORWARD);  //Направо
        motorR.setSpeed(255);
        motorL.setMode(STOP);
        break;
      case 7:
        motorL.setMode(AUTO);  //ПлавноНАЛЕВО
        motorL.setSpeed(ints[1]);
        break;
      case 8:
        motorR.setMode(AUTO);  //ПлавноНАПРАВО
        motorR.setSpeed(ints[1]);
        break;
    }
  }
}