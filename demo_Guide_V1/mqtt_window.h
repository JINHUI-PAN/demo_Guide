#ifndef MQTT_WINDOW_H
#define MQTT_WINDOW_H

#include <QMainWindow>
#include <QDebug>
#include <QTimer>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QtNetwork>
#include <QHostAddress>
#include <QtMqtt/qmqttclient.h>
#include <QtMqtt/qmqttglobal.h>
#include <QCryptographicHash>       //MD5
#include <QtGlobal>                 //随机数生成头文件

namespace Ui {
class Mqtt_window;
}

class Mqtt_window : public QMainWindow
{
    Q_OBJECT

public:
    explicit Mqtt_window(QWidget *parent = nullptr);
    ~Mqtt_window();
      QTimer *timer1;         //定时器1
      QTimer *timer2;         //定时器2



private slots:
       void TimeOut1();   //定时2回调函数
       void TimeOut2();   //定时2回调函数
      void mqtt_recv_msg(QByteArray msg,QMqttTopicName topic);

      void mqtt_connect_success();

      void mqtt_disconnect();

private:
    Ui::Mqtt_window *ui;

    QJsonObject object;            //定义Json对象
    QMqttClient *mqtt;            //定义mqtt结构
};

#endif // MQTT_WINDOW_H
