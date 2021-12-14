#include "passwork.h"
#include "ui_passwork.h"

passwork::passwork(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::passwork)
{
    ui->setupUi(this);
    Config_init();

}

passwork::~passwork()
{
    delete ui;
}

void passwork::on_okButton_clicked()
{

    //确认按键
    QString passw;
    passw = ui->lineEdit->text();
    if(passw == PassWork2 || passw == PassWork1)    //普通登录
    {
        //判断项目是否过期
        CheckProjectdate();

    }else if (passw == PassWork0)
    {
        //高级管理员 直接进入主页面





    }else
    {
        //密码错误
        ui->lineEdit->clear();
        ui->lineEdit->setPlaceholderText("密码错误请重新输入");


    }

}


void passwork::on_pushButton_2_clicked()
{
    //取消登录
    this->hide();
   user_instructions *user1 = new user_instructions;

    user1->show();


}



void passwork::Config_init()
{
    //读取配置
    QFileInfo file(CONFIG_NAME);
    if(!file.isFile())       //判断文件如果不存在 ,就创建默认值文件
    {
    QSettings settings(CONFIG_NAME,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
    settings.beginGroup("Seting");
    settings.setValue("year","2021");
    settings.setValue("month","12");
    settings.setValue("day","12");
    settings.setValue("project","");
    settings.setValue("V0KEY","1234567");
    settings.setValue("V1KEY","123456");
    settings.setValue("V2KEY","66");

    settings.endGroup();
    }

        //读取配置
      QSettings settingsread(CONFIG_NAME,QSettings::IniFormat);
      PassWork0 = settingsread.value("Seting/V0KEY").toString();
      PassWork1 = settingsread.value("Seting/V1KEY").toString();
      PassWork2 = settingsread.value("Seting/V2KEY").toString();
      a[0] = settingsread.value("Seting/year").toInt();   // Year
      a[1] = settingsread.value("Seting/month").toInt();  //Month
      a[2] = settingsread.value("Seting/day").toInt();    // Day


}

void passwork::CheckProjectdate()
{
    //检查项目是否过期
  DAY = QDate::currentDate();
  int b[3];             //  b数组保存当前时间，a数组保存运行到期数组
  b[0] =DAY.year();
  b[1] =DAY.month();
  b[2] =DAY.day();
  qDebug() <<DAY.year() <<DAY.month()<< DAY.day();
  int lastday =( a[0] -b[0])*365 +(a[1] - b[1])*30 +a[2] -b[2];

      if(lastday < 0)
      {
          //项目到期
          QString  cmd ;
          QString  project = "保利单元3栋A单元";
          cmd += " 当前项目： ";
          cmd += project;
          cmd += "已过使用期限 \n 如有需要继续使用，请联系管理员延长使用时间";


          BOX.warning(NULL,"项目过期 ",cmd);

           qDebug() << "项目过期" ;
           return;
      }



  //进入主页面




}

