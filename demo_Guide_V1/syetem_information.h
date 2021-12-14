#ifndef SYETEM_INFORMATION_H
#define SYETEM_INFORMATION_H

#include <QMainWindow>

#include <QMessageBox>
#include <QDebug>
#include "configwindow.h"
#include "diarywindow.h"
#include "remoteset.h"
#include "project_information.h"
//#include "syetem_information.h"
#include "onewindow.h"
#include "user_instructions.h"
#include <QTimer>

namespace Ui {
class Syetem_information;
}

class Syetem_information : public QMainWindow
{
    Q_OBJECT

public:
    explicit Syetem_information(QWidget *parent = nullptr);
    ~Syetem_information();

    QTimer *timer2;         //定时器1

private slots:

     void TimeOut2();   //定时1回调函数


    void on_pushButton_restore_clicked();

    void on_pushButton_restart_clicked();

    void on_pushButton_exit_clicked();


    void on_pushButton_control_clicked();

    void on_pushButton_oneConfig_clicked();

    void on_pushButton_remote_clicked();

    void on_pushButton_dairy_clicked();

    void on_pushButton_project_clicked();

 //   void on_pushButton_system_clicked();



private:
    Ui::Syetem_information *ui;
};

#endif // SYETEM_INFORMATION_H
