#include "remotecontrol.h"
#include "ui_remotecontrol.h"

RemoteControl::RemoteControl(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::RemoteControl)
{
    ui->setupUi(this);

}

RemoteControl::~RemoteControl()
{
    delete ui;
}

void RemoteControl::resizeEvent(QResizeEvent *event)
{
    //布局
//    int x = this->frameGeometry().width();      //  获取ui窗口宽度包含边框和标题栏
//    int y = this->frameGeometry().height();     //获取窗口高度含边框和标题栏
    int x = this->geometry().width();      //  获取ui窗口宽度不包含边框和标题栏
    int y = this->geometry().height();     //获取窗口高度不含边框和标题栏

    qDebug() << "x:" <<x << "y:"<<y;

}

void RemoteControl::mousePressEvent(QMouseEvent *event)
{
    //获取屏幕点击
    QPoint point;
    point = event->pos();
    if(event->button() == Qt::LeftButton)
    {
        //左键
       if(point.rx()<790 && point.rx() >670 && point.ry()<200)
       {
            pointTime++;

       }
        qDebug() << "左键 " << point.rx()<<point.ry() << QString::number(pointTime) ;
        if(pointTime >= 8)
        {
            pointTime =0;
            qDebug() << "左键点击8次 ";
            this->hide();
            remoteSet *remote = new remoteSet;
            remote->show();
        }

    }
    if(event->button() == Qt::RightButton)
    {

        qDebug() << "右键"<< point.x() << point.y();
    }

}




void RemoteControl::on_pushButton_comeMain_clicked()
{
    //返回主页

}

