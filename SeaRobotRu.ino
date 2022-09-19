int pot;
int out;

// Analog
#define PHOTO 2


// Digital
#define SRV_PIN 12

#define DS_PIN 4              // пин датчика 
#include "ds18b20minim.h"


#define PWM1 5
#define AIN1 7
#define BIN1 4


#include <Servo.h>
Servo servo;
int servo_position = 0;
Servo servo_1;
int servo_position_1 = 0;

#include "Parser.h"
#include "AsyncStream.h"  // асинхронное чтение сериал
AsyncStream<50> serial(&Serial, ';');   // указываем обработчик и стоп символ



void setup() {
  Serial.begin(115200);
  
  pinMode(BIN1,OUTPUT);
  pinMode(AIN1,OUTPUT);
  pinMode(PWM1,OUTPUT);
  
  servo.attach(SRV_PIN);

  dallas_begin(DS_PIN);
}

// с ардуино на пк, терминтаор \n
// 0,фоторез,термометр


// с пк на ардуино, терминтаор ;
// 0-1, Fancontrol
// 2-11,Servo



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
}

// функция парсинга, опрашивать в лупе
void parsing() {
  if (serial.available()) {
    Parser data(serial.buf, ',');  // отдаём парсеру
    int ints[10];           // массив для численных данных
    data.parseInts(ints);   // парсим в него

    switch (ints[0]) {      // свитч по ключу
      case 0:
        digitalWrite(BIN1,LOW);
        digitalWrite(AIN1,LOW);
        pot=analogRead(A5);
        out=map(0,0,0,0,0);
        analogWrite(PWM1,out); //Speed control of Motor A
        break;
      case 1:
        digitalWrite(BIN1,HIGH);
        digitalWrite(AIN1,HIGH);
        pot=analogRead(A5);
        out=map(850,0,1023,0,255);
        analogWrite(PWM1,out); //Speed control of Motor A
        break;
      case 2:
        servo.write(ints[1]);
        break;
      case 3:                     //Right
        servo_position = 30;
        servo.write(servo_position);
        break;
      case 4:                     //Left
        servo_position = 150;
        servo.write(servo_position);
        break;
      case 5:
        servo_position = 0;
        servo.write(servo_position);
        break;
      case 6:
        servo_position = 45;
        servo.write(servo_position);
        break;
      case 7:
        servo_position = 90;
        servo.write(servo_position);
        break;
      case 8:
        servo_position = 135;
        servo.write(servo_position);
        break;
      case 9:
        servo_position = 180;
        servo.write(servo_position);
        break;
      case 10:
        servo_position_1 = 30;
        servo.write(servo_position_1);
        break;
      case 11:
        servo_position_1 = 150;
        servo.write(servo_position_1);
        break;
      case 12:
        servo_position_1 = 0;
        servo.write(servo_position_1);
        break;
      case 13:
        servo_position_1 = 45;
        servo.write(servo_position_1); 
        break;
      case 14:
        servo_position_1 = 90;
        servo.write(servo_position_1); 
        break;
      case 15:
        servo_position_1 = 135;
        servo.write(servo_position_1); 
        break;
      case 16:
        servo_position_1 = 180;
        servo.write(servo_position_1); 
        break;  
      case 17:
        servo_1.write(ints[1]);
        break;  
    }
  }
}
        
