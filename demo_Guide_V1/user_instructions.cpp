#include "user_instructions.h"
#include "ui_user_instructions.h"


user_instructions::user_instructions(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::user_instructions)
{
    ui->setupUi(this);

    // 需要开机隐藏的控件
    HideControls();

    //读取配置
    Config_init();

    //判断项目状态
    CheckProjectdate();

    //按键设置
    Key_Config();


  //  ui->pushButton->setEnabled(false);
  this->setStyleSheet("#user_instructions{background-image: url(:/back/Image/button/password.png);}");      //:/back/Image/.png
    //:/back/Image/button/password.png
    ui->lineEdit_passwork->installEventFilter(this);
    Motor1.Motor_state = Stop;


    //操作日记
    QFileInfo file_op(DIARY_OPERRATION);

    if(!file_op.isFile())       //判断文件如果不存在 ,就创建默认值文件
    {
         QSettings settings(DIARY_OPERRATION,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
         settings.beginGroup("number");
         settings.setValue("all",0);
         settings.setValue("now",0);
         settings.endGroup();

    }
    QSettings settings(DIARY_OPERRATION,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
    int all = settings.value("number/all").toInt() +1;
    settings.beginGroup(QString::number(all));
    settings.setValue("MainID",User_information.MainID);
    settings.setValue("Action","开机");
    settings.setValue("User",User_information.User);
    settings.setValue("time",now_data);
    settings.endGroup();

    settings.beginGroup("number");
    settings.setValue("all",all);
    settings.endGroup();





}


user_instructions::~user_instructions()
{
    delete ui;
}

void user_instructions::Key_Config()
{

    //按键初始化 background-color: rgb(207, 237, 255);  color: rgb(6, 93, 139);
    ui->pushButton_01->setStyleSheet("QPushButton{background-color: rgb(207, 237, 255);border:0px groove gray;border-radius:35px;padding:0px 0px;border-style: outset; color: rgb(6, 93, 139);}"  //
                                           "QPushButton:hover{background-color: rgb(207, 237, 255); color: rgb(6, 93, 139);}"  //按键本色
                                           "QPushButton:pressed{ background-color: rgb(207, 237, 255);border-style: inset;}");
    ui->pushButton_02->setStyleSheet("QPushButton{background-color: rgb(207, 237, 255);border:0px groove gray;border-radius:35px;padding:0px 0px;border-style: outset; color: rgb(6, 93, 139);}"  //
                                           "QPushButton:hover{background-color: rgb(207, 237, 255); color: rgb(6, 93, 139);}"  //按键本色
                                           "QPushButton:pressed{ background-color: rgb(207, 237, 255);border-style: inset;}");
    ui->pushButton_03->setStyleSheet("QPushButton{background-color: rgb(207, 237, 255);border:0px groove gray;border-radius:35px;padding:0px 0px;border-style: outset; color: rgb(6, 93, 139);}"  //
                                           "QPushButton:hover{background-color: rgb(207, 237, 255); color: rgb(6, 93, 139);}"  //按键本色
                                           "QPushButton:pressed{ background-color: rgb(207, 237, 255);border-style: inset;}");
    ui->pushButton_04->setStyleSheet("QPushButton{background-color: rgb(207, 237, 255);border:0px groove gray;border-radius:35px;padding:0px 0px;border-style: outset; color: rgb(6, 93, 139);}"  //
                                           "QPushButton:hover{background-color: rgb(207, 237, 255); color: rgb(6, 93, 139);}"  //按键本色
                                           "QPushButton:pressed{ background-color: rgb(207, 237, 255);border-style: inset;}");
    ui->pushButton_05->setStyleSheet("QPushButton{background-color: rgb(207, 237, 255);border:0px groove gray;border-radius:35px;padding:0px 0px;border-style: outset; color: rgb(6, 93, 139);}"  //
                                           "QPushButton:hover{background-color: rgb(207, 237, 255); color: rgb(6, 93, 139);}"  //按键本色
                                           "QPushButton:pressed{ background-color: rgb(207, 237, 255);border-style: inset;}");
    ui->pushButton_06->setStyleSheet("QPushButton{background-color: rgb(207, 237, 255);border:0px groove gray;border-radius:35px;padding:0px 0px;border-style: outset; color: rgb(6, 93, 139);}"  //
                                           "QPushButton:hover{background-color: rgb(207, 237, 255); color: rgb(6, 93, 139);}"  //按键本色
                                           "QPushButton:pressed{ background-color: rgb(207, 237, 255);border-style: inset;}");
    ui->pushButton_07->setStyleSheet("QPushButton{background-color: rgb(207, 237, 255);border:0px groove gray;border-radius:35px;padding:0px 0px;border-style: outset; color: rgb(6, 93, 139);}"  //
                                           "QPushButton:hover{background-color: rgb(207, 237, 255); color: rgb(6, 93, 139);}"  //按键本色
                                           "QPushButton:pressed{ background-color: rgb(207, 237, 255);border-style: inset;}");
    ui->pushButton_08->setStyleSheet("QPushButton{background-color: rgb(207, 237, 255);border:0px groove gray;border-radius:35px;padding:0px 0px;border-style: outset; color: rgb(6, 93, 139);}"  //
                                           "QPushButton:hover{background-color: rgb(207, 237, 255); color: rgb(6, 93, 139);}"  //按键本色
                                           "QPushButton:pressed{ background-color: rgb(207, 237, 255);border-style: inset;}");
    ui->pushButton_09->setStyleSheet("QPushButton{background-color: rgb(207, 237, 255);border:0px groove gray;border-radius:35px;padding:0px 0px;border-style: outset; color: rgb(6, 93, 139);}"  //
                                           "QPushButton:hover{background-color: rgb(207, 237, 255); color: rgb(6, 93, 139);}"  //按键本色
                                           "QPushButton:pressed{ background-color: rgb(207, 237, 255);border-style: inset;}");
    ui->pushButton_point->setStyleSheet("QPushButton{background-color: rgb(207, 237, 255);border:0px groove gray;border-radius:35px;padding:0px 0px;border-style: outset; color: rgb(6, 93, 139);}"  //
                                           "QPushButton:hover{background-color: rgb(207, 237, 255); color: rgb(6, 93, 139);}"  //按键本色
                                           "QPushButton:pressed{ background-color: rgb(207, 237, 255);border-style: inset;}");
    ui->pushButton_00->setStyleSheet("QPushButton{background-color: rgb(207, 237, 255);border:0px groove gray;border-radius:35px;padding:0px 0px;border-style: outset; color: rgb(6, 93, 139);}"  //
                                           "QPushButton:hover{background-color: rgb(207, 237, 255); color: rgb(6, 93, 139);}"  //按键本色
                                           "QPushButton:pressed{ background-color: rgb(207, 237, 255);border-style: inset;}");
    ui->pushButton_NO->setStyleSheet("QPushButton{background-color: rgb(207, 237, 255);border:0px groove gray;border-radius:35px;padding:0px 0px;border-style: outset; color: rgb(6, 93, 139);}"  //
                                           "QPushButton:hover{background-color: rgb(207, 237, 255); color: rgb(6, 93, 139);}"  //按键本色
                                           "QPushButton:pressed{ background-color: rgb(207, 237, 255);border-style: inset;}");
    ui->pushButton_del->setStyleSheet("QPushButton{background-color: rgb(207, 237, 255);border:0px groove gray;border-radius:35px;padding:0px 0px;border-style: outset; color: rgb(6, 93, 139);}"  //
                                           "QPushButton:hover{background-color: rgb(207, 237, 255); color: rgb(6, 93, 139);}"  //按键本色
                                           "QPushButton:pressed{ background-color: rgb(207, 237, 255);border-style: inset;}");
    ui->pushButton_ok->setStyleSheet("QPushButton{background-color: rgb(232, 173, 86);border:0px groove gray;border-radius:35px;padding:0px 0px;border-style: outset; color: rgb(255, 255, 255);}"  //
                                           "QPushButton:hover{background-color: rgb(232, 173, 86);rgb(255, 255, 255);}"  //按键本色  background-color: rgb(232, 173, 86);
                                           "QPushButton:pressed{background-color: rgb(232, 173, 86);border-style: inset;}");

}

void user_instructions::resizeEvent(QResizeEvent *event)
{
    //    int x = this->frameGeometry().width();      //  获取ui窗口宽度 带边框
    //    int y = this->frameGeometry().height();    //   获取窗口高度
        int x = this->geometry().width();      //  获取ui窗口宽度 不带边框
        int y = this->geometry().height();     //  获取窗口高度

        qDebug() << "x:" <<x << "y:"<<y;
}

void user_instructions::HideControls()
{
    //隐藏控件
    ui->label_StateInformation->hide();
    ui->label_passworderror->hide();
    ui->label_admininformation->hide();
    ui->widget->hide();
    ui->lineEdit_passwork->hide();
    ui->label_instructions->hide();
}


bool user_instructions::eventFilter(QObject *watch, QEvent *event)
{
    //获取密码输入框焦点
    if((watch == ui->lineEdit_passwork) && (event->type() == QEvent::MouseButtonPress))
    {

       ui->widget->show();
    }
    return false;

}

void user_instructions::on_pushButton_clicked()
{
    //确定同意按钮
    //跳转登录页面
    this->close();
    Parameter_window *P = new Parameter_window;
    P->show();




}

void user_instructions::Config_init()
{

    //读写配置
   //写配置
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
   settings.setValue("State","true");
   settings.setValue("ID","202192130568");
   settings.setValue("UserName","李雪");
   settings.setValue("UserPhone","13545906840");
   settings.endGroup();
   }

       //读取配置
       QSettings settingsread(CONFIG_NAME,QSettings::IniFormat);
       project_name = settingsread.value("Seting/project").toString();
       PassWork0 = settingsread.value("Seting/V0KEY").toString();
       PassWork1 = settingsread.value("Seting/V1KEY").toString();
       PassWork2 = settingsread.value("Seting/V2KEY").toString();
       UserName = settingsread.value("Seting/UserName").toString();
       UserPhone = settingsread.value("Seting/UserPhone").toString();
       State     = settingsread.value("Seting/State").toString();
       a[0] = settingsread.value("Seting/year").toInt();   // Year
       a[1] = settingsread.value("Seting/month").toInt();  //Month
       a[2] = settingsread.value("Seting/day").toInt();    // Day
       User_information.MainID = settingsread.value("Seting/ID").toString();    // ID
//       QStringList KeyList = settingsread.allKeys();


}


void user_instructions::CheckProjectdate()
{
    //检查项目
    //判断项目名字是否存在
   if(project_name == "")
    {
        //不存在未绑定
       QString  bind = QString("设备ID：%1 未与后台绑定，如需使用请联系管理员").arg(ID);
       QString  User = QString("爬架管理员信息：%1 %2").arg(UserName).arg(UserPhone);
       ui->label_StateInformation->setText(bind);
       ui->label_StateInformation->show();

       ui->label_admininformation->setText(User);
       ui->label_admininformation->show();
       ui->lineEdit_passwork->setPlaceholderText("请输入激活码");

       ui->lineEdit_passwork->show();

       ui->widget->show();
       return;
    }

   //判断是否被锁定
   if(State == "false")
   {
       //项目被锁定
      QString  lock = QString("设备ID：%1 被远程锁定，如需使用请联系管理员").arg(ID);
      QString  User = QString("爬架管理员信息：%1 %2").arg(UserName).arg(UserPhone);
      ui->label_StateInformation->setText(lock);
      ui->label_StateInformation->show();
      ui->label_admininformation->setText(User);
      ui->label_admininformation->show();
      ui->lineEdit_passwork->hide();
      ui->label_instructions->hide();
      ui->widget->hide();
      return;
   }

   //使用是否到期
   //检查项目是否过期
 DAY = QDate::currentDate();
 int b[3];             //  b数组保存当前时间，a数组保存运行到期数组
 b[0] =DAY.year();
 b[1] =DAY.month();
 b[2] =DAY.day();
 int lastday =( a[0] -b[0])*365 +(a[1] - b[1])*30 +a[2] -b[2];

     if(lastday < 0)
     {
         //项目到期
         QString  project = QString("设备ID：%1 已超过使用期限，如需使用请联系管理员").arg(ID);
         QString  User = QString("爬架管理员信息：%1 %2").arg(UserName).arg(UserPhone);
         ui->label_StateInformation->setText(project);
         ui->label_StateInformation->show();

         ui->label_admininformation->setText(User);
         ui->label_admininformation->show();
         ui->lineEdit_passwork->show();
         ui->label_instructions->show();
         ui->widget->show();
         ui->lineEdit_passwork->setPlaceholderText("请输入超级管理员口令");
         data_fale = false;
          return;
     }

  data_fale = true;
  ui->lineEdit_passwork->setPlaceholderText("请输入口令");
  ui->label_instructions->show();
  ui->lineEdit_passwork->show();
  ui->widget->show();


}




void user_instructions::on_pushButton_NO_clicked()
{
    //取消
    KeyValue.clear();

    ui->lineEdit_passwork->clear();

}


void user_instructions::on_pushButton_del_clicked()
{
    //删除
   KeyValue = KeyValue.left(KeyValue.size() -1);
   ui->lineEdit_passwork->setText(KeyValue);

}


void user_instructions::on_pushButton_ok_clicked()
{
    if(project_name == "")      //  未激活
    {
        //后台校验

        //校验成功  检验失败

        project_name =    KeyValue;     //  需要和后台校验

        //保存校验
        QFileInfo file_op(CONFIG_NAME);
        QSettings settings(CONFIG_NAME,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
        settings.beginGroup("Seting");
        settings.setValue("project",project_name);
        settings.endGroup();

        //转换成密码输入
         HideControls();
         CheckProjectdate();

    }else                       //已经激活
    {


    if(data_fale)
    {
        //项目未过期
        if(KeyValue == PassWork0)
        {
            //超级管理员
            User_information.User = "超级管理员";
            //操作日记
            QFileInfo file_op(DIARY_OPERRATION);

            if(!file_op.isFile())       //判断文件如果不存在 ,就创建默认值文件
            {
                 QSettings settings(DIARY_OPERRATION,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
                 settings.beginGroup("number");
                 settings.setValue("all",0);
                 settings.setValue("now",0);
                 settings.endGroup();

            }

            QSettings settings(DIARY_OPERRATION,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
            int all = settings.value("number/all").toInt() +1;
            settings.beginGroup(QString::number(all));
            settings.setValue("MainID",User_information.MainID);
            settings.setValue("Action","登录");
            settings.setValue("User",User_information.User);
            settings.setValue("time",now_data);
            settings.endGroup();

            settings.beginGroup("number");
            settings.setValue("all",all);
            settings.endGroup();

           Motor1.login = MASTER_SIGN_ON;     //登录

            this->close();
           if(Motor1.all_num == 0)      //跳转到扫描页面
           {
               configWindow *con = new configWindow;
               con->show();

           }else        //跳转到操作页面
           {
               oneWindow *one = new oneWindow;
               one->show();

            }



        }else if (KeyValue == PassWork1)
        {
            //现场管理员
            User_information.User = "现场管理员";
            //操作日记
            QFileInfo file_op(DIARY_OPERRATION);
            if(!file_op.isFile())       //判断文件如果不存在 ,就创建默认值文件
            {
                 QSettings settings(DIARY_OPERRATION,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
                 settings.beginGroup("number");
                 settings.setValue("all",0);
                 settings.setValue("now",0);
                 settings.endGroup();
            }

            QSettings settings(DIARY_OPERRATION,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径

            int all = settings.value("number/all").toInt() +1;
            settings.beginGroup(QString::number(all));
            settings.setValue("MainID",User_information.MainID);
            settings.setValue("Action","登录");
            settings.setValue("User",User_information.User);
            settings.setValue("time",now_data);
            settings.endGroup();

            settings.beginGroup("number");
            settings.setValue("all",all);
            settings.endGroup();

            Motor1.login = MASTER_SIGN_ON;     //登录

            this->close();
            if(Motor1.all_num == 0)      //跳转到扫描页面
            {
                configWindow *con = new configWindow;
                con->show();

            }else        //跳转到操作页面
            {
                oneWindow *one = new oneWindow;
                one->show();

             }


        }else if (KeyValue == PassWork2)
        {
             //操作员
            User_information.User = "现场管理员";
            //操作日记
            QFileInfo file_op(DIARY_OPERRATION);

            if(!file_op.isFile())       //判断文件如果不存在 ,就创建默认值文件
            {
                 QSettings settings(DIARY_OPERRATION,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
                 settings.beginGroup("number");
                 settings.setValue("all",0);
                 settings.setValue("now",0);
                 settings.endGroup();

            }

            QSettings settings(DIARY_OPERRATION,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径

            int all = settings.value("number/all").toInt() +1;
            settings.beginGroup(QString::number(all));
            settings.setValue("MainID",User_information.MainID);
            settings.setValue("Action","登录");
            settings.setValue("User",User_information.User);
            settings.setValue("time",now_data);
            settings.endGroup();

            settings.beginGroup("number");
            settings.setValue("all",all);
            settings.endGroup();
            Motor1.login = MASTER_SIGN_ON;     //登录
            this->close();
            if(Motor1.all_num == 0)      //跳转到扫描页面
            {
                configWindow *con = new configWindow;
                con->show();

            }else        //跳转到操作页面
            {
                oneWindow *one = new oneWindow;
                one->show();

             }
        }else
        {
            ui->label_passworderror->show();

        }


    }else
    {   // 项目过期
        if(KeyValue == PassWork0)
        {
            //超级管理员
            User_information.User = "超级管理员";
            //操作日记
            QFileInfo file_op(DIARY_OPERRATION);

            if(!file_op.isFile())       //判断文件如果不存在 ,就创建默认值文件
            {
                 QSettings settings(DIARY_OPERRATION,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
                 settings.beginGroup("number");
                 settings.setValue("all",0);
                 settings.setValue("now",0);
                 settings.endGroup();

            }

            QSettings settings(DIARY_OPERRATION,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径

            int all = settings.value("number/all").toInt() +1;
            settings.beginGroup(QString::number(all));
            settings.setValue("MainID",User_information.MainID);
            settings.setValue("Action","登录");
            settings.setValue("User",User_information.User);
            settings.setValue("time",now_data);
            settings.endGroup();

            settings.beginGroup("number");
            settings.setValue("all",all);
            settings.endGroup();

            Motor1.login = MASTER_SIGN_ON;     //登录
            this->close();
            if(Motor1.all_num == 0)      //跳转到扫描页面
            {
                configWindow *con = new configWindow;
                con->show();

            }else        //跳转到操作页面
            {
                oneWindow *one = new oneWindow;
                one->show();

             }

        }else
        {
             ui->label_passworderror->show();

        }


    }
    }
}


void user_instructions::on_pushButton_point_clicked()
{
    KeyValue += ".";
    ui->lineEdit_passwork->setText(KeyValue);

}


void user_instructions::on_pushButton_00_clicked()
{
      KeyValue += "0";
      ui->lineEdit_passwork->setText(KeyValue);

}



void user_instructions::on_pushButton_01_clicked()
{
     KeyValue += "1";
     ui->lineEdit_passwork->setText(KeyValue);

}

void user_instructions::on_pushButton_02_clicked()
{
     KeyValue += "2";
     ui->lineEdit_passwork->setText(KeyValue);

}

void user_instructions::on_pushButton_03_clicked()
{
     KeyValue += "3";
     ui->lineEdit_passwork->setText(KeyValue);

}

void user_instructions::on_pushButton_04_clicked()
{
     KeyValue += "4";
     ui->lineEdit_passwork->setText(KeyValue);

}

void user_instructions::on_pushButton_05_clicked()
{
     KeyValue += "5";
     ui->lineEdit_passwork->setText(KeyValue);

}

void user_instructions::on_pushButton_06_clicked()
{
     KeyValue += "6";
     ui->lineEdit_passwork->setText(KeyValue);

}

void user_instructions::on_pushButton_07_clicked()
{
     KeyValue += "7";
     ui->lineEdit_passwork->setText(KeyValue);

}


void user_instructions::on_pushButton_08_clicked()
{
     KeyValue += "8";
     ui->lineEdit_passwork->setText(KeyValue);

}


void user_instructions::on_pushButton_09_clicked()
{
     KeyValue += "9";
     ui->lineEdit_passwork->setText(KeyValue);

}

