// ------------------------ BIBLIOTECAS
#include <LiquidCrystal.h>
#include <DHT.h>

//---------------------------------------- PINOS e INICIALIZAÇÕES
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

//-----------------  DEFS
#define SDA 10 
#define CTRL_1 13

#define LED_1 9
#define LED_2 8
#define LED_3 7
#define LED_4 6

#define CH_1 A0
#define CH_2 A1
#define CH_3 A2
#define CH_4 A3

#define DHTTYPE DHT11

DHT dht(SDA, DHTTYPE);

//------------------------- UNION 32 bits em 4 dados de 8 bits
// union serve para separar um dado de 32bits em 4 dados de 8 bits, ajuda na transmissao de float
typedef union{
  struct{
    unsigned char high;
    unsigned char low;
    unsigned char high1;
    unsigned char low1;
    }parcial;
  float total;
}INTEIRO;
INTEIRO dado16,TEMP16, UMI16,temp,umi;

//--------------------------------------------------- LEITURA DHT11
void LEITURA_DHT11(void){
  TEMP16.total = dht.readTemperature();
  UMI16.total = dht.readHumidity();
  Serial.print("Temp:  ");
  Serial.print((int)TEMP16.total);
  Serial.println("oC");
  Serial.print("UMI:  ");
  Serial.print((int)UMI16.total);
  Serial.println("%");
}

//------------------------------------------------------------------------------------------ FUNÇÃO ACIONA LEDS
void ACIONA_LEDS (uint8_t comando_LEDS){
  if (comando_LEDS & (1<<0)) digitalWrite(LED_1, HIGH);
  else digitalWrite(LED1, LOW);
  if (comando_LEDS & (1<<1)) digitalWrite(LED_2, HIGH);
  else digitalWrite(LED2, LOW);
  if (comando_LEDS & (1<<2)) digitalWrite(LED_3, HIGH);
  else digitalWrite(LED3, LOW);
  if (comando_LEDS & (1<<3)) digitalWrite(LED_4, HIGH);
  else digitalWrite(LED4, LOW);
}

//------------------------------------------------------------------------------------------ CALCULO CRC
//--------------------------------------------------------  TABELAS CRC
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

//--------------------------------------------------------  FUNÇÃO QUE CALCULA CRC
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

