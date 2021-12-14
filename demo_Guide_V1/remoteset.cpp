#include "remoteset.h"
#include "ui_remoteset.h"

remoteSet::remoteSet(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::remoteSet)
{
    ui->setupUi(this);
    ui->widget_main->hide();
     this->setStyleSheet("#remoteSet{background-image: url(:/back/Image/button/config.png);}");

    timer3 = new QTimer(this);   //定时更新
    connect(timer3,SIGNAL(timeout()),this,SLOT(TimeOut3()));
    timer3->start(1000);      //一秒中断一次

}

remoteSet::~remoteSet()
{
    delete timer3;
    delete ui;
}

void remoteSet::TimeOut3()
{
    //定时器回调
     ui->label_time->setText(now_data);  //更新时间
}

void remoteSet::on_bindmainButton_clicked()
{

    //绑定主遥控   发送绑定遥控指令
    remotesetfale = 1;  //主遥控
    ui->widget_main->show();
    ui->label_remotelable->setText("遥控搜索中");
    ui->label_mainORvice->setText("主遥控");
    ui->label_bindID->clear();
    ui->label_mainRemind->show();


}

void remoteSet::on_bindviceButton_clicked()
{
    //绑定副遥控
     remotesetfale = 1;  //副遥控
    ui->widget_main->show();
    ui->label_remotelable->setText("遥控搜索中");
    ui->label_mainORvice->setText("副遥控");
    ui->label_bindID->clear();
    ui->label_mainRemind->hide();
}


void remoteSet::on_pushButton_mainbindCencel_clicked()
{
    //搜索绑定取消按钮
    ui->widget_main->hide();
     remotesetfale = 0;  //遥控标志位

}


void remoteSet::on_pushButton_mainbindOK_clicked()
{
    if( remotesetfale == 1)
    {
        //主控绑定确定按钮

        ui->widget_main->hide();


    }else if (remotesetfale == 2)
    {
        //副控绑定确定按钮
        ui->widget_main->hide();

    }



}


void remoteSet::on_pushButton_clearmain_clicked()
{
    //取消主遥控绑定
    QMessageBox::StandardButton result = QMessageBox::warning(NULL,"取消","你确定取消主遥控绑定吗",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
    switch (result)
    {
        case QMessageBox::Yes: qDebug() << "YES" ;break;
        case QMessageBox::No: qDebug()<< "NO"; break;
        default:break;

    }


}

void remoteSet::on_pushButton_clear1_clicked()
{
     //副遥控1
    QMessageBox::StandardButton result = QMessageBox::warning(NULL,"取消","你确定取消副遥控绑定吗",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
    switch (result)
    {
        case QMessageBox::Yes: qDebug() << "YES" ;break;
        case QMessageBox::No: qDebug()<< "NO"; break;
        default:break;

    }

}


void remoteSet::on_pushButton_clear2_clicked()
{
    //副遥控2
    QMessageBox::StandardButton result = QMessageBox::warning(NULL,"取消","你确定取消副遥控绑定吗",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
    switch (result)
    {
        case QMessageBox::Yes: qDebug() << "YES" ;break;
        case QMessageBox::No: qDebug()<< "NO"; break;
        default:break;

    }

}


void remoteSet::on_pushButton_clear3_clicked()
{
    //副遥控3
    QMessageBox::StandardButton result = QMessageBox::warning(NULL,"取消","你确定取消副遥控绑定吗",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
    switch (result)
    {
        case QMessageBox::Yes: qDebug() << "YES" ;break;
        case QMessageBox::No: qDebug()<< "NO"; break;
        default:break;

    }

}


void remoteSet::on_pushButton_clear4_clicked()
{
    //副遥控4
    QMessageBox::StandardButton result = QMessageBox::warning(NULL,"取消","你确定取消副遥控绑定吗",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
    switch (result)
    {
        case QMessageBox::Yes: qDebug() << "YES" ;break;
        case QMessageBox::No: qDebug()<< "NO"; break;
        default:break;

    }
}


void remoteSet::on_pushButton_clear5_clicked()
{
     //副遥控5
    QMessageBox::StandardButton result = QMessageBox::warning(NULL,"取消","你确定取消副遥控绑定吗",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
    switch (result)
    {
        case QMessageBox::Yes: qDebug() << "YES" ;break;
        case QMessageBox::No: qDebug()<< "NO"; break;
        default:break;

    }

}


//页面切换
void remoteSet::on_pushButton_control_clicked()
{
    //分控操作页面

         this->close();
        oneWindow *one = new oneWindow;
        one->show();

}


void remoteSet::on_pushButton_oneConfig_clicked()
{
    //分控设置页面

         this->close();
        configWindow *config = new configWindow;
        config->show();


}

//void remoteSet::on_pushButton_remote_clicked()
//{
//    //遥控设置
//    this->close();
//    remoteSet *remote = new remoteSet;
//    remote->show();

//}

void remoteSet::on_pushButton_dairy_clicked()
{
    //操作日志

         this->close();
        DiaryWindow  *diary = new DiaryWindow;
        diary->show();


}


void remoteSet::on_pushButton_project_clicked()
{
    //项目信息

        this->close();
        Project_information  *project = new Project_information;
        project->show();


}


void remoteSet::on_pushButton_system_clicked()
{
    //系统信息

         this->close();
        Syetem_information *system = new Syetem_information;
        system->show();


}





