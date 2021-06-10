// include the library code:
#include <LiquidCrystal.h>


// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

#define CTRL_3 13
typedef union{
  struct{
    unsigned char high;
    unsigned char low;
    }parcial;
  uint8_t total;
}INTEIRO;
INTEIRO motor,servo;
/*typedef union{
  struct{
    unsigned char high;
    unsigned char low;
    unsigned char high1;
    unsigned char low1;
    }parcial;
  float total;
}INTEIRO;
INTEIRO dado16,temp,umi;
*/

/* Table of CRC values for high–order byte */
static unsigned char auchCRCHi[] = {
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01,
0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0,
0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01,
0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81, 0x40, 0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41,
0x00, 0xC1, 0x81, 0x40, 0x01, 0xC0, 0x80, 0x41, 0x01, 0xC0, 0x80, 0x41, 0x00, 0xC1, 0x81,
0x40
} ;
/* Table of CRC values for low–order byte */
static char auchCRCLo[] = {
0x00, 0xC0, 0xC1, 0x01, 0xC3, 0x03, 0x02, 0xC2, 0xC6, 0x06, 0x07, 0xC7, 0x05, 0xC5, 0xC4,
0x04, 0xCC, 0x0C, 0x0D, 0xCD, 0x0F, 0xCF, 0xCE, 0x0E, 0x0A, 0xCA, 0xCB, 0x0B, 0xC9, 0x09,
0x08, 0xC8, 0xD8, 0x18, 0x19, 0xD9, 0x1B, 0xDB, 0xDA, 0x1A, 0x1E, 0xDE, 0xDF, 0x1F, 0xDD,
0x1D, 0x1C, 0xDC, 0x14, 0xD4, 0xD5, 0x15, 0xD7, 0x17, 0x16, 0xD6, 0xD2, 0x12, 0x13, 0xD3,
0x11, 0xD1, 0xD0, 0x10, 0xF0, 0x30, 0x31, 0xF1, 0x33, 0xF3, 0xF2, 0x32, 0x36, 0xF6, 0xF7,
0x37, 0xF5, 0x35, 0x34, 0xF4, 0x3C, 0xFC, 0xFD, 0x3D, 0xFF, 0x3F, 0x3E, 0xFE, 0xFA, 0x3A,
0x3B, 0xFB, 0x39, 0xF9, 0xF8, 0x38, 0x28, 0xE8, 0xE9, 0x29, 0xEB, 0x2B, 0x2A, 0xEA, 0xEE,
0x2E, 0x2F, 0xEF, 0x2D, 0xED, 0xEC, 0x2C, 0xE4, 0x24, 0x25, 0xE5, 0x27, 0xE7, 0xE6, 0x26,
0x22, 0xE2, 0xE3, 0x23, 0xE1, 0x21, 0x20, 0xE0, 0xA0, 0x60, 0x61, 0xA1, 0x63, 0xA3, 0xA2,
0x62, 0x66, 0xA6, 0xA7, 0x67, 0xA5, 0x65, 0x64, 0xA4, 0x6C, 0xAC, 0xAD, 0x6D, 0xAF, 0x6F,
0x6E, 0xAE, 0xAA, 0x6A, 0x6B, 0xAB, 0x69, 0xA9, 0xA8, 0x68, 0x78, 0xB8, 0xB9, 0x79, 0xBB,
0x7B, 0x7A, 0xBA, 0xBE, 0x7E, 0x7F, 0xBF, 0x7D, 0xBD, 0xBC, 0x7C, 0xB4, 0x74, 0x75, 0xB5,
0x77, 0xB7, 0xB6, 0x76, 0x72, 0xB2, 0xB3, 0x73, 0xB1, 0x71, 0x70, 0xB0, 0x50, 0x90, 0x91,
0x51, 0x93, 0x53, 0x52, 0x92, 0x96, 0x56, 0x57, 0x97, 0x55, 0x95, 0x94, 0x54, 0x9C, 0x5C,
0x5D, 0x9D, 0x5F, 0x9F, 0x9E, 0x5E, 0x5A, 0x9A, 0x9B, 0x5B, 0x99, 0x59, 0x58, 0x98, 0x88,
0x48, 0x49, 0x89, 0x4B, 0x8B, 0x8A, 0x4A, 0x4E, 0x8E, 0x8F, 0x4F, 0x8D, 0x4D, 0x4C, 0x8C,
0x44, 0x84, 0x85, 0x45, 0x87, 0x47, 0x46, 0x86, 0x82, 0x42, 0x43, 0x83, 0x41, 0x81, 0x80,
0x40
};

