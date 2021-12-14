#ifndef PARAMETER_H
#define PARAMETER_H

#include <QMainWindow>
#include <QTimer>
#include <QSettings>
#include <QFileInfo>
//#include "user_instructions.h"
//#include "serialport.h"

namespace Ui {
class Parameter;
}

class Parameter : public QMainWindow
{
    Q_OBJECT

public:
    explicit Parameter(QWidget *parent = nullptr);
    ~Parameter();


    //读取参数配置
    void Read_config();

    //变量
     QTimer *timer1;         //定时器1



private slots:
      void TimeOut1();   //定时1回调函数
    void on_pushButton_Modify_clicked();

    void on_pushButton_exit_clicked();

private:
    Ui::Parameter *ui;
};

#endif // PARAMETER_H
