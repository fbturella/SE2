#include <LiquidCrystal.h>
#include <Servo.h>

Servo servomotor;

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

typedef union{
  struct{
    unsigned char high;
    unsigned char low;
    }parcial;
  int total;
}INTEIRO;
INTEIRO motor,servo;

void aciona_servo(int valor){
   int pos = map(valor,0,360,0,255);
   servomotor.write(pos);
   delay(10);
   lcd.setCursor(0,2);
   lcd.print(servo.total);
   lcd.print("%");
   lcd.setCursor(0,0);
   lcd.print(pos);
}

void setup() {
  // initialize both serial ports:
  lcd.begin(16, 2);
  Serial.begin(9600);
  servomotor.attach(6);
}

void loop() {
  
  uint8_t dadoRX;

  if (Serial.available() > 0) {
    dadoRX = Serial.read(); 
    if (dadoRX == 'a'){
      motor.total = Serial.read();   
    }else if (dadoRX == 'b'){
      servo.total = Serial.read();   
    }    
  }
  aciona_servo(servo.total);
    //lcd.clear();
    //lcd.setCursor(0,0);
    //lcd.print(motor.total);
    //delay(100);
}