unsigned short CRC16 (unsigned char *puchMsg, unsigned short usDataLen){ /* The function returns the
CRC as a unsigned short type */

  unsigned char uchCRCHi = 0xFF ; /* high byte of CRC initialized */
  unsigned char uchCRCLo = 0xFF ; /* low byte of CRC initialized */
  unsigned uIndex ; /* will index into CRC lookup table */
  while (usDataLen--){ /* pass through message buffer */ 
    uIndex = uchCRCLo ^ *puchMsg++; /* calculate the CRC */
    uchCRCLo = uchCRCHi ^ auchCRCHi[uIndex];
    uchCRCHi = auchCRCLo[uIndex] ;
  }
  return (uchCRCLo << 8 | uchCRCHi);
}


void modbus(uint8_t dado){
    static int state = 0;
    uint8_t crc_High, crc_Low, vetor[30],ALS,WV;
    static uint8_t cont_byte;
    uint16_t crc16;
        
    switch(state){
      case 0:
        if(dado == 0x03){ //endereço do kit
          state = 1;
        }else{
          state = 0;
        }break;

      case 1:
        if(dado == 0x03){ //função ADs
          state = 2;
        }else if(dado == 0x10){ //modulo 4
          state = 12;
        }else{
          state = 0;
        }break;

      case 2:
        if(dado == 0x00){ //endereço inicial HIGH
          state = 3;
        }else{
          state = 0;
        }break;

      case 3:
        if(dado == 0x01){ //endereço inicial LOW
          state = 4;
        }else if(dado == 0x02){//ALS
          state = 8;
        }else{
          state = 0;
        }break;

      case 4:
        if(dado == 0x00){ //numero de estados HIGH
          state = 5;
        }else{
          state = 0;
        }break;

      case 5:
        if(dado == 0x01){ //numero de estados LOW
          state = 6;
        }else{
          state = 0;
        }break;
        
       case 6: 
          crc_High = dado;
          state = 7;
        break;
  
        case 7: 
          crc_Low = dado;
          state = 0;

          WV = analogRead(A0);;
          pos_vento(WV);
          lcd.setCursor(0,2);
          lcd.print(WV);
  
          vetor[0] = 0x03;
          vetor[1] = 0x03;
          vetor[2] = 0x02;
          vetor[3] = 0x00;
          vetor[4] = WV;

          crc16 = CRC16(vetor,5);
          
          vetor[5] = (crc16 >> 8) & 0xff;
          vetor[6] = crc16 & 0xff;
          controle_max('a'); //ENVIA ATRAVES DO MAX
          Serial.write(vetor,7);
          
        break;
        
        case 8:
        if(dado == 0x00){ //numero de estados HIGH
          state = 9;
        }else{
          state = 0;
        }break; 

        case 9:
        if(dado == 0x01){ //numero de estados LOW
          state = 10;
        }else{
          state = 0;
        }break;

        case 10: 
          crc_High = dado;
          state = 11;
        break;
  
        case 11: 
          crc_Low = dado;
          state = 0;

          ALS = analogRead(A1);;
          status_dia(ALS);
          lcd.setCursor(6,2);
          lcd.print(ALS);
  
          vetor[0] = 0x03;
          vetor[1] = 0x03;
          vetor[2] = 0x02;
          vetor[3] = 0x00;
          vetor[4] = ALS;

          crc16 = CRC16(vetor,5);
          
          vetor[5] = (crc16 >> 8) & 0xff;
          vetor[6] = crc16 & 0xff;
          controle_max('a'); //ENVIA ATRAVES DO MAX
          Serial.write(vetor,7);
          
        break;

        case 12:
        if(dado == 0x00){ //endereço inicial HIGH
          state = 13;
        }else{
          state = 0;
        }break;

      case 13:
        if(dado == 0x06){ //endereço inicial LOW
          state = 14;
        }else if(dado == 0x05){
          state = 21; //controle servo
        }else{
          state = 0;
        }break;

      case 14:
        if(dado == 0x00){ //numero de estados HIGH
          state = 15;
        }else{
          state = 0;
        }break;

      case 15:
        if(dado == 0x01){ //numero de estados LOW
          state = 16;
        }else{
          state = 0;
        }break;

      case 16:
        if(dado == 0x02){ //numero de bytes
          state = 17;
        }else{
          state = 0;
        }break;
   //Recebimento de dados
      case 17: 
         motor.parcial.low = dado;
         state = 18;
      break;

      case 18: 
        motor.parcial.high = dado;
        state = 19;
      break;

      case 19: 
        crc_High = dado;
        state = 20;
      break;

      case 20: 
        crc_Low = dado;
        state = 0;
        Serial1.write('a');
        Serial1.write(motor.total);

        vetor[0] = 0x03;
        vetor[1] = 0x10;
        vetor[2] = 0x00;
        vetor[3] = 0x06;
        vetor[4] = 0x00;
        vetor[5] = 0x01;
        
        crc16 = CRC16(vetor,6);
        
        vetor[6] = (crc16 >> 8) & 0xff;
        vetor[7] = crc16 & 0xff;
        controle_max('a'); //ENVIA ATRAVES DO MAX
        Serial.write(vetor,8);
        
      break;

      case 21:
        if(dado == 0x00){ //numero de estados HIGH
          state = 22;
        }else{
          state = 0;
        }break;

      case 22:
        if(dado == 0x01){ //numero de estados LOW
          state = 23;
        }else{
          state = 0;
        }break;

      case 23:
        if(dado == 0x02){ //numero de bytes
          state = 24;
        }else{
          state = 0;
        }break;
   //Recebimento de dados
      case 24: 
         servo.parcial.low = dado;
         state = 25;
      break;

      case 25: 
        servo.parcial.high = dado;
        state = 26;
      break;

      case 26: 
        crc_High = dado;
        state = 27;
      break;

      case 27: 
        crc_Low = dado;
        state = 0;
        Serial1.write('b');
        Serial1.write(servo.total);

        vetor[0] = 0x03;
        vetor[1] = 0x10;
        vetor[2] = 0x00;
        vetor[3] = 0x05;
        vetor[4] = 0x00;
        vetor[5] = 0x01;
        
        crc16 = CRC16(vetor,6);
        
        vetor[6] = (crc16 >> 8) & 0xff;
        vetor[7] = crc16 & 0xff;
        controle_max('a'); //ENVIA ATRAVES DO MAX
        Serial.write(vetor,8);
        
      break;

      
    }
}

