/* 
10bit Absolute Encoder Gray code decoder Example 
Codding by www.thaiconverter.com
The code is Free of use, feel free to use it ;)
We will happy if you use our code to make our World better.
*/

boolean pin_state[10];
byte input_pin[] = {2,3,4,5,6,7,8,9,10,11}; // Config Pins Input Here bit-1 to 10
int dec_position = 0;
int old_position = 0;
boolean a = 0;
void setup() {
pinMode(13, OUTPUT);

Serial.begin(9600);

for(byte i = 0; i <10; i = i +1 ){ // declare pin mode
  pinMode(input_pin[i], INPUT);
}

}

void loop() {
 Read_input(); // Read Encoder Signal
 delay(100);
 digitalWrite(13, HIGH);
 
}

void Read_input(){
  for(byte i = 0; i <10; i = i + 1 ){
    pin_state[i] = !digitalRead(input_pin[i]); // Bitwise for active LOW
  }
  
  // Gray Code Decoder
  dec_position = pin_state[9];
  for( int i = 8; i >= 0; i = i -1){
    dec_position = (dec_position << 1) | (pin_state[i] ^ (dec_position&0x1));
  }
  
  // Print 0-1023 position with line feed
  Serial.println(dec_position, DEC); 
  if(old_position == dec_position){
      digitalWrite(13, LOW);
  } else{
    digitalWrite(13, HIGH);
    
    };
  old_position = dec_position;
  
}
