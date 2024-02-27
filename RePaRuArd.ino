#include "Parser.h"                    //Библиотека Гайвера для парсинга данных
#include "AsyncStream.h"               // асинхронное чтение сериал
AsyncStream<50> serial(&Serial, ';');  // Создаём объект и указываем обработчик и стоп символ



#include <TroykaIMU.h>        // Библиотека для IMU - датчика
Gyroscope gyroscope;          // Создаём объект для работы с гироскопом
Accelerometer accelerometer;  // Создаём объект для работы с акселерометром
Compass compass;              // Создаём объект для работы с магнитометром/компасом
Barometer barometer;          // Создаём объект для работы с барометром


//Библиотека для датчика температуры
#include "microDS18B20.h"
uint8_t addrTempBig[] = { 0x28, 0xFF, 0x64, 0x1E, 0xF7, 0x9E, 0xF4, 0x96 };
uint8_t addrTempLittle[] = { 0x28, 0x72, 0x8, 0x48, 0xF6, 0xF5, 0x3C, 0xD };
MicroDS18B20<A1, addrTempBig> dsBig;
MicroDS18B20<A1, addrTempLittle> dsLittle;

//Библеотека СЕРВО, для управления ШИМ_сигналом - используются ~ пины
#include "Servo.h"
Servo ESC;
Servo SER;
#define ESC_pin 3                   // = 3 = шим к ESC
#define SER_pin 5                   // = 5 = шим к Servo

void setup() {
  Serial.begin(115200);
  pinMode(13, 1);
  Serial.println("IMU Begin");      // Выводим сообщение о начале инициализации
  gyroscope.begin();                // Инициализируем гироскоп
  accelerometer.begin();            // Инициализируем акселерометр
  compass.begin();                  // Инициализируем компас
  barometer.begin();                // Инициализируем барометр
  ESC.attach(ESC_pin);
  ESC.writeMicroseconds(1600);
  delay(3000);
  ESC.writeMicroseconds(1360);
  delay(3000);
  SER.attach(SER_pin);
}
void loop() {
  parsing();
  Sensor_0();
  Accelerometer_1();
}
void Sensor_0() {
  static uint32_t tmr = 0;
  if (millis() - tmr > 800) {
    tmr = millis();
    Serial.print(0);
    Serial.print(',');
    dsBig.requestTemp();  //ЗапросТемпературы1.
    dsBig.readTemp();     //Читаем, если прочитано - выводим.
    Serial.print(dsBig.getTemp());
    Serial.print(',');
    dsLittle.requestTemp();  //ЗапросТемпературы2.
    dsLittle.readTemp();     //Читаем, если прочитано - выводим.
    Serial.println(dsLittle.getTemp());
  }
}
void Accelerometer_1() {
  static uint32_t tmr = 0;
  if (millis() - tmr > 100) {
    tmr = millis();
    Serial.print(1);
    Serial.print(',');
    Serial.print(accelerometer.readAccelerationAX());
    Serial.print(',');
    Serial.print(accelerometer.readAccelerationAY());
    Serial.print(',');
    Serial.println(accelerometer.readAccelerationAZ());
  }
}

// функция парсинга
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
        ESC.write(ints[1]);
        break;
      case 2:
        SER.write(ints[1]);
        break;
      case 3:
        ESC.write(2300);  //Максимум вперёд!
        break;
      case 4:
        ESC.write(1465);  //СТОП!
        break;
      case 5:
        ESC.write(800);  //Максимум назад!
        break;
    }
  }
}