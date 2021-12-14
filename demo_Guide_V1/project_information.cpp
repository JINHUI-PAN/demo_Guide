#include "project_information.h"
#include "ui_project_information.h"

Project_information::Project_information(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Project_information)
{
    ui->setupUi(this);
    this->setStyleSheet("#Project_information{background-image: url(:/back/Image/button/config.png);}");   //设置底图

    timer2 = new QTimer(this);   //定时更新
    connect(timer2,SIGNAL(timeout()),this,SLOT(TimeOut2()));
    timer2->start(1000);      //一秒中断一次
}

Project_information::~Project_information()
{
    delete timer2;
    delete ui;
}

void Project_information::TimeOut2()
{
    //定时器回调
     ui->label_time->setText(now_data);  //更新时间
}




//页面切换
void Project_information::on_pushButton_control_clicked()
{
    //分控操作页面

        this->close();
        oneWindow *one = new oneWindow;
        one->show();
}


void Project_information::on_pushButton_oneConfig_clicked()
{
    //分控设置页面

        this->close();
        configWindow *config = new configWindow;
        config->show();


}

void Project_information::on_pushButton_remote_clicked()
{
    //遥控设置
    this->close();
    remoteSet *remote = new remoteSet;
    remote->show();


}

void Project_information::on_pushButton_dairy_clicked()
{
    //操作日志

         this->close();
        DiaryWindow  *diary = new DiaryWindow;
        diary->show();


}


//void Project_information::on_pushButton_project_clicked()
//{
//    //项目信息

//        this->close();
//        Project_information  *project = new Project_information;
//        project->show();

//}


void Project_information::on_pushButton_system_clicked()
{
    //系统信息

        this->close();
        Syetem_information *system = new Syetem_information;
        system->show();


}
