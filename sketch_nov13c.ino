#include <SoftwareSerial.h> 
#include <Servo.h>  
Servo myservo1;

int bluetoothTx = 10; 
int bluetoothRx = 11; 

SoftwareSerial bluetooth(bluetoothTx, bluetoothRx);

void setup()
{
  myservo1.attach(4); 
  
 
  Serial.begin(9600);

  
  bluetooth.begin(9600);
}

void loop()
{
 
  if(bluetooth.available()>= 2 )
  {
    unsigned int servopos = bluetooth.read();
    unsigned int servopos1 = bluetooth.read();
    unsigned int realservo = (servopos1 *256) + servopos;
    Serial.println(realservo);

    if (realservo >= 1000 && realservo <1180) {
      int servo1 = realservo;
      servo1 = map(servo1, 1000, 1180, 0, 180);
      myservo1.write(servo1);
      Serial.println("Servo 1 ON");
      delay(10);
    }
  }
}
