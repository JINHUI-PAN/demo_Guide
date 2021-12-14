#include "mqtt_window.h"
#include "ui_mqtt_window.h"

Mqtt_window::Mqtt_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Mqtt_window)
{
    ui->setupUi(this);

    timer2 = new QTimer(this);   //定时更新
    connect(timer2,SIGNAL(timeout()),this,SLOT(TimeOut2()));

   // 连接mqtt
   mqtt = new QMqttClient();
   mqtt->setHostname("emqx.qinyun575.cn");  //   broker.hivemq.com //官方测试地址
   mqtt->setPort(1883);
   mqtt->setClientId("1234");
   mqtt->setUsername("admin");
   mqtt->setPassword("1234");
   mqtt->setKeepAlive(60); // 心跳
   connect(mqtt,  &QMqttClient::stateChanged, this,&Mqtt_window::mqtt_connect_success);
   connect(mqtt,  &QMqttClient::disconnected, this,&Mqtt_window::mqtt_disconnect);
   connect(mqtt,&QMqttClient::messageReceived, this,&Mqtt_window::mqtt_recv_msg);
   mqtt->connectToHost();  // 连接 EMQ 代理服务器

   timer1 = new QTimer(this);   //定时更新
   connect(timer1,SIGNAL(timeout()),this,SLOT(TimeOut1()));
   timer1->start(500);      //一秒中断一次


   QString key="bC1&dG7#bA2*eA6%";
   QString md5;
   QByteArray ba,bb;
   QCryptographicHash md(QCryptographicHash::Md5);
   ba.append(key);
   //随机数
    qsrand(QTime(0,0,0).secsTo(QTime::currentTime()));
    int test =qrand()%10;
    QString test1 = QString(test);
    ba.append(test1);
    //秒级时间戳（十位）
    QString timestamp = QString::number(QDateTime::currentMSecsSinceEpoch() / 1000);
    ba.append(timestamp);
    qDebug() << ba;
   md.addData(ba);          //动态接口加密 hash静态加密
   bb = md.result();
   qDebug() << bb.toHex();






}

Mqtt_window::~Mqtt_window()
{
    delete timer1;
    delete timer2;
    delete ui;
}

void Mqtt_window::TimeOut1()
{
    //定时器回调
    if(mqtt->state() == QMqttClient::Connected)
    {

      mqtt->subscribe(QString("demo"),1) ;


    }

    //发布消息
    QString topic;
    QByteArray messagePub;
    if(mqtt->state() == QMqttClient::Connected)
    {
      topic = "demo";
     //  messagePub = ui->lineEdit_pubContent->text().toUtf8();
      object.insert("hello","server");      //  添加json数据
     QJsonDocument document(object);        //转换成QJsonDocument对象
     messagePub = document.toJson();        //转换成QByteArray对象
       if(mqtt->publish(topic,messagePub) == -1)
       {
           qDebug() << "发布话题失败";

       }
    }

     timer1->stop();

}

void Mqtt_window::TimeOut2()
{
    //定时器2回调 mqtt重连
    mqtt->setHostname("emqx.qinyun575.cn");  //   broker.hivemq.com //官方测试地址
    mqtt->setPort(1883);
    mqtt->setClientId("1234");
    mqtt->setUsername("admin");
    mqtt->setPassword("1234");
    mqtt->setKeepAlive(60); // 心跳
//    connect(mqtt,  &QMqttClient::stateChanged, this,&Mqtt_window::mqtt_connect_success);
//    connect(mqtt,  &QMqttClient::disconnected, this,&Mqtt_window::mqtt_disconnect);
//    connect(mqtt,&QMqttClient::messageReceived, this,&Mqtt_window::mqtt_recv_msg);
    mqtt->connectToHost();  // 连接 EMQ 代理服务器
    timer1->start(1000);
}

void Mqtt_window::mqtt_recv_msg(QByteArray msg, QMqttTopicName topic)
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

void Mqtt_window::mqtt_connect_success()
{
    //mqtt连接成功
    if(mqtt->state() == QMqttClient::Connected)
    {
         QString CMD = QString("成功连接 :%1").arg(mqtt->state());
         qDebug() <<  CMD;
         timer2->stop();
    }


}

void Mqtt_window::mqtt_disconnect()
{
    //mqtt断开连接
    timer2->start(1000);
     qDebug() << "mqtt断开连接";
}
