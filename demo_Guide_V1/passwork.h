#ifndef PASSWORK_H
#define PASSWORK_H

#include <QMainWindow>
#include <QSettings>
#include <QFileInfo>
#include <QDebug>
#include <QTime>
#include <QMessageBox>
#include "user_instructions.h"


namespace Ui {
class passwork;
}

class passwork : public QMainWindow
{
    Q_OBJECT

public:
    explicit passwork(QWidget *parent = nullptr);
    ~passwork();

    void setPasswork(const QString &newPasswork);

private slots:
    void on_okButton_clicked();

    void on_pushButton_2_clicked();

private:
    Ui::passwork *ui;
    //变量
    QString  PassWork0 = "";
    QString  PassWork1 = "";
    QString  PassWork2 = "";
    QString  CONFIG_NAME = "CONGIF.ini";
    QDate DAY;
    QMessageBox BOX;




    int Year  ,Month,Day ;
    int a[3];


    //函数
   void Config_init(void);
   void CheckProjectdate(void);

};

#endif // PASSWORK_H
