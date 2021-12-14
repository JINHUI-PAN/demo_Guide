#include "parameter.h"
#include "ui_parameter.h"


Parameter::Parameter(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Parameter)
{
    ui->setupUi(this);
   this->setStyleSheet("#Parameter{background-image: url(:/back/Image/button/config.png);}");      //底图

    //定时器
    timer1 = new QTimer(this);   //定时更新
    connect(timer1,SIGNAL(timeout()),this,SLOT(TimeOut1()));

}

Parameter::~Parameter()
{
    delete ui;
}

void Parameter::Read_config()
{
  //参数设置


}

void Parameter::TimeOut1()
{
    //定时器回调


}

void Parameter::on_pushButton_Modify_clicked()
{
    //确认修改参数



}


void Parameter::on_pushButton_exit_clicked()
{

    //退出设置界面
//    this->close();
//    user_instructions *user = new user_instructions;
//    user->show();

}

