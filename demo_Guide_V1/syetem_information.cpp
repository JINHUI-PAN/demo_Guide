#include "syetem_information.h"
#include "ui_syetem_information.h"

Syetem_information::Syetem_information(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Syetem_information)
{
    ui->setupUi(this);
     this->setStyleSheet("#Syetem_information{background-image: url(:/back/Image/button/config.png);}");   //设置底图

    timer2 = new QTimer(this);   //定时更新
    connect(timer2,SIGNAL(timeout()),this,SLOT(TimeOut2()));
    timer2->start(1000);      //一秒中断一次
}

Syetem_information::~Syetem_information()
{
    delete timer2;
    delete ui;
}

void Syetem_information::TimeOut2()
{
    //定时器回调
     ui->label_time->setText(now_data);  //更新时间
}

void Syetem_information::on_pushButton_restore_clicked()
{
    //恢复出厂设置

    QMessageBox::StandardButton result = QMessageBox::warning(NULL,"恢复出厂","确定恢复出厂设置",QMessageBox::Yes | QMessageBox::No );

    switch (result)
    {

        case QMessageBox::Yes: qDebug() << "YES";break;

        case QMessageBox::No: qDebug() << "NO";break;
        default :break;


    }

}


void Syetem_information::on_pushButton_restart_clicked()
{
    //重启设备

    QMessageBox::StandardButton result = QMessageBox::warning(NULL,"重启","确定重启设备吗",QMessageBox::Yes | QMessageBox::No );

    switch (result)
    {

        case QMessageBox::Yes: qDebug() << "YES";break;
        case QMessageBox::No: qDebug() << "NO";break;
        default :break;


    }
}


void Syetem_information::on_pushButton_exit_clicked()
{
    //退出登录
    QMessageBox::StandardButton result = QMessageBox::warning(NULL,"退出","确定退出登录吗",QMessageBox::Yes | QMessageBox::No );

    switch (result)
    {

        case QMessageBox::Yes: { this->close();
                                 user_instructions *usr = new user_instructions;
                                 usr->show();
                                 Motor1.login = MASTER_SIGN_OFF;
             } qDebug() << "YES";break;
        case QMessageBox::No: qDebug() << "NO";break;
        default :break;


    }
}



//页面切换
void Syetem_information::on_pushButton_control_clicked()
{
    //分控操作页面

         this->close();
        oneWindow *one = new oneWindow;
        one->show();

}


void Syetem_information::on_pushButton_oneConfig_clicked()
{
    //分控设置页面

         this->close();
        configWindow *config = new configWindow;
        config->show();


}

void Syetem_information::on_pushButton_remote_clicked()
{
    //遥控设置
     this->close();
    remoteSet *remote = new remoteSet;
    remote->show();


}

void Syetem_information::on_pushButton_dairy_clicked()
{
    //操作日志

        this->close();
        DiaryWindow  *diary = new DiaryWindow;
        diary->show();


}


void Syetem_information::on_pushButton_project_clicked()
{
    //项目信息

        this->close();
        Project_information  *project = new Project_information;
        project->show();


}


//void Syetem_information::on_pushButton_system_clicked()
//{
//    //系统信息

//        this->close();
//        Syetem_information *system = new Syetem_information;
//        system->show();

//}