void pos_vento(uint8_t dado){
  
  if(dado == 18){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("N");
  }else if(dado == 205){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("NE");
  }else if(dado == 93){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("L");
  }else if(dado == 185){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SE");
  }else if(dado == 31){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("S");
  }else if(dado == 118){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("SO");
  }else if(dado == 177){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("O");
  }else if(dado == 119){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("NO");
  }else{
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("--");
  }
}

void status_dia(uint8_t dado){
  
  if(dado > 100){
    lcd.clear();
    lcd.setCursor(6,0);
    lcd.print("SOL");
  }else if(dado > 43){
    lcd.clear();
    lcd.setCursor(6,0);
    lcd.print("PARC.NUB.");
  }else if(dado > 16){
    lcd.clear();
    lcd.setCursor(6,0);
    lcd.print("NUBLADO");
  }else{
    lcd.clear();
    lcd.setCursor(6,0);
    lcd.print("NOITE");
  }
}

void controle_max(char comando){
  if(comando == 'a'){ 
    digitalWrite(CTRL_3,HIGH);//envio pelo barramento
  }else{        
    digitalWrite(CTRL_3,LOW);//recepção pelo barramento
  }
}

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);
  Serial1.begin(9600);
  //delay(1000);

  pinMode(CTRL_3, OUTPUT);
}

void loop() {

  uint8_t dadoRX;
  controle_max('b');
  if (Serial.available() > 0) {
    dadoRX = Serial.read();
    modbus(dadoRX);
    }
  delay(15);
}
