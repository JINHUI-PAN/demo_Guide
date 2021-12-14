#ifndef NETWORK_DETECT_H
#define NETWORK_DETECT_H

#include <QThread>          //线程头文件
#include <QDebug>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QtNetwork>
#include <QHostAddress>
#include <QtMqtt/qmqttclient.h>
#include <QtMqtt/qmqttglobal.h>


class network_detect : public QThread
{
    Q_OBJECT
public:
    network_detect();
    //获取网络状态

    virtual void run();
    void stop();

    bool flagRunning;       //线程运行标志

signals:
//    void send_network_connect_state(int state);

private slots:
      void mqtt_recv_msg(QByteArray msg,QMqttTopicName topic);

      void mqtt_connect_success();

      void mqtt_disconnect();

private:

     QJsonObject object;            //定义Json对象
     QMqttClient *mqtt;            //定义mqtt结构

};

#endif // NETWORK_DETECT_H
