#ifndef REMOTESET_H
#define REMOTESET_H

#include <QMainWindow>

#include <QMessageBox>
#include <QDebug>
#include "configwindow.h"
#include "diarywindow.h"
//#include "remoteset.h"
#include "project_information.h"
#include "syetem_information.h"
#include "onewindow.h"
#include <QTimer>

namespace Ui {
class remoteSet;
}

class remoteSet : public QMainWindow
{
    Q_OBJECT

public:
    explicit remoteSet(QWidget *parent = nullptr);
    ~remoteSet();

    QTimer *timer3;         //定时器1

    int remotesetfale = 0;      //主副遥控修改标志

private slots:
     void TimeOut3();   //定时1回调函数

    void on_bindmainButton_clicked();

    void on_pushButton_mainbindCencel_clicked();

    void on_pushButton_mainbindOK_clicked();

    void on_bindviceButton_clicked();

    void on_pushButton_clearmain_clicked();

    void on_pushButton_clear2_clicked();

    void on_pushButton_clear3_clicked();

    void on_pushButton_clear4_clicked();

    void on_pushButton_clear1_clicked();

    void on_pushButton_clear5_clicked();


    void on_pushButton_control_clicked();

    void on_pushButton_oneConfig_clicked();

 //   void on_pushButton_remote_clicked();

    void on_pushButton_dairy_clicked();

    void on_pushButton_project_clicked();

    void on_pushButton_system_clicked();






private:
    Ui::remoteSet *ui;
};

#endif // REMOTESET_H
