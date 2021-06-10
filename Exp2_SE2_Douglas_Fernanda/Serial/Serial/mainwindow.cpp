#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QSerialPort>
#include <QtDebug>
#include <QTimer>
#include <QPixmap>

QTimer *timer = new QTimer(); //cria novo timer
int isConected = 0;
uint8_t led_state = 0;

typedef union{
    struct{
        unsigned char high;
        unsigned char low;
        unsigned char high1;
        unsigned char low1;
    }parcial;
    float total;
}INTEIRO;
INTEIRO dado16,temp_sht,umi_sht;

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
static unsigned char auchCRCLo[] = {
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

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    serial = new QSerialPort(this);
    connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);
    connect(timer, SIGNAL(timeout()),SLOT(timer_teste()));
    QPixmap luz_acesa ("C:/Users/Leonardo/Desktop/Embarcados II/T1_modbus/Serial/luz_acesa.png");
    QPixmap luz_apagada ("C:/Users/Leonardo/Desktop/Embarcados II/T1_modbus/Serial/luz_apagada.png");
    ui->led1_ind->setPixmap(luz_apagada.scaled(71,61,Qt::KeepAspectRatio));
    ui->led2_ind->setPixmap(luz_apagada.scaled(71,61,Qt::KeepAspectRatio));
    ui->led3_ind->setPixmap(luz_apagada.scaled(71,61,Qt::KeepAspectRatio));
    ui->led4_ind->setPixmap(luz_apagada.scaled(71,61,Qt::KeepAspectRatio));
    atualizarTela();
}

