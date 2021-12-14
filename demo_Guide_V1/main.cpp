#include "user_instructions.h"
#include "hell_window.h"
#include "serialport.h"
#include "mqtt_window.h"
#include <QApplication>

int main(int argc, char *argv[])
{
   // qputenv("QT_IM_MODULE",QByteArray("qtvirtualkeyboard"));
    QApplication a(argc, argv);
   // QApplication::setOverrideCursor(Qt::BlankCursor);     //隐藏全局鼠标    //单页面QWidget::setCursor(QCursor(Qt::BlankCursor)) this->setCursor(Qt::BlankCursor);   //隐藏鼠标  this->setCursor(Qt::ArrowCursor);  //显示正常鼠标

    Serialport j;       //初始化串口

    Mqtt_window M;      //初始化后台数据
    Hell_window H;  //欢迎页面




//    user_instructions w;

//    w.showFullScreen();     //全屏


    H.show();
    return a.exec();
}
