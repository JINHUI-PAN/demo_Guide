#ifndef PARAMETER_WINDOW_H
#define PARAMETER_WINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QSettings>
#include <QFileInfo>
#include <QDebug>
#include "user_instructions.h"
#include "serialport.h"

namespace Ui {
class Parameter_window;
}

class Parameter_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Parameter_window(QWidget *parent = nullptr);
    ~Parameter_window();

    //读取参数配置
    void Read_config();

    //变量
     QTimer *timer1;         //定时器1

private slots:
   void TimeOut1();   //定时1回调函数
    void on_pushButton_Modify_clicked();

    void on_pushButton_exit_clicked();

private:
    Ui::Parameter_window *ui;
};

#endif // PARAMETER_WINDOW_H