//------------------------------------------------------------------------------------------ FUNÇÃO MODBUS
void modbus(uint8_t dado){
    static int state = 0;
    uint8_t crc_High, crc_Low, vetor[30],a,b,c,d;
    static uint8_t cont_byte;
    uint16_t crc16;
    static char msg[30];
    int i = 0;
    
    switch(state){
      case 0:
        if(dado == 0x01){ //endereço do kit
          state = 1;
        }else{
          state = 0;
        }break;

      case 1:
        if(dado == 0x10){ //função Float ou LCD
          state = 2;
        }
        else if(dado == 0x0F){ //função Leds
          state = 13;
        }else if(dado == 0x02){ //função Switches
          state = 21;
        }else if(dado == 0x03){ //função SHT15
          state = 40;
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
        if(dado == 0x06){ //endereço inicial LOW
          state = 4;
        }else if(dado == 0x00){ //endereço para print na linha superior display
          state = 27;
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
        if(dado == 0x02){ //numero de estados LOW
          state = 6;
        }else{
          state = 0;
        }break;

      case 6:
        if(dado == 0x04){ //numero de bytes
          state = 7;
        }else{
          state = 0;
        }break;
   //Recebimento de dados
      case 7: 
         dado16.parcial.low = dado;
         state = 8;
      break;

      case 8: 
        dado16.parcial.high = dado;
        state = 9;
      break;

      case 9: 
        dado16.parcial.low1 = dado;
        state = 10;
      break;

      case 10: 
        dado16.parcial.high1 = dado;
        state = 11;

        lcd.setCursor(0,0);
        lcd.print(dado16.total);
      break;

      case 11: 
        crc_High = dado;
        state = 12;
      break;

      case 12: 
        crc_Low = dado;
        state = 0;

        vetor[0] = 0x01;
        vetor[1] = 0x10;
        vetor[2] = 0x00;
        vetor[3] = 0x06;
        vetor[4] = 0x00;
        vetor[5] = 0x02;
        
        crc16 = CRC16(vetor,6);
        
        vetor[6] = (crc16 >> 8) & 0xff;
        vetor[7] = crc16 & 0xff;
        controle_max('a'); //ENVIA ATRAVES DO MAX
        Serial.write(vetor,8);
        
      break;
      //função Leds
      case 13:
        if(dado == 0x00){ //endereço inicial HIGH
          state = 14;
        }else{
          state = 0;
        }
      break;

      case 14:
        if(dado == 0x01){ //endereço inicial LOW
          state = 15;
        }else{
          state = 0;
        }
      break;

      case 15:
        if(dado == 0x00){ //número de estados HIGH
          state = 16;
        }else{
          state = 0;
        }
      break;

      case 16:
        if(dado == 0x04){ //número de estados LOW
          state = 17;
        }else{
          state = 0;
        }
      break;

      case 17:
        if(dado == 0x01){ //numero de bytes
          state = 18;
        }else{
          state = 0;
        }
      break;

      case 18:
          toogleLED(dado);
          lcd.setCursor(12,0);
          lcd.print(dado,BIN);
          state = 19;
      break;

      case 19: 
        crc_High = dado;
        state = 20;
      break;

      case 20: 
        crc_Low = dado;
        state = 0;

        vetor[0] = 0x01;
        vetor[1] = 0x0F;
        vetor[2] = 0x00;
        vetor[3] = 0x01;
        vetor[4] = 0x00;
        vetor[5] = 0x04;
        
        crc16 = CRC16(vetor,6);
        
        vetor[6] = (crc16 >> 8) & 0xff;
        vetor[7] = crc16 & 0xff;
        controle_max('a'); //ENVIA ATRAVES DO MAX
        Serial.write(vetor,8);
        
      break;
      //função Switches
      case 21:
        if(dado == 0x00){ //endereço inicial HIGH
          state = 22;
        }else{
          state = 0;
        }break;

        case 22:
        if(dado == 0x02){ //endereço inicial LOW
          state = 23;
        }else{
          state = 0;
        }break;

        case 23:
        if(dado == 0x00){ //número de estados HIGH
          state = 24;
        }else{
          state = 0;
        }break;

        case 24:
        if(dado == 0x04){ //número de estados LOW
          state = 25;
        }else{
          state = 0;
        }break;

        case 25: 
        crc_High = dado;
        state = 26;
        break;

        case 26:
        crc_Low = dado;
        state = 0;

        a = digitalRead(CH_1);
        b = digitalRead(CH_2);
        c = digitalRead(CH_3);
        d = digitalRead(CH_4);
        
        vetor[0] = 0x01;
        vetor[1] = 0x02;
        vetor[2] = 0x01;
        vetor[3] = ((d << 3) | (c << 2) | (b << 1) | (a << 0));
        crc16 = CRC16(vetor,4);
        vetor[4] = (crc16 >> 8) & 0xff;
        vetor[5] = crc16 & 0xff;
        
        lcd.setCursor(12,2);
        lcd.print(vetor[3],BIN);
        
        controle_max('a'); //ENVIA ATRAVES DO MAX
        Serial.write(vetor,6);
        
        break;

        case 27:
        if(dado == 0x00){ //numero de estados HIGH
          state = 28;
        }else{
          state = 0;
        }break;

        case 28:
        if(dado == 0x08){ //numero de estados LOW
          state = 29;
        }else{
          state = 0;
        }break;

        case 29:
        if(dado == 0x10){ //numero de bytes
          cont_byte = dado;
          state = 30;
        }else{
          state = 0;
        }break;

        case 30:
          msg[0] = dado;   
          state = 31;
        break;

        case 31:
          msg[1] = dado;
          state = 32;
        break; 

        case 32:
          msg[2] = dado;
          state = 33;
        break;

        case 33:
          msg[3] = dado;
          state = 34;
        break;

        case 34:
          msg[4] = dado;
          state = 35;
        break;

        case 35:
          msg[5] = dado;
          state = 36;
        break;

        case 36:
          msg[6] = dado;
          state = 37;
        break;

        case 37:
            msg[7] = dado;
            //lcd.setCursor(0,0);
            //lcd.print(msg);
            state = 38;
        break;
  
        case 38: 
          crc_High = dado;
          state = 39;
        break;
  
        case 39: 
          crc_Low = dado;
          state = 0;
  
          vetor[0] = 0x01;
          vetor[1] = 0x10;
          vetor[2] = 0x00;
          vetor[3] = 0x00;
          vetor[4] = 0x00;
          vetor[5] = 0x08;
          
          crc16 = CRC16(vetor,6);
          
          vetor[6] = (crc16 >> 8) & 0xff;
          vetor[7] = crc16 & 0xff;
          controle_max('a'); //ENVIA ATRAVES DO MAX
          Serial.write(vetor,8);
          
        break;
        //Função SHT15
        case 40:
          if(dado == 0x00){ //endereço inicial HIGH
            state = 41;
          }else{
            state = 0;
        }break;

        case 41:
          if(dado == 0x04){ //endereço inicial LOW
            state = 42;
          }else if(dado == 0x05){ //endereço umidade
            state = 46;
          }else{
            state = 0;
        }break;

        case 42:
          if(dado == 0x00){ //número de estados HIGH
            state = 43;
          }else{
            state = 0;
        }break;

        case 43:
          if(dado == 0x03){ //número de estados LOW
            state = 44;
          }else{
            state = 0;
        }break;
        
       case 44: 
          crc_High = dado;
          state = 45;
        break;
  
        case 45: 
          crc_Low = dado;
          state = 0;

          temp.total = dht.readTemperature();
          lcd.setCursor(0,2);
          lcd.print(temp.total);
  
          vetor[0] = 0x01;
          vetor[1] = 0x03;
          vetor[2] = 0x06;
          vetor[3] = 0x00;
          vetor[4] = 0x01;
          vetor[5] = temp.parcial.low;
          vetor[6] = temp.parcial.high;
          vetor[7] = temp.parcial.low1;
          vetor[8] = temp.parcial.high1;
          
          
          crc16 = CRC16(vetor,9);
          
          vetor[9] = (crc16 >> 8) & 0xff;
          vetor[10] = crc16 & 0xff;
          controle_max('a'); //ENVIA ATRAVES DO MAX        
          Serial.write(vetor,11);
          
        break; 

        case 46:
          if(dado == 0x00){ //número de estados HIGH
            state = 47;
          }else{
            state = 0;
        }break;

        case 47:
          if(dado == 0x03){ //número de estados LOW
            state = 48;
          }else{
            state = 0;
        }break;
        
       case 48: 
          crc_High = dado;
          state = 49;
        break;
  
        case 49: 
          crc_Low = dado;
          state = 0;

          umi.total = dht.readHumidity();
          lcd.setCursor(6,2);
          lcd.print(umi.total);
  
          vetor[0] = 0x01;
          vetor[1] = 0x03;
          vetor[2] = 0x06;
          vetor[3] = 0x00;
          vetor[4] = 0x02;
          vetor[5] = umi.parcial.low;
          vetor[6] = umi.parcial.high;
          vetor[7] = umi.parcial.low1;
          vetor[8] = umi.parcial.high1;
    
          crc16 = CRC16(vetor,9);
          
          vetor[9] = (crc16 >> 8) & 0xff;
          vetor[10] = crc16 & 0xff;
          controle_max('a'); //ENVIA ATRAVES DO MAX
          Serial.write(vetor,11);
          
        break;
    }
}

void controle_max(char comando){
    
  if(comando == 'a'){
    digitalWrite(CTRL_1,HIGH);//envio pelo barramento
    //delay(2);
  }else{         
    digitalWrite(CTRL_1,LOW);//recepção pelo barramento
    //delay(2);
  }
}

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  Serial.begin(9600);
  dht.begin();

  pinMode(CTRL_1, OUTPUT);

  pinMode(LED_4, OUTPUT);
  pinMode(LED_3, OUTPUT);
  pinMode(LED_2, OUTPUT);
  pinMode(LED_1, OUTPUT);

  pinMode(CH_1, INPUT);
  pinMode(CH_2, INPUT);
  pinMode(CH_3, INPUT);
  pinMode(CH_4, INPUT);
}

void loop() {

  uint8_t dadoRX;

  controle_max('b');
  if (Serial.available() > 0) {
    dadoRX = Serial.read();
    modbus(dadoRX);
  }
    delay(17);
}
