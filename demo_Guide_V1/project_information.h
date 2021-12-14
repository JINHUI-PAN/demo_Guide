#ifndef PROJECT_INFORMATION_H
#define PROJECT_INFORMATION_H

#include <QMainWindow>
#include <QTimer>
#include "configwindow.h"
#include "diarywindow.h"
#include "remoteset.h"
//#include "project_information.h"
#include "syetem_information.h"
#include "onewindow.h"


namespace Ui {
class Project_information;
}

class Project_information : public QMainWindow
{
    Q_OBJECT

public:
    explicit Project_information(QWidget *parent = nullptr);
    ~Project_information();

    QTimer *timer2;         //定时器1

private slots:

     void TimeOut2();   //定时1回调函数


    void on_pushButton_control_clicked();

    void on_pushButton_oneConfig_clicked();

    void on_pushButton_remote_clicked();

    void on_pushButton_dairy_clicked();

 //   void on_pushButton_project_clicked();

    void on_pushButton_system_clicked();

private:
    Ui::Project_information *ui;
};

#endif // PROJECT_INFORMATION_H
