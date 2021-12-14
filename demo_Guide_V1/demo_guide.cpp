#include "demo_guide.h"
#include "ui_demo_guide.h"

demo_guide::demo_guide(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::demo_guide)
{
    ui->setupUi(this);

}

demo_guide::~demo_guide()
{
    delete ui;
}

//void demo_guide::Config_init()
//{
//    //读写配置
//   //写配置
//   QFileInfo file(CONFIG_NAME);
//   if(!file.isFile())       //判断文件如果不存在 ,就创建默认值文件
//   {
//   QSettings settings(CONFIG_NAME,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
//   settings.beginGroup("Seting");
//   settings.setValue("project","");
//   settings.setValue("V0KEY","123456");
//   settings.setValue("V1KEY","1234");
//   settings.setValue("V2KEY","123");

//   settings.endGroup();
//   }

//       //读取配置
//       QSettings settingsread(CONFIG_NAME,QSettings::IniFormat);
//       project_name = settingsread.value("Seting/project").toString();

//       qDebug() <<project_name;


//}




void demo_guide::on_bandButton_clicked()
{
    //确定绑定按钮
    this->close();
   user_instructions *user2 = new user_instructions;
   user2->show();
   emit opendSerial();

//    project_name = ui->lineEdit_projectName->text();        //  获取项目编号
//    if(project_name !="")
//    {
//        //项目有效
//      QSettings settings(CONFIG_NAME,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
//      settings.beginGroup("Seting");
//      settings.setValue("project",project_name);
//      settings.endGroup();



//    }else
//    {
//        ui->lineEdit_projectName->setPlaceholderText("项目编号有误，请从新输入");

//    }

}

