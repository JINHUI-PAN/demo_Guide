#include "network_detect.h"

network_detect::network_detect()
{
     flagRunning = true;
}


//多线程函数
void network_detect::run()
{
    //连接

//   mqtt = new QMqttClient();
//   mqtt->setHostname("emqx.qinyun575.cn");  //   broker.hivemq.com //官方测试地址
//   mqtt->setPort(1883);
//   mqtt->setClientId("1234");
//   mqtt->setUsername("admin");
//   mqtt->setPassword("1234");
//   mqtt->setKeepAlive(60); // 心跳
//   connect(mqtt,  &QMqttClient::stateChanged, this,&network_detect::mqtt_connect_success);
//   connect(mqtt,  &QMqttClient::disconnected, this,&network_detect::mqtt_disconnect);
//   connect(mqtt,&QMqttClient::messageReceived, this,&network_detect::mqtt_recv_msg);
//   mqtt->connectToHost();  // 连接 EMQ 代理服务器

//   if(mqtt->state() == QMqttClient::Connected)
//   {
//        mqtt->subscribe(QString("/demo"),1);

//   }



    while(flagRunning)
    {
        //订阅主题

        sleep(100);       //休眠一秒

    }
}

void network_detect::stop()
{
    flagRunning = false;
}

void network_detect::mqtt_recv_msg(QByteArray msg, QMqttTopicName topic)
{
    //mqtt接收到数据
    //接收数据
    //解析json数据
    QJsonParseError parseJsonErr;
    QJsonDocument document = QJsonDocument::fromJson(msg,&parseJsonErr);
    if(!(parseJsonErr.error == QJsonParseError::NoError))
       {
            qDebug()<<tr("解析json文件错误！");

        }
    QJsonObject jsonObject = document.object();
    if(jsonObject.contains("msg"))  //键值对
    {
        QJsonValue  value = jsonObject.value("msg");
        QString aa = value.toString();
       qDebug()<<"value :"<< aa;    //jsonObject["msg"].toString()
    }


    QString recv_msg = "Topic:";
    recv_msg += topic.name();
    recv_msg += "\n";
    recv_msg  += msg;
    qDebug() << recv_msg;

}

void network_detect::mqtt_connect_success()
{
    //mqtt连接成功
    qDebug() << "连接成功";

}

void network_detect::mqtt_disconnect()
{
    qDebug() << "断开连接";


}
