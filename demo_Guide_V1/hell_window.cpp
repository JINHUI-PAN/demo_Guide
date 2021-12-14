#include "hell_window.h"
#include "ui_hell_window.h"

Hell_window::Hell_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Hell_window)
{
    ui->setupUi(this);

      this->setStyleSheet("#Hell_window{background-image: url(:/back/Image/button/hello.png);}");      //设置底图
    //定时器
    timer1 = new QTimer(this);   //定时更新
    connect(timer1,SIGNAL(timeout()),this,SLOT(TimeOut1()));
    timer1->start(1000); //一秒中断一次

    detect_thread = new network_detect();

 //   connect(detect_thread,&QThread::finished,detect_thread,&QObject::deleteLater);   //线程结束调用detect来销毁分配的内存
    if(!this->detect_thread->isRunning())
    {
         this->detect_thread->start();     //开启网络检测线程
    }
}

Hell_window::~Hell_window()
{
    delete timer1;
    delete ui;
}

void Hell_window::TimeOut1()
{
    //定时器回调
    time1--;
    QString CMD = QString("欢迎使用壁虎爬架 (%1)").arg(time1);
    ui->label_hellodata->setText(CMD);  //设置欢迎倒计时
    if(time1 == 0)      //  倒计时结束
    {
       this->close();
       timer1->stop();
       user_instructions *user = new user_instructions;
       user->show();

    }

}

//void Hell_window::update_network_state(int state)
//{
//    //检测是否有网
//    if(state)
//    {
//        qDebug() << "在线";
//        detect_thread->stop();
//    }else
//    {
//        qDebug() << "离线";
//        detect_thread->stop();
//    }

//}
