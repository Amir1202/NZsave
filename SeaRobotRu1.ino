int out;
int pic = 255 - analogRead(0)/2;

// Analog
#define PHOTO 2

// Digital
#define SRV_PIN 1

#define DS_PIN 11              // пин датчика 
#include "ds18b20minim.h"

#include <TroykaIMU.h>
// Создаём объект для работы с гироскопом
Gyroscope gyroscope;
// Создаём объект для работы с акселерометром
Accelerometer accelerometer;
// Создаём объект для работы с магнитометром/компасом
Compass compass;
// Создаём объект для работы с барометром
Barometer barometer;

//мотор-1 
#define inA1 4   
#define inB1 9  
#define pwm1 6  

#define inA2 7   
#define inB2 8  
#define pwm2 5

#include "Parser.h"
#include "AsyncStream.h"  // асинхронное чтение сериал
AsyncStream<50> serial(&Serial, ';');   // указываем обработчик и стоп символ



void setup() {
  Serial.begin(115200);
  
  pinMode(inA1, OUTPUT);     
  pinMode(inB1, OUTPUT);     
  pinMode(pwm1, OUTPUT);   
   
  dallas_begin(DS_PIN);

  // Инициализируем гироскоп
  gyroscope.begin();
  // Инициализируем акселерометр
  accelerometer.begin();
  // Инициализируем компас
  compass.begin();
  // Инициализируем барометр
  barometer.begin();
}

// с ардуино на пк, терминтаор \n
// 0,фоторез,термометр


// с пк на ардуино, терминтаор ;
// 0-1,


void loop() {
  parsing();
  static uint32_t tmr = 0;
  if (millis() - tmr > 500) {
    tmr = millis();
    Serial.print(0);
    Serial.print(',');
    Serial.print(analogRead(PHOTO));
    Serial.print(',');
    dallas_requestTemp(DS_PIN);
    Serial.println(dallas_getTemp(DS_PIN));
  }
  static uint32_t tmr2 = 0;
  if (millis() - tmr2 > 100) {
    tmr2 = millis();
    Serial.print(1);
    Serial.print(',');
    Serial.print(gyroscope.readRotationDegX());
    Serial.print(',');
    Serial.print(gyroscope.readRotationDegY());
    Serial.print(',');
    Serial.print(gyroscope.readRotationDegZ());
    Serial.print(',');
    Serial.print(accelerometer.readAccelerationAX());
    Serial.print(',');
    Serial.print(accelerometer.readAccelerationAY());
    Serial.print(',');
    Serial.print(accelerometer.readAccelerationAZ());
    Serial.print(',');
    Serial.print(compass.readMagneticGaussX());
    Serial.print(',');
    Serial.print(compass.readMagneticGaussY());
    Serial.print(',');
    Serial.print(compass.readMagneticGaussZ());
    Serial.println();
  }
    static uint32_t tmr3 = 0;
  if (millis() - tmr3 > 100) {
    tmr3 = millis();
    Serial.print(2);
    Serial.print(',');
    Serial.print(barometer.readPressureMillimetersHg());
    Serial.print(',');
    Serial.print(barometer.readTemperatureC());
    Serial.println();
  }
}




// функция парсинга, опрашивать в LOOP
void parsing() {
  if (serial.available()) {
    Parser data(serial.buf, ',');  // отдаём парсеру
    int ints[10];           // массив для численных данных
    data.parseInts(ints);   // парсим в него


    switch (ints[0]) {      // свитч по ключу
      case 0:
//        servo.write(ints[1]);
        pic = map(pic, 0, 1023, -255, 225);
        if (pic > 0) {
          analogWrite(pwm1, pic);
          digitalWrite(inA1, 1);
          digitalWrite(inB1, 0);
        } else if (pic < 0){
          analogWrite(pwm1, 255 + pic);
          digitalWrite(inA1, 0);
          digitalWrite(inB1, 1);
        } else{
          analogWrite(pwm1, 0);
          digitalWrite(inA1, 0);
          digitalWrite(inB1, 0);
        }
    
        break;
      case 1:
        digitalWrite(inA1,HIGH);
        digitalWrite(inB1,LOW);
        out=map(600,600,600,600,600);
        analogWrite(pwm1,out); //Speed control of Motor A    
        break;
      case 2:
        digitalWrite(inA1,LOW);
        digitalWrite(inB1,HIGH);
        out=map(600,600,600,600,600);
        analogWrite(pwm1,out); //Speed control of Motor A    
        break;
      case 3:
        digitalWrite(inA1,HIGH);
        digitalWrite(inB1,LOW);
        out=map(300,300,300,300,300);
        analogWrite(pwm1,out); //Speed control of Motor A 
        delay(500);   
        analogWrite(pwm1, 0);
        digitalWrite(inA1, LOW);
        digitalWrite(inB1, LOW);                 
        break;
      case 4:
        digitalWrite(inA1,LOW);
        digitalWrite(inB1,HIGH);
        out=map(300,300,300,300,300);
        analogWrite(pwm1,out); //Speed control of Motor A 
        delay(500);   
        analogWrite(pwm1, 0);
        digitalWrite(inA1, LOW);
        digitalWrite(inB1, LOW);               
        break;
      case 5:
        digitalWrite(inA2,HIGH);
        digitalWrite(inB2,LOW);
        out=map(600,600,600,600,600);
        analogWrite(pwm2,out); //Speed control of Motor A      
        break;
      case 6:
        digitalWrite(inA2,LOW);
        digitalWrite(inB2,HIGH);
        out=map(600,600,600,600,600);
        analogWrite(pwm2,out); //Speed control of Motor A      
        break;
      case 7:
        digitalWrite(inA2,HIGH);
        digitalWrite(inB2,LOW);
        out=map(300,300,300,300,300);
        analogWrite(pwm2,out); //Speed control of Motor A 
        delay(500);   
        analogWrite(pwm2, 0);
        digitalWrite(inA2, LOW);
        digitalWrite(inB2, LOW);   
        break;
      case 8:
        digitalWrite(inA2,LOW);
        digitalWrite(inB2,HIGH);
        out=map(300,300,300,300,300);
        analogWrite(pwm2,out); //Speed control of Motor A 
        delay(500);   
        analogWrite(pwm2, 0);
        digitalWrite(inA2, LOW);
        digitalWrite(inB2, LOW);     
        break;
    }
  }
}
        
