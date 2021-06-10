#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSerialPort>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void readData();
    void timer_teste();
    void on_pushButton_2_clicked();
    void atualizarTela();
    void desconectar();
    void conectar();
    //void conectar2();
    //void desconectar2();

    void on_Float_valueChanged(double arg1);

    void on_Button_led1_clicked();

    void on_Button_led2_clicked();

    void on_Button_led3_clicked();

    void on_Button_led4_clicked();

    void modbus(char dado);

    void Switches();

    void Temperatura();

    void Umidade();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
    //QSerialPort *serial1;
};

#endif // MAINWINDOW_H
