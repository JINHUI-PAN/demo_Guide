#ifndef REMOTECONTROL_H
#define REMOTECONTROL_H

#include <QMainWindow>
#include <QMouseEvent>      //获取屏幕点击头文件
#include <QPoint>
#include <QDebug>
#include "remoteset.h"
#include "mainwindow.h"

namespace Ui {
class RemoteControl;
}

class RemoteControl : public QMainWindow
{
    Q_OBJECT

public:
    explicit RemoteControl(QWidget *parent = nullptr);
    ~RemoteControl();
     int pointTime =0;

private:
    Ui::RemoteControl *ui;


protected:
    virtual void resizeEvent(QResizeEvent *event)   override;
    void mousePressEvent(QMouseEvent *event);

private slots:

    void on_pushButton_comeMain_clicked();
};

#endif // REMOTECONTROL_H
