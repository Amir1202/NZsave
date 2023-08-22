#define LED_R 44
#define LED_G 45
#define LED_B 46



///////////////////////////////////////////////////Для ультразвукового датчика расстояния - используется "DIGITAL" пины 22, 23, 24
#include <NewPing.h>
NewPing sonar(TRIG, ECHO, 400);
float dist_3[3] = { 0.0, 0.0, 0.0 };  // массив для хранения трёх последних измерений
float middle, dist, dist_filtered;
float k;
byte i, delta;
unsigned long sensTimer;
#define ECHO 24
#define TRIG 23
#define sensVCC 22

/////////////////////////////////////////////////// Для датчика температуры - используется "DIGITAL" пин 34
#include "microDS18B20.h"
MicroDS18B20<34> ds;


// #define IN1 2
// #define PWM1 3
// #define IN2 4
// #define PWM2 5

/////////////////////////////////////////////////// Для моторов - используется "DIGITAL" !!ШИМ!! пин 2,3,4,5                    (Можно использовать один шим пин для одного мотора PWM)
#include "GyverMotor.h"
GMotor motorL(DRIVER2WIRE, 2, 3, HIGH);
GMotor motorR(DRIVER2WIRE, 4, 5, HIGH);


#include "Parser.h"                    //Библиотека Гайвера для парсинга данных
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
  pinMode(sensVCC, OUTPUT);
  digitalWrite(sensVCC, 1);

  motorL.setMinDuty(110);            //Минимальная Скорость Вращения Левого Мотора
  motorR.setMinDuty(110);            //Минимальная Скорость Вращения Правого Мотора
}

void loop() {
  parsing();
  static uint32_t tmr = 0;                                  //Таймер опрос которого происходит каждые 400 миллисекунд
  if (millis() - tmr > 400) {
    tmr = millis();
    Serial.print(0);
    Serial.print(',');
    ds.requestTemp();  //ЗапросТемпературы.
    ds.readTemp();     //Читаем, если прочитано - выводим.
    Serial.println(ds.getTemp());
  }
  static uint32_t tmr2 = 0;
  if (millis() - tmr2 > 100) {
    tmr2 = millis();
    Serial.print(1);
    Serial.print(',');
    if (i > 1) i = 0;
    else i++;
    dist_3[i] = (float)sonar.ping() / 57.5;               // получить расстояние в текущую ячейку массива
    dist = middle_of_3(dist_3[0], dist_3[1], dist_3[2]);  // фильтровать медианным фильтром из 3ёх последних измерений
    delta = abs(dist_filtered - dist);                    // расчёт изменения с предыдущим
    if (delta > 1) k = 0.7;                               // если большое - резкий коэффициент
    else k = 0.1;                                         // если маленькое - плавный коэффициент
    dist_filtered = dist * k + dist_filtered * (1 - k);   // фильтр "бегущее среднее"
    Serial.println(dist);                                 //В конце опроса любого датчика, нужно писать "println"
  }
}


//ФИЛЬТРдляДАТЧИКАРАССТОЯНИЯ
float middle_of_3(float a, float b, float c) {
  if ((a <= b) && (a <= c)) {
    middle = (b <= c) ? b : c;
  } else {
    if ((b <= a) && (b <= c)) {
      middle = (a <= c) ? a : c;
    } else {
      middle = (a <= b) ? a : b;
    }
  }
  return middle;
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