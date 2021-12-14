#ifndef HELL_WINDOW_H
#define HELL_WINDOW_H

#include <QMainWindow>
#include <QTimer>
#include "user_instructions.h"
#include "network_detect.h"


namespace Ui {
class Hell_window;
}

class Hell_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Hell_window(QWidget *parent = nullptr);
    ~Hell_window();

    //变量
     QTimer *timer1;         //定时器1

     int time1 = 3;

private slots:

     void TimeOut1();   //定时1回调函数

  //    void update_network_state(int state); //检测是否联网

private:
    Ui::Hell_window *ui;
    network_detect *detect_thread;
};

#endif // HELL_WINDOW_H
