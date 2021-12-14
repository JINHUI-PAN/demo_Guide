#include "parameter_window.h"
#include "ui_parameter_window.h"

Parameter_window::Parameter_window(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Parameter_window)
{
    ui->setupUi(this);
    this->setStyleSheet("#Parameter_window{background-image: url(:/back/Image/button/config.png);}");      //底图

    //初始化
    Read_config();
    Motor1.Motor_state = Wait;
    ui->label_information->hide();

    //定时器
    timer1 = new QTimer(this);   //定时更新
    connect(timer1,SIGNAL(timeout()),this,SLOT(TimeOut1()));
}

Parameter_window::~Parameter_window()
{
    delete timer1;
    delete ui;
}

void Parameter_window::Read_config()
{

    //初始化参数配置显示
    ui->comboBox_led->setCurrentIndex(paramemter_config.led);
    ui->comboBox_loose->setCurrentIndex(paramemter_config.loose);
    ui->comboBox_checkcurrent->setCurrentIndex(paramemter_config.checkcurrent);
    ui->comboBox_bolt->setCurrentIndex(paramemter_config.bolt);
    ui->comboBox_Startcurrent->setCurrentIndex(paramemter_config.startcurrent);
    ui->comboBox_currentMAX->setCurrentIndex(paramemter_config.curentMAX);
    ui->comboBox_runCurrent->setCurrentIndex(paramemter_config.runCurrent);
    ui->comboBox_baud->setCurrentIndex(paramemter_config.baud);

}

void Parameter_window::TimeOut1()
{
    //定时器回调
    if(Motor1.Motor_state == Stop)      //  设置完成
    {
          ui->label_information->hide();
          timer1->stop();
          Motor1.Motor_state = Wait;
    }


}

void Parameter_window::on_pushButton_Modify_clicked()
{
    //确定修改按钮
    paramemter_config.led = ui->comboBox_led->currentIndex();
    paramemter_config.loose = ui->comboBox_loose->currentIndex();
    paramemter_config.checkcurrent = ui->comboBox_checkcurrent->currentIndex();
    paramemter_config.bolt = ui->comboBox_bolt->currentIndex();
    paramemter_config.startcurrent = ui->comboBox_Startcurrent->currentIndex();
    paramemter_config.runCurrent = ui->comboBox_runCurrent->currentIndex();
    paramemter_config.curentMAX = ui->comboBox_currentMAX->currentIndex();
    paramemter_config.baud = ui->comboBox_baud->currentIndex();

    //保存
     QSettings settings(Config_parameter,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
     settings.beginGroup("Setting");
     settings.setValue("led",paramemter_config.led);              //0是报错led闪烁，1是常亮
     settings.setValue("loose",paramemter_config.loose);           //失载计算时间单位S，
     settings.setValue("checkCurrent",paramemter_config.checkcurrent);   //是否启用电流检测，0不启用，1启用
     settings.setValue("bolt",paramemter_config.bolt);           //多久时间不通讯，判定为断线单位S
     settings.setValue("startcurrent",paramemter_config.startcurrent);   //电机状态切换电流延迟多少毫秒检测，滤波，0表示200ms ~ 3表示500ms
     settings.setValue("curentMAX",paramemter_config.curentMAX);     //电流检测峰值，0表示30 ~ 4表示50
     settings.setValue("runCurrent",paramemter_config.runCurrent);    //电机状态切换电流延迟多少毫秒检测，滤波，0表示200ms ~ 3表示500ms
     settings.setValue("baud",paramemter_config.baud);          //是否启用电流检测，0波特率9600，1 57600 , 2 115200
     settings.endGroup();

    //下发参数
    Motor1.Motor_state = Parameter;     //参数设置
    ID_location = 0;
    Motor1.Group =0;
    timer1->start(200);    //定时一秒
    ui->label_information->setText("功能设置中");
    ui->label_information->show();


}


void Parameter_window::on_pushButton_exit_clicked()
{
    //退出
    if(Motor1.Motor_state != Parameter)
    {
        timer1->stop();
        this->close();
        user_instructions *U = new user_instructions;
        U->show();
    }

}