unsigned short CRC16 (unsigned char *puchMsg, unsigned short usDataLen){ /* The function returns the CRC as a unsigned short type */
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

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_clicked()
{
    if(isConected) {
        desconectar();
    } else {
        conectar();
    }
    atualizarTela();

}

void MainWindow::timer_teste()
{
    Switches();
    Temperatura();
    Umidade();
    //qDebug("Entra nesta função a cada 1s");
}
void MainWindow::readData()
{
    char buffer[10];
    serial->read(buffer,1);
    modbus(buffer[0]);

}

void MainWindow::on_pushButton_2_clicked()
{
    unsigned char buffer[26];
    char auxiliar[16];
    QString frase;
    unsigned short crc;
    //buffer[0]='1';
    frase = ui->lineEdit->text();
    strcpy(auxiliar,frase.toLatin1());

    buffer[0] = 0x01;
    buffer[1] = 0x10;
    buffer[2] = 0x00;
    buffer[3] = 0x00;
    buffer[4] = 0x00;
    buffer[5] = 0x08;
    buffer[6] = 0x10;
    buffer[7] = auxiliar[0];
    buffer[8] = auxiliar[1];
    buffer[9] = auxiliar[2];
    buffer[10] = auxiliar[3];
    buffer[11] = auxiliar[4];
    buffer[12] = auxiliar[5];
    buffer[13] = auxiliar[6];
    buffer[14] = auxiliar[7];
    buffer[15] = auxiliar[8];
    buffer[16] = auxiliar[9];
    buffer[17] = auxiliar[10];
    buffer[18] = auxiliar[11];
    buffer[19] = auxiliar[12];
    buffer[20] = auxiliar[13];
    buffer[21] = auxiliar[14];
    buffer[22] = auxiliar[15];

    crc = CRC16(buffer,23);

    buffer[23] = (crc >> 8) & 0xff;
    buffer[24] = crc & 0xff;

    if(serial->isOpen())// se porta aberta
    {
        serial->write((char*)buffer,sizeof(buffer));
    }
}

void MainWindow::conectar()
{
    serial->setPortName("COM2");
    serial->setBaudRate(9600);
    serial->setDataBits(static_cast<QSerialPort::DataBits>(8));
    serial->setParity(static_cast<QSerialPort::Parity>(0));
    serial->setStopBits(static_cast<QSerialPort::StopBits>(1));
    serial->setFlowControl(static_cast<QSerialPort::FlowControl>(0));
    if (serial->open(QIODevice::ReadWrite))
    {
        qDebug("Porta Aberta");
        ui->label->setText("Status da Porta : ABERTA");
        timer->start(1000);
        isConected = 1;
    }
    else
    {
        qDebug("Não Abriu");
        ui->label->setText("Status da Porta : FECHADA");
    }
}

void MainWindow::desconectar()
{
    if(serial->isOpen()) {
        serial->close();
    }
    isConected = 0;
}

//testando conectar2 para zegbee
/*void MainWindow::conectar2()
{
    serial1->setPortName("COM2");
    serial1->setBaudRate(9600);
    serial1->setDataBits(static_cast<QSerialPort::DataBits>(8));
    serial1->setParity(static_cast<QSerialPort::Parity>(0));
    serial1->setStopBits(static_cast<QSerialPort::StopBits>(1));
    serial1->setFlowControl(static_cast<QSerialPort::FlowControl>(0));
    if (serial1->open(QIODevice::ReadWrite))
    {
        qDebug("Porta Aberta");
        ui->label->setText("Status da Porta : ABERTA");
        timer->start(1000);
        isConected = 1;
    }
    else
    {
        qDebug("Não Abriu");
        ui->label->setText("Status da Porta : FECHADA");
    }
}


void MainWindow::desconectar2()
{
    if(serial1->isOpen()) {
        serial1->close();
    }
    isConected = 0;
}
*/

void MainWindow::atualizarTela()
{
    if(isConected) {
        ui->lineEdit->setEnabled(true);
        ui->pushButton->setText("Desconectar");
        ui->pushButton_2->setEnabled(true);
        ui->umi->setEnabled(true);
        ui->temp->setEnabled(true);
        ui->Float->setEnabled(true);
        ui->label_4->setEnabled(true);
        ui->label_5->setEnabled(true);
        ui->Button_led1->setEnabled(true);
        ui->Button_led2->setEnabled(true);
        ui->Button_led3->setEnabled(true);
        ui->Button_led4->setEnabled(true);
        ui->led1_ind->setEnabled(true);
        ui->led2_ind->setEnabled(true);
        ui->led3_ind->setEnabled(true);
        ui->led4_ind->setEnabled(true);
        ui->Chave1->setEnabled(true);
        ui->Chave2->setEnabled(true);
        ui->Chave3->setEnabled(true);
        ui->Chave4->setEnabled(true);
        //ui->label_2->setEnabled(true);
        //ui->label_3->setEnabled(true);
    }else{
        ui->lineEdit->setEnabled(false);
        ui->pushButton->setText("Conectar");
        ui->pushButton_2->setEnabled(false);
        ui->umi->setEnabled(false);
        ui->temp->setEnabled(false);
        ui->Float->setEnabled(false);
        ui->label_4->setEnabled(false);
        ui->label_5->setEnabled(false);
        ui->Button_led1->setEnabled(false);
        ui->Button_led2->setEnabled(false);
        ui->Button_led3->setEnabled(false);
        ui->Button_led4->setEnabled(false);
        ui->led1_ind->setEnabled(false);
        ui->led2_ind->setEnabled(false);
        ui->led3_ind->setEnabled(false);
        ui->led4_ind->setEnabled(false);
        ui->Chave1->setEnabled(false);
        ui->Chave2->setEnabled(false);
        ui->Chave3->setEnabled(false);
        ui->Chave4->setEnabled(false);
        //ui->label_2->setEnabled(false);
        //ui->label_3->setEnabled(false);
    }
}
void MainWindow::on_Float_valueChanged(double arg1)
{
    unsigned char float_com[13];
    unsigned short crc;

    if(serial->isOpen())// se porta aberta
    {
        float_com[0] = 0x01;
        float_com[1] = 0x10;
        float_com[2] = 0x00;
        float_com[3] = 0x06;
        float_com[4] = 0x00;
        float_com[5] = 0x02;
        float_com[6] = 0x04;

        dado16.total = arg1;

        float_com[7] = dado16.parcial.low;
        float_com[8] = dado16.parcial.high;
        float_com[9] = dado16.parcial.low1;
        float_com[10] = dado16.parcial.high1;

        crc = CRC16(float_com,11);

        float_com[11] = (crc >> 8) & 0xff;
        float_com[12] = crc & 0xff;

        serial->write((char*)float_com,13);
    }
}

void MainWindow::on_Button_led1_clicked()
{
    unsigned char Led1_state[13];
    unsigned short crc;
    QPixmap luz_acesa ("luz_acesa.png");
    QPixmap luz_apagada ("luz_apagada.png");

    if(serial->isOpen()){// se porta aberta

        Led1_state[0] = 0x01;
        Led1_state[1] = 0x0F;
        Led1_state[2] = 0x00;
        Led1_state[3] = 0x01;
        Led1_state[4] = 0x00;
        Led1_state[5] = 0x04;
        Led1_state[6] = 0x01;

        led_state ^= (1 << 0);

         if(led_state & (1 << 0)){
           ui->led1_ind->setPixmap(luz_acesa.scaled(71,61,Qt::KeepAspectRatio));
         }else{
           ui->led1_ind->setPixmap(luz_apagada.scaled(71,61,Qt::KeepAspectRatio));
         }

        Led1_state[7] = led_state;
        crc = CRC16(Led1_state,8);

        Led1_state[8] = (crc >> 8) & 0xff;
        Led1_state[9] = crc & 0xff;

        serial->write((char*)Led1_state,10);
    }
}

void MainWindow::on_Button_led2_clicked()
{
    unsigned char Led2_state[13];
    unsigned short crc;
    QPixmap luz_acesa ("C:/Users/Leonardo/Desktop/Embarcados II/T1_modbus/Serial/luz_acesa.png");
    QPixmap luz_apagada ("C:/Users/Leonardo/Desktop/Embarcados II/T1_modbus/Serial/luz_apagada.png");

    if(serial->isOpen()){// se porta aberta

        Led2_state[0] = 0x01;
        Led2_state[1] = 0x0F;
        Led2_state[2] = 0x00;
        Led2_state[3] = 0x01;
        Led2_state[4] = 0x00;
        Led2_state[5] = 0x04;
        Led2_state[6] = 0x01;

        led_state ^= (1 << 1);

         if(led_state & (1 << 1)){
           ui->led2_ind->setPixmap(luz_acesa.scaled(71,61,Qt::KeepAspectRatio));
         }else{
           ui->led2_ind->setPixmap(luz_apagada.scaled(71,61,Qt::KeepAspectRatio));
         }

        Led2_state[7] = led_state;
        crc = CRC16(Led2_state,8);

        Led2_state[8] = (crc >> 8) & 0xff;
        Led2_state[9] = crc & 0xff;

        serial->write((char*)Led2_state,10);
    }
}

void MainWindow::on_Button_led3_clicked()
{
    unsigned char Led3_state[13];
    unsigned short crc;
    QPixmap luz_acesa ("C:/Users/Leonardo/Desktop/Embarcados II/T1_modbus/Serial/luz_acesa.png");
    QPixmap luz_apagada ("C:/Users/Leonardo/Desktop/Embarcados II/T1_modbus/Serial/luz_apagada.png");

    if(serial->isOpen()){// se porta aberta

        Led3_state[0] = 0x01;
        Led3_state[1] = 0x0F;
        Led3_state[2] = 0x00;
        Led3_state[3] = 0x01;
        Led3_state[4] = 0x00;
        Led3_state[5] = 0x04;
        Led3_state[6] = 0x01;

        led_state ^= (1 << 2);

         if(led_state & (1 << 2)){
           ui->led3_ind->setPixmap(luz_acesa.scaled(71,61,Qt::KeepAspectRatio));
         }else{
           ui->led3_ind->setPixmap(luz_apagada.scaled(71,61,Qt::KeepAspectRatio));
         }

        Led3_state[7] = led_state;
        crc = CRC16(Led3_state,8);

        Led3_state[8] = (crc >> 8) & 0xff;
        Led3_state[9] = crc & 0xff;

        serial->write((char*)Led3_state,10);
    }
}

void MainWindow::on_Button_led4_clicked()
{
    unsigned char Led4_state[13];
    unsigned short crc;
    QPixmap luz_acesa ("C:/Users/Leonardo/Desktop/Embarcados II/T1_modbus/Serial/luz_acesa.png");
    QPixmap luz_apagada ("C:/Users/Leonardo/Desktop/Embarcados II/T1_modbus/Serial/luz_apagada.png");

    if(serial->isOpen()){// se porta aberta

        Led4_state[0] = 0x01;
        Led4_state[1] = 0x0F;
        Led4_state[2] = 0x00;
        Led4_state[3] = 0x01;
        Led4_state[4] = 0x00;
        Led4_state[5] = 0x04;
        Led4_state[6] = 0x01;

        led_state ^= (1 << 3);

         if(led_state & (1 << 3)){
           ui->led4_ind->setPixmap(luz_acesa.scaled(71,61,Qt::KeepAspectRatio));
         }else{
           ui->led4_ind->setPixmap(luz_apagada.scaled(71,61,Qt::KeepAspectRatio));
         }

        Led4_state[7] = led_state;
        crc = CRC16(Led4_state,8);

        Led4_state[8] = (crc >> 8) & 0xff;
        Led4_state[9] = crc & 0xff;

        serial->write((char*)Led4_state,10);
    }
}

void MainWindow::modbus(char dado){
    static int state = 0;
    uint8_t crc_High, crc_Low;
    unsigned char buffer[15];
    unsigned short crc;

    switch(state){
          case 0:
            if(dado == 0x01){ //endereço do kit
              buffer[0] = dado;
              state = 1;
            }else{
              state = 0;
            }break;

          case 1:
            if(dado == 0x02){ //função Switches
              buffer[1] = dado;
              state = 2;
            }else if(dado == 0x03){ //função Temperatura e Umidade
              buffer[1] = dado;
              state = 6;
            }else{
              state = 0;
            }break;

          case 2:
            if(dado == 0x01){ //número de bytes
              buffer[2] = dado;
              state = 3;
            }else{
              state = 0;
            }break;

          case 3:
            if(dado & (1 << 0)){
                ui->Chave1->setText("Chave 1 - On");
            }else{
                ui->Chave1->setText("Chave 1 - Off");
            }if(dado & (1 << 1)){
                ui->Chave2->setText("Chave 2 - On");
            }else{
                ui->Chave2->setText("Chave 2 - Off");
            }if(dado & (1 << 2)){
                ui->Chave3->setText("Chave 3 - On");
            }else{
                ui->Chave3->setText("Chave 3 - Off");
            }if(dado & (1 << 3)){
                ui->Chave4->setText("Chave 4 - On");
            }else{
                ui->Chave4->setText("Chave 4 - Off");
            }
            buffer[3] = dado;
            state = 4;
        break;

        case 4:
          crc_High = dado;
          state = 5;
        break;

        case 5:
          crc_Low = dado;
          state = 0;
        break;
        //Função Temperatura - SHT15
        case 6:
          if(dado == 0x06){ //número de bytes
            buffer[2] = dado;
            state = 7;
          }else{
            state = 0;
          }break;

        case 7:
          if(dado == 0x00){
            buffer[3] = dado;
            state = 8;
          }else{
            state = 0;
          }
        break;

        case 8:
          if(dado == 0x01){
            buffer[4] = dado;
            state = 9;
          }else if(dado == 0x02){
              buffer[4] = dado;
              state = 15;
          }else{
            state = 0;
          }
        break;

        case 9:
          temp_sht.parcial.low = dado;
          buffer[5] = dado;
          state = 10;
        break;

        case 10:
          temp_sht.parcial.high = dado;
          buffer[6] = dado;
          state = 11;
        break;

        case 11:
          temp_sht.parcial.low1 = dado;
          buffer[7] = dado;
          state = 12;
        break;

        case 12:
          temp_sht.parcial.high1 = dado;
          buffer[8] = dado;
          ui->temp->display(temp_sht.total);
          state = 13;
        break;

        case 13:
          crc_High = dado;
          state = 14;
        break;

        case 14:
          crc_Low = dado;
          state = 0;

          crc = CRC16(buffer,9);
          buffer[9] = ((crc >> 8) & 0xff);
          buffer[10] = ((crc) & 0xff);

         /*if((buffer[7] == crc_High) && (buffer[8] == crc_Low)){
            qDebug("CRC ok!");
         }else{
            qDebug("CRC error!");
         }*/
      break;

        case 15:
          umi_sht.parcial.low = dado;
          buffer[5] = dado;
          state = 16;
        break;

        case 16:
          umi_sht.parcial.high = dado;
          buffer[6] = dado;
          state = 17;
        break;

        case 17:
          umi_sht.parcial.low1 = dado;
          buffer[7] = dado;
          state = 18;
        break;

        case 18:
          umi_sht.parcial.high1 = dado;
          buffer[8] = dado;
          ui->umi->display(umi_sht.total);
          state = 19;
        break;

        case 19:
          crc_High = dado;
          state = 20;
        break;

        case 20:
          crc_Low = dado;
          state = 0;

          crc = CRC16(buffer,9);
          buffer[9] = ((crc >> 8) & 0xff);
          buffer[10] = ((crc) & 0xff);

      break;
    }
}
void MainWindow::Switches(){
    unsigned char switches_com[9];
    unsigned short crc;

    switches_com[0] = 0x01;
    switches_com[1] = 0x02;
    switches_com[2] = 0x00;
    switches_com[3] = 0x02;
    switches_com[4] = 0x00;
    switches_com[5] = 0x04;

    crc = CRC16(switches_com,6);

    switches_com[6] = (crc >> 8) & 0xff;
    switches_com[7] = crc & 0xff;

    serial->write((char*)switches_com,8);
}
void MainWindow::Temperatura(){
    unsigned char temp_com[9];
    unsigned short crc;

    temp_com[0] = 0x01;
    temp_com[1] = 0x03;
    temp_com[2] = 0x00;
    temp_com[3] = 0x04;
    temp_com[4] = 0x00;
    temp_com[5] = 0x03;

    crc = CRC16(temp_com,6);

    temp_com[6] = (crc >> 8) & 0xff;
    temp_com[7] = crc & 0xff;

    serial->write((char*)temp_com,8);
}
void MainWindow::Umidade(){
    unsigned char umi_com[9];
    unsigned short crc;

    umi_com[0] = 0x01;
    umi_com[1] = 0x03;
    umi_com[2] = 0x00;
    umi_com[3] = 0x05;
    umi_com[4] = 0x00;
    umi_com[5] = 0x03;

    crc = CRC16(umi_com,6);

    umi_com[6] = (crc >> 8) & 0xff;
    umi_com[7] = crc & 0xff;

    serial->write((char*)umi_com,8);
}
