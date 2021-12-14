#include "configwindow.h"
#include "ui_configwindow.h"

configWindow::configWindow(QWidget *parent)
    : QMainWindow(parent),
    ui(new Ui::configWindow)
{
    ui->setupUi(this);
    ui->widget_Key->hide();
    ui->widget_SCAN->hide();
    ui->pushButton_ScanSave->hide();
    ui->pushButton_ScanCancel->hide();
    ui->label_ChangeOK->hide();
    ui->label_Nomotor->hide();

    //按键样式初始化
    Key_Config();

     //设置表格
    tablewigetSet();
    //Config_Setting("1");

    this->setStyleSheet("#configWindow{background-image: url(:/back/Image/button/config.png);}");      //:/back/Image/2.png

    //定时器
    timer4 = new QTimer(this);   //定时更新
    connect(timer4,SIGNAL(timeout()),this,SLOT(TimeOut4()));

    timer2 = new QTimer(this);   //定时更新
    connect(timer2,SIGNAL(timeout()),this,SLOT(TimeOut2()));
    timer2->start(1000);      //一秒中断一次

    timer3 = new QTimer(this);   //定时更新
    connect(timer3,SIGNAL(timeout()),this,SLOT(TimeOut3()));



    //初始化

    Motor1.Group = 0;
    for (int i = 0; i  < Motor1.page1_num ;i++ )    //设置1组状态为选中
    {

        Motor1.branch_oneGroup[i].branch.choose = true;
    }
    for (int i = 0; i  < Motor1.page2_num ;i++ )    //设置2组状态为选中
    {

        Motor1.branch_twoGroup[i].branch.choose = true;
    }

    ui->checkBox->setCheckState(Qt::Checked);
    tablewiget_SetData(Motor1.Group,Motor1);
    CheckBox_num = Motor1.all_num;
    Motor1.Motor_state = Wait;
    ui->pushButton_allGroup->setText(QString("全部(%1)").arg(Motor1.all_num));
    ui->pushButton_oneGroup->setText( QString("1组(%1)").arg(Motor1.page1_num));
    ui->pushButton_twoGroup->setText(QString("2组(%1)").arg(Motor1.page2_num));

    //设置按钮颜色
    ui->pushButton_allGroup->setStyleSheet("QPushButton{color: rgb(255, 255, 255);background-color: rgb(9, 82, 120);}");  //选中
    ui->pushButton_oneGroup->setStyleSheet("QPushButton{color: rgb(0, 0, 0);background-color: rgb(200, 200, 200);}");  //未选中
    ui->pushButton_twoGroup->setStyleSheet("QPushButton{color: rgb(0, 0, 0);background-color: rgb(200, 200, 200);}");  //未选中

     ui->lineEdit_load->installEventFilter(this);
     ui->lineEdit_loss->installEventFilter(this);
     ui->lineEdit_loose->installEventFilter(this);
     ui->lineEdit_tight->installEventFilter(this);
     ui->lineEdit_current->installEventFilter(this);

     if(Motor1.all_num == 0)                //没有单控，直接启动扫描
     {
         QString Sty = "QLabel{background-color: rgb(165, 165, 165);color: rgb(255, 255, 255);}";
         for (int i =1; i < 151 ;i++ )
         {
             label_NumSet(i,Sty);
         }
         ui->pushButton_ScanOrStop->show();
         ui->pushButton_ScanCancel->hide();
         ui->pushButton_ScanSave->hide();

         QString num = QString("扫描分控数量：%1/%2").arg(Motor1.all_num).arg(ALL_Num);
         ui->label_ScanNum->setText(num);
         ui->label_SCANLabel->setText("分控扫描中....");
         ui->widget_SCAN->show();       //显示控件
         Motor1.Motor_state = Scan;      //  扫描
         ID_location = 1;  //从机位号1开始扫描
         Motor1.Group = 0;
         timer4->start(200);
     }

}

configWindow::~configWindow()
{
    delete timer4;
    delete timer2;
    delete timer3;
    delete ui;
}

void configWindow::tablewigetSet()
{

    //表格设置
      ui->tableWidget->setColumnCount(8);

    ui->tableWidget->setWindowTitle("DEMO");
  //  ui->tableWidget->resize(350,200);

//    QHeaderView *hearder = ui->tableWidget->verticalHeader();   //隐藏左边行数
  //  hearder->setHidden(true);    //隐藏左边行数
    QStringList header;
    header<<""<<"组号"<< "机位号" << "过载值/T" << "失载值/T"<< "紧钩值/T" << "松钩值/T"  << "电流值/A";
    ui->tableWidget->setHorizontalHeaderLabels(header);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    //设置列宽
//    ui->tableWidget->setColumnWidth(0,50);
//    ui->tableWidget->setColumnWidth(1,200);
//    ui->tableWidget->setColumnWidth(2,200);
//    ui->tableWidget->setColumnWidth(3,200);



     connect(ui->tableWidget,SIGNAL(itemClicked(QTableWidgetItem *)),this, SLOT( getItem(QTableWidgetItem *)) );  //获取单元格点击事件

}

void configWindow::Key_Config()
{
    //按键样式初始化
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

void configWindow::label_NumSet(int ID,QString Style)
{
    //扫描ID个数 显示状态
    //background-color: rgb(0, 170, 255);  "QPushButton{background-image: url(:/back/Image/button/113.png);
    switch (ID)
    {
    case 1:ui->label->setStyleSheet(Style);break;
    case 2:ui->label_2->setStyleSheet(Style);break;
    case 3:ui->label_3->setStyleSheet(Style);break;
    case 4:ui->label_4->setStyleSheet(Style);break;
    case 5:ui->label_5->setStyleSheet(Style);break;
    case 6:ui->label_6->setStyleSheet(Style);break;
    case 7:ui->label_7->setStyleSheet(Style);break;
    case 8:ui->label_8->setStyleSheet(Style);break;
    case 9:ui->label_9->setStyleSheet(Style);break;
    case 10:ui->label_10->setStyleSheet(Style);break;
    case 11:ui->label_11->setStyleSheet(Style);break;
    case 12:ui->label_12->setStyleSheet(Style);break;
    case 13:ui->label_13->setStyleSheet(Style);break;
    case 14:ui->label_14->setStyleSheet(Style);break;
    case 15:ui->label_15->setStyleSheet(Style);break;
    case 16:ui->label_16->setStyleSheet(Style);break;
    case 17:ui->label_17->setStyleSheet(Style);break;
    case 18:ui->label_18->setStyleSheet(Style);break;
    case 19:ui->label_19->setStyleSheet(Style);break;
    case 20:ui->label_20->setStyleSheet(Style);break;
    case 21:ui->label_21->setStyleSheet(Style);break;
    case 22:ui->label_22->setStyleSheet(Style);break;
    case 23:ui->label_23->setStyleSheet(Style);break;
    case 24:ui->label_24->setStyleSheet(Style);break;
    case 25:ui->label_25->setStyleSheet(Style);break;
    case 26:ui->label_26->setStyleSheet(Style);break;
    case 27:ui->label_27->setStyleSheet(Style);break;
    case 28:ui->label_28->setStyleSheet(Style);break;
    case 29:ui->label_29->setStyleSheet(Style);break;
    case 30:ui->label_30->setStyleSheet(Style);break;
    case 31:ui->label_31->setStyleSheet(Style);break;
    case 32:ui->label_32->setStyleSheet(Style);break;
    case 33:ui->label_33->setStyleSheet(Style);break;
    case 34:ui->label_34->setStyleSheet(Style);break;
    case 35:ui->label_35->setStyleSheet(Style);break;
    case 36:ui->label_36->setStyleSheet(Style);break;
    case 37:ui->label_37->setStyleSheet(Style);break;
    case 38:ui->label_38->setStyleSheet(Style);break;
    case 39:ui->label_39->setStyleSheet(Style);break;
    case 40:ui->label_40->setStyleSheet(Style);break;
    case 41:ui->label_41->setStyleSheet(Style);break;
    case 42:ui->label_42->setStyleSheet(Style);break;
    case 43:ui->label_43->setStyleSheet(Style);break;
    case 44:ui->label_44->setStyleSheet(Style);break;
    case 45:ui->label_45->setStyleSheet(Style);break;
    case 46:ui->label_46->setStyleSheet(Style);break;
    case 47:ui->label_47->setStyleSheet(Style);break;
    case 48:ui->label_48->setStyleSheet(Style);break;
    case 49:ui->label_49->setStyleSheet(Style);break;
    case 50:ui->label_50->setStyleSheet(Style);break;
    case 51:ui->label_51->setStyleSheet(Style);break;
    case 52:ui->label_52->setStyleSheet(Style);break;
    case 53:ui->label_53->setStyleSheet(Style);break;
    case 54:ui->label_54->setStyleSheet(Style);break;
    case 55:ui->label_55->setStyleSheet(Style);break;
    case 56:ui->label_56->setStyleSheet(Style);break;
    case 57:ui->label_57->setStyleSheet(Style);break;
    case 58:ui->label_58->setStyleSheet(Style);break;
    case 59:ui->label_59->setStyleSheet(Style);break;
    case 60:ui->label_60->setStyleSheet(Style);break;
    case 61:ui->label_61->setStyleSheet(Style);break;
    case 62:ui->label_62->setStyleSheet(Style);break;
    case 63:ui->label_63->setStyleSheet(Style);break;
    case 64:ui->label_64->setStyleSheet(Style);break;
    case 65:ui->label_65->setStyleSheet(Style);break;
    case 66:ui->label_66->setStyleSheet(Style);break;
    case 67:ui->label_67->setStyleSheet(Style);break;
    case 68:ui->label_68->setStyleSheet(Style);break;
    case 69:ui->label_69->setStyleSheet(Style);break;
    case 70:ui->label_70->setStyleSheet(Style);break;
    case 71:ui->label_71->setStyleSheet(Style);break;
    case 72:ui->label_72->setStyleSheet(Style);break;
    case 73:ui->label_73->setStyleSheet(Style);break;
    case 74:ui->label_74->setStyleSheet(Style);break;
    case 75:ui->label_75->setStyleSheet(Style);break;
    case 76:ui->label_76->setStyleSheet(Style);break;
    case 77:ui->label_77->setStyleSheet(Style);break;
    case 78:ui->label_78->setStyleSheet(Style);break;
    case 79:ui->label_79->setStyleSheet(Style);break;
    case 80:ui->label_80->setStyleSheet(Style);break;
    case 81:ui->label_81->setStyleSheet(Style);break;
    case 82:ui->label_82->setStyleSheet(Style);break;
    case 83:ui->label_83->setStyleSheet(Style);break;
    case 84:ui->label_84->setStyleSheet(Style);break;
    case 85:ui->label_85->setStyleSheet(Style);break;
    case 86:ui->label_86->setStyleSheet(Style);break;
    case 87:ui->label_87->setStyleSheet(Style);break;
    case 88:ui->label_88->setStyleSheet(Style);break;
    case 89:ui->label_89->setStyleSheet(Style);break;
    case 90:ui->label_90->setStyleSheet(Style);break;
    case 91:ui->label_91->setStyleSheet(Style);break;
    case 92:ui->label_92->setStyleSheet(Style);break;
    case 93:ui->label_93->setStyleSheet(Style);break;
    case 94:ui->label_94->setStyleSheet(Style);break;
    case 95:ui->label_95->setStyleSheet(Style);break;
    case 96:ui->label_96->setStyleSheet(Style);break;
    case 97:ui->label_97->setStyleSheet(Style);break;
    case 98:ui->label_98->setStyleSheet(Style);break;
    case 99:ui->label_99->setStyleSheet(Style);break;
    case 100:ui->label_100->setStyleSheet(Style);break;
    case 101:ui->label_101->setStyleSheet(Style);break;
    case 102:ui->label_102->setStyleSheet(Style);break;
    case 103:ui->label_103->setStyleSheet(Style);break;
    case 104:ui->label_104->setStyleSheet(Style);break;
    case 105:ui->label_105->setStyleSheet(Style);break;
    case 106:ui->label_106->setStyleSheet(Style);break;
    case 107:ui->label_107->setStyleSheet(Style);break;
    case 108:ui->label_108->setStyleSheet(Style);break;
    case 109:ui->label_109->setStyleSheet(Style);break;
    case 110:ui->label_110->setStyleSheet(Style);break;
    case 111:ui->label_111->setStyleSheet(Style);break;
    case 112:ui->label_112->setStyleSheet(Style);break;
    case 113:ui->label_113->setStyleSheet(Style);break;
    case 114:ui->label_114->setStyleSheet(Style);break;
    case 115:ui->label_115->setStyleSheet(Style);break;
    case 116:ui->label_116->setStyleSheet(Style);break;
    case 117:ui->label_117->setStyleSheet(Style);break;
    case 118:ui->label_118->setStyleSheet(Style);break;
    case 119:ui->label_119->setStyleSheet(Style);break;
    case 120:ui->label_120->setStyleSheet(Style);break;
    case 121:ui->label_121->setStyleSheet(Style);break;
    case 122:ui->label_122->setStyleSheet(Style);break;
    case 123:ui->label_123->setStyleSheet(Style);break;
    case 124:ui->label_124->setStyleSheet(Style);break;
    case 125:ui->label_125->setStyleSheet(Style);break;
    case 126:ui->label_126->setStyleSheet(Style);break;
    case 127:ui->label_127->setStyleSheet(Style);break;
    case 128:ui->label_128->setStyleSheet(Style);break;
    case 129:ui->label_129->setStyleSheet(Style);break;
    case 130:ui->label_130->setStyleSheet(Style);break;
    case 131:ui->label_131->setStyleSheet(Style);break;
    case 132:ui->label_132->setStyleSheet(Style);break;
    case 133:ui->label_133->setStyleSheet(Style);break;
    case 134:ui->label_134->setStyleSheet(Style);break;
    case 135:ui->label_135->setStyleSheet(Style);break;
    case 136:ui->label_136->setStyleSheet(Style);break;
    case 137:ui->label_137->setStyleSheet(Style);break;
    case 138:ui->label_138->setStyleSheet(Style);break;
    case 139:ui->label_139->setStyleSheet(Style);break;
    case 140:ui->label_140->setStyleSheet(Style);break;
    case 141:ui->label_141->setStyleSheet(Style);break;
    case 142:ui->label_142->setStyleSheet(Style);break;
    case 143:ui->label_143->setStyleSheet(Style);break;
    case 144:ui->label_144->setStyleSheet(Style);break;
    case 145:ui->label_145->setStyleSheet(Style);break;
    case 146:ui->label_146->setStyleSheet(Style);break;
    case 147:ui->label_147->setStyleSheet(Style);break;
    case 148:ui->label_148->setStyleSheet(Style);break;
    case 149:ui->label_149->setStyleSheet(Style);break;
    case 150:ui->label_150->setStyleSheet(Style);break;
    default:break;

    }




}

void configWindow::TimeOut4()
{
    //定时器回调
   QString Sty = "QLabel{background-color: rgb(0, 170, 255);color: rgb(255, 255, 255);}";

   //更新扫描数据
    if(Motor1_new.all_num >0)
    {

        for (int i = 0;i <Motor1_new.all_num ;i++ )
        {
            label_NumSet(Motor1_new.all_id[i],Sty);


        }
    }

    //设置数据
    QString num = QString("扫描分控数量：%1/%2").arg(Motor1_new.all_num).arg(ALL_Num);
    ui->label_ScanNum->setText(num);

    //判断扫描是否结束
    if(Motor1.Motor_state == Stop)
    {
        //扫面结束
        timer4->stop();
        ui->pushButton_ScanOrStop->hide();
        ui->pushButton_ScanSave->show();
        ui->pushButton_ScanCancel->show();
        ui->label_SCANLabel->setText("扫描完成");

        int Check_num = ALL_Num - Motor1_new.all_num;
         QString check;
        if(Check_num > 0)
        {
             check = QString("当前分控总数：%1/%2台   缺失%3台").arg(Motor1_new.all_num).arg(ALL_Num).arg(Check_num);

        }else if (Check_num < 0)
        {
            Check_num = Motor1_new.all_num - ALL_Num;
            check = QString("当前分控总数：%1/%2台   多出%3台").arg(Motor1_new.all_num).arg(ALL_Num).arg(Check_num);

        }else
        {
            check = QString("当前分控总数：%1/%2台 ").arg(Motor1_new.all_num).arg(ALL_Num);

        }
        ui->label_ScanNum->setText(check);
    }

}

void configWindow::TimeOut2()
{
    //定时器2回调
     ui->label_time->setText(now_data);  //更新时间


}

void configWindow::TimeOut3()
{
    //定时器3回调

    if(Motor1.Motor_state == Stop)
     {
        //设置完成
        ui->pushButton_change->setEnabled(true);
        ui->label_ChangeOK->setText("修改成功 (3)");
        Motor1.Motor_state = Wait;
        Save_finish = true;

    }
    if(Save_finish == true)
    {

        Set_fale--;
        ui->label_ChangeOK->setText(QString("修改成功(%1)").arg(Set_fale));
        if(Set_fale ==0)
        {
            Set_fale =3;
            Save_finish = false;
            timer3->stop();
            ui->label_ChangeOK->hide();
        }

    }

}

//设置lineEdit只能输入整型或者浮点
//QDoubleValidator *lineDouble = new QDoubleValidator(0,100,3,nullptr);
//QIntValidator *intVa = new QIntValidator;
//intVa->setRange(1,100);
//ui->lineEdit_first->setValidator(lineDouble);

void configWindow::on_pushButton_comemain_clicked()
{
    //取消参数修改
    ui->lineEdit_current->clear();
    ui->lineEdit_load->clear();
    ui->lineEdit_loose->clear();
    ui->lineEdit_loss->clear();
    ui->lineEdit_tight->clear();
    KeyValue.clear();
    key_point = 0;

}


void configWindow::on_pushButton_change_clicked()
{
    //确定修改参数按钮

  if(Motor1.Motor_state != Set)
     {

    ui->pushButton_change->setEnabled(false);
    ui->label_ChangeOK->setText("参数修改中");
    ui->label_ChangeOK->show();

    //判断数据是否合理和是否有修改  "过载值/T" << "失载值/T"<< "紧钩值/T" << "松钩值/T"  << "电流值/A"
    Check_double[0] = ui->lineEdit_load->text().toDouble();
    Check_double[1] = ui->lineEdit_loss->text().toDouble();
    Check_double[2] = ui->lineEdit_tight->text().toDouble();
    Check_double[3] = ui->lineEdit_loose->text().toDouble();
    Check_double[4] = ui->lineEdit_current->text().toDouble();
 //   qDebug() << QString("过载值/T：%1 失载值/T：%2 紧钩值/T：%3 松钩值/T：%4   电流值/A：%5").arg(Check_double[0]).arg(Check_double[1]).arg(Check_double[2]).arg(Check_double[3]).arg(Check_double[4]);
    for (int i =0 ;i < 5 ;i++ )
    {
        Check_int[i] = Check_double[i]*10; //转换成int 放大10倍

        if(Check_int[i] > 0 && Check_int[i] < 60)   //判断数据是否有效
        {
            //数据有效
            Check_fale[i] = true;
        }else
        {
            Check_fale[i] = false;
            if(Check_int[i] >= 60)
            qDebug() << "数据有错";
        }

    }
// qDebug() << QString("过载值/T：%1 失载值/T：%2 紧钩值/T：%3 松钩值/T：%4   电流值/A：%5").arg(Check_int[0]).arg(Check_int[1]).arg(Check_int[2]).arg(Check_int[3]).arg(Check_int[4]);


    //修改数据

        if(Check_fale[0])  //过载值   //失载值  //紧钩值  //松钩值 //电流值
        {
            if(Motor1.Group == 1)
            {
                for (int i =0;i < Motor1.page1_num ;i++ )
                {
                    if(Motor1.branch_oneGroup[i].branch.choose) //选中
                    {
                        Motor1.branch_oneGroup[i].parameter.overload_value = Check_int[0];

                    }

                }

            }else if (Motor1.Group == 2)
            {
                for (int i =0;i < Motor1.page2_num ;i++ )
                {
                    if(Motor1.branch_twoGroup[i].branch.choose) //选中
                    {
                        Motor1.branch_twoGroup[i].parameter.overload_value = Check_int[0];

                    }

                }


            }else if (Motor1.Group == 0)
            {
                for (int i =0;i < Motor1.page1_num ;i++ )
                {
                    if(Motor1.branch_oneGroup[i].branch.choose) //选中
                    {
                        Motor1.branch_oneGroup[i].parameter.overload_value = Check_int[0];

                    }

                }

                for (int k =0;k < Motor1.page2_num ;k++ )
                {
                    if(Motor1.branch_twoGroup[k].branch.choose) //选中
                    {
                        Motor1.branch_twoGroup[k].parameter.overload_value = Check_int[0];

                    }

                }


            }
        }

        if(Check_fale[1])     //失载值
        {
            if(Motor1.Group == 1)
            {
                for (int i =0;i < Motor1.page1_num ;i++ )
                {
                    if(Motor1.branch_oneGroup[i].branch.choose) //选中
                    {
                        Motor1.branch_oneGroup[i].parameter.loss_value = Check_int[1];

                    }

                }

            }else if (Motor1.Group == 2)
            {
                for (int i =0;i < Motor1.page2_num ;i++ )
                {
                    if(Motor1.branch_twoGroup[i].branch.choose) //选中
                    {
                        Motor1.branch_twoGroup[i].parameter.loss_value = Check_int[1];

                    }

                }


            }else if (Motor1.Group == 0)
            {
                for (int i =0;i < Motor1.page1_num ;i++ )
                {
                    if(Motor1.branch_oneGroup[i].branch.choose) //选中
                    {
                        Motor1.branch_oneGroup[i].parameter.loss_value = Check_int[1];

                    }

                }

                for (int k =0;k < Motor1.page2_num ;k++ )
                {
                    if(Motor1.branch_twoGroup[k].branch.choose) //选中
                    {
                        Motor1.branch_twoGroup[k].parameter.loss_value = Check_int[1];

                    }

                }


            }
        }

        if(Check_fale[2])  //过载值   //失载值  //紧钩值  //松钩值 //电流值
        {
            if(Motor1.Group == 1)
            {
                for (int i =0;i < Motor1.page1_num ;i++ )
                {
                    if(Motor1.branch_oneGroup[i].branch.choose) //选中
                    {
                        Motor1.branch_oneGroup[i].parameter.tighten_value = Check_int[2];

                    }

                }

            }else if (Motor1.Group == 2)
            {
                for (int i =0;i < Motor1.page2_num ;i++ )
                {
                    if(Motor1.branch_twoGroup[i].branch.choose) //选中
                    {
                        Motor1.branch_twoGroup[i].parameter.tighten_value = Check_int[2];

                    }

                }


            }else if (Motor1.Group == 0)
            {
                for (int i =0;i < Motor1.page1_num ;i++ )
                {
                    if(Motor1.branch_oneGroup[i].branch.choose) //选中
                    {
                        Motor1.branch_oneGroup[i].parameter.tighten_value = Check_int[2];

                    }

                }

                for (int k =0;k < Motor1.page2_num ;k++ )
                {
                    if(Motor1.branch_twoGroup[k].branch.choose) //选中
                    {
                        Motor1.branch_twoGroup[k].parameter.tighten_value = Check_int[2];

                    }

                }


            }
        }

        if(Check_fale[3])  //过载值   //失载值  //紧钩值  //松钩值 //电流值
        {
            if(Motor1.Group == 1)
            {
                for (int i =0;i < Motor1.page1_num ;i++ )
                {
                    if(Motor1.branch_oneGroup[i].branch.choose) //选中
                    {
                        Motor1.branch_oneGroup[i].parameter.loose_value = Check_int[3];

                    }

                }

            }else if (Motor1.Group == 2)
            {
                for (int i =0;i < Motor1.page2_num ;i++ )
                {
                    if(Motor1.branch_twoGroup[i].branch.choose) //选中
                    {
                        Motor1.branch_twoGroup[i].parameter.loose_value = Check_int[3];

                    }

                }


            }else if (Motor1.Group == 0)
            {
                for (int i =0;i < Motor1.page1_num ;i++ )
                {
                    if(Motor1.branch_oneGroup[i].branch.choose) //选中
                    {
                        Motor1.branch_oneGroup[i].parameter.loose_value = Check_int[3];

                    }

                }

                for (int k =0;k < Motor1.page2_num ;k++ )
                {
                    if(Motor1.branch_twoGroup[k].branch.choose) //选中
                    {
                        Motor1.branch_twoGroup[k].parameter.loose_value = Check_int[3];

                    }

                }


            }
        }

        if(Check_fale[4])  //过载值   //失载值  //紧钩值  //松钩值 //电流值
        {
            if(Motor1.Group == 1)
            {
                for (int i =0;i < Motor1.page1_num ;i++ )
                {
                    if(Motor1.branch_oneGroup[i].branch.choose) //选中
                    {
                        Motor1.branch_oneGroup[i].parameter.current_value = Check_int[4];

                    }

                }

            }else if (Motor1.Group == 2)
            {
                for (int i =0;i < Motor1.page2_num ;i++ )
                {
                    if(Motor1.branch_twoGroup[i].branch.choose) //选中
                    {
                        Motor1.branch_twoGroup[i].parameter.current_value = Check_int[4];

                    }

                }


            }else if (Motor1.Group == 0)
            {
                for (int i =0;i < Motor1.page1_num ;i++ )
                {
                    if(Motor1.branch_oneGroup[i].branch.choose) //选中
                    {
                        Motor1.branch_oneGroup[i].parameter.current_value = Check_int[4];

                    }

                }

                for (int k =0;k < Motor1.page2_num ;k++ )
                {
                    if(Motor1.branch_twoGroup[k].branch.choose) //选中
                    {
                        Motor1.branch_twoGroup[k].parameter.current_value = Check_int[4];

                    }

                }


            }
        }

        //刷新表格
        ui->tableWidget->clearContents();
        Motor1.Group = 0;   //全选
       tablewiget_SetData( Motor1.Group,Motor1);  //显示全部参数

       //保存数据
        if(Motor1.Group == 1)
        {
            QFile fileOne(CONFIG_oneGroupNAME);
            fileOne.remove();
            Config_Setting(Motor1,CONFIG_oneGroupNAME);


        }else if (Motor1.Group == 2)
        {
            QFile fileTwo(CONFIG_twoGroupNAME);
            fileTwo.remove();
            Config_Setting(Motor1,CONFIG_twoGroupNAME);


        }else
        {
            Motor1.Group =1;
            QFile fileOne(CONFIG_oneGroupNAME);
            fileOne.remove();
            Config_Setting(Motor1,CONFIG_oneGroupNAME);

            Motor1.Group =2;
            QFile fileTwo(CONFIG_twoGroupNAME);
            fileTwo.remove();
            Config_Setting(Motor1,CONFIG_twoGroupNAME);
             Motor1.Group =0;

        }


       //下发数据
       Motor1.Motor_state =Set;              //设置状态为参数下发
       ID_location = 0;                     //设置起始地址
       timer3->start(200);

    }


}

void configWindow::getItem(QTableWidgetItem *item1)
{
    //获取表格点击回调

    //获取单元格点击事件
    if(item1->column() == 0)
    {
        if(Motor1.Group == 1)       //1组
        {
            if(item1->checkState() == Qt::Checked)
            {
               item1->setCheckState(Qt::Unchecked);
               Motor1.branch_oneGroup[item1->row()].branch.choose = NO;
               CheckBox_num--;
               ui->label_Selected->setText(QString("已选数量：%1").arg(CheckBox_num));   //设置已选数量

            }else if(item1->checkState() == Qt::Unchecked)
            {
                item1->setCheckState(Qt::Checked);
                 Motor1.branch_oneGroup[item1->row()].branch.choose = YES;
                 CheckBox_num++;
                 ui->label_Selected->setText(QString("已选数量：%1").arg(CheckBox_num));   //设置已选数量

            }

        }else if (Motor1.Group == 2)    //2组
        {
            if(item1->checkState() == Qt::Checked)
            {
               item1->setCheckState(Qt::Unchecked);
               Motor1.branch_twoGroup[item1->row() ].branch.choose = NO;
                CheckBox_num--;
                ui->label_Selected->setText(QString("已选数量：%1").arg(CheckBox_num));   //设置已选数量

            }else
            {
                item1->setCheckState(Qt::Checked);
                Motor1.branch_twoGroup[item1->row()].branch.choose = YES;
                 CheckBox_num++;
                 ui->label_Selected->setText(QString("已选数量：%1").arg(CheckBox_num));   //设置已选数量
            }

        }else if (Motor1.Group == 0)    //全部
        {
            if(item1->row() < Motor1.page1_num)         //1组
            {
                if(item1->checkState() == Qt::Checked)
                {
                   item1->setCheckState(Qt::Unchecked);
                   Motor1.branch_oneGroup[item1->row()].branch.choose = NO;
                   CheckBox_num--;
                   ui->label_Selected->setText(QString("已选数量：%1").arg(CheckBox_num));   //设置已选数量

                }else
                {
                    item1->setCheckState(Qt::Checked);
                    Motor1.branch_oneGroup[item1->row()].branch.choose = YES;
                    CheckBox_num++;
                    ui->label_Selected->setText(QString("已选数量：%1").arg(CheckBox_num));   //设置已选数量
                }

            }else                                    //2组
            {
                if(item1->checkState() == Qt::Checked)
                {
                   item1->setCheckState(Qt::Unchecked);
                   Motor1.branch_twoGroup[item1->row() - Motor1.page1_num].branch.choose = NO;
                   CheckBox_num--;
                   ui->label_Selected->setText(QString("已选数量：%1").arg(CheckBox_num));   //设置已选数量

                }else
                {
                    item1->setCheckState(Qt::Checked);
                    Motor1.branch_twoGroup[item1->row()  - Motor1.page1_num].branch.choose = YES;
                    CheckBox_num++;
                    ui->label_Selected->setText(QString("已选数量：%1").arg(CheckBox_num));   //设置已选数量
                }

            }


        }

    }
}


void configWindow::on_checkBox_clicked()
{
    //全选
  //  全选复选框状态改变
    if(Motor1.Group == 1)       //1组
    {
        if(ui->checkBox->isChecked())
        {
             CheckBox_num = Motor1.page1_num;   //选中个数为1组个数
             ui->label_Selected->setText(QString("已选数量：%1").arg(CheckBox_num));
            for (int i  = 0 ; i < Motor1.page1_num ; i++ )
            {
                 ui->tableWidget->item(i,0)->setCheckState(Qt::Checked);
                 Motor1.branch_oneGroup[i].branch.choose = YES;

            }


        }else
        {
            CheckBox_num = 0;
            ui->label_Selected->setText(QString("已选数量：%1").arg(CheckBox_num));
            for (int i  = 0 ; i < Motor1.page1_num ; i++ )
            {
                  ui->tableWidget->item(i,0)->setCheckState(Qt::Unchecked);
                  Motor1.branch_oneGroup[i].branch.choose = NO;

            }


        }

    }else if (Motor1.Group == 2)        //2组
    {
        if(ui->checkBox->isChecked())
        {
            CheckBox_num = Motor1.page2_num;        //选中个数为2组个数
            ui->label_Selected->setText(QString("已选数量：%1").arg(CheckBox_num));

            for (int i  = 0 ; i < Motor1.page2_num ; i++ )
            {
                 ui->tableWidget->item(i,0)->setCheckState(Qt::Checked);
                 Motor1.branch_twoGroup[i].branch.choose = YES;

            }


        }else
        {
            CheckBox_num = 0;
            ui->label_Selected->setText(QString("已选数量：%1").arg(CheckBox_num));

            for (int i  = 0 ; i < Motor1.page2_num ; i++ )
            {
                  ui->tableWidget->item(i,0)->setCheckState(Qt::Unchecked);
                  Motor1.branch_twoGroup[i].branch.choose = NO;

            }


        }

    }else if (Motor1.Group == 0)
    {
        if(ui->checkBox->isChecked())
        {
            CheckBox_num = Motor1.all_num;
            ui->label_Selected->setText(QString("已选数量：%1").arg(CheckBox_num));

            for (int k = 0 ;k <Motor1.all_num ; k++ )
            {
                 ui->tableWidget->item(k,0)->setCheckState(Qt::Checked);

            }
            for (int i  = 0 ; i < Motor1.page1_num ; i++ )
            {

                 Motor1.branch_oneGroup[i].branch.choose = YES;


            }
            for (int j = 0; j <Motor1.page2_num ;j++ )
            {
                  Motor1.branch_twoGroup[j].branch.choose = YES;

            }


        }else
        {
            CheckBox_num = 0;
            ui->label_Selected->setText(QString("已选数量：%1").arg(CheckBox_num));

            for (int k = 0 ;k <Motor1.all_num ; k++ )
            {
                 ui->tableWidget->item(k,0)->setCheckState(Qt::Unchecked);

            }
            for (int i  = 0 ; i < Motor1.page1_num ; i++ )
            {

                 Motor1.branch_oneGroup[i].branch.choose = NO;


            }
            for (int j = 0; j <Motor1.page2_num ;j++ )
            {
                  Motor1.branch_twoGroup[j].branch.choose = NO;

            }

        }


    }

}

void configWindow:: Config_Setting(Motor_dev &Motor_config ,QString GroupFile)
{

    //读写配置
    //删除原文件
//   QFile fileTemp(GroupFile);
//   fileTemp.remove();
   //写配置
   QFileInfo file(GroupFile);

   if(!file.isFile())       //判断文件如果不存在 ,就创建默认值文件
   {
     QSettings settings(GroupFile,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
   }
   if(Motor_config.Group == 1)
   {
       QSettings settings(GroupFile,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
       settings.beginGroup("number");
       settings.setValue("page1_num",Motor_config.page1_num);
       settings.endGroup();

       for (int i = 0 ;i < Motor_config.page1_num ;i++ )
       {
           settings.beginGroup(QString("%1").arg(i));

           settings.setValue("id",Motor_config.branch_oneGroup[i].branch.id);
           settings.setValue("only_id",Motor_config.branch_oneGroup[i].parameter.Only_ID);
           settings.setValue("load",Motor_config.branch_oneGroup[i].parameter.overload_value);
           settings.setValue("loss",Motor_config.branch_oneGroup[i].parameter.loss_value);
           settings.setValue("tight",Motor_config.branch_oneGroup[i].parameter.tighten_value);
           settings.setValue("loose",Motor_config.branch_oneGroup[i].parameter.loose_value);
           settings.setValue("current",Motor_config.branch_oneGroup[i].parameter.current_value);
           settings.endGroup();


       }


   }else if (Motor_config.Group == 2)
   {
       QSettings settings(GroupFile,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
       settings.beginGroup("number");
       settings.setValue("page2_num",Motor_config.page2_num);
       settings.endGroup();

      for (int i = 0 ;i < Motor_config.page2_num ;i++ )
      {
          settings.beginGroup(QString("%1").arg(i));
          settings.setValue("id",Motor_config.branch_twoGroup[i].branch.id);
          settings.setValue("load",Motor_config.branch_twoGroup[i].parameter.overload_value);
          settings.setValue("loss",Motor_config.branch_twoGroup[i].parameter.loss_value);
          settings.setValue("tight",Motor_config.branch_twoGroup[i].parameter.tighten_value);
          settings.setValue("loose",Motor_config.branch_twoGroup[i].parameter.loose_value);
          settings.setValue("current",Motor_config.branch_twoGroup[i].parameter.current_value);
          settings.endGroup();


      }


    }
//   QSettings settings(GroupFile,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
//   settings.beginGroup(ID);
//   settings.setValue("group","1");
//   settings.setValue("load","5.0");
//   settings.setValue("loss","3.0");
//   settings.setValue("tight","1.5");
//   settings.setValue("loose","0.2");
//   settings.setValue("current","3.2");
//   settings.endGroup();



//   QSettings settings(CONFIG_NAME,QSettings::IniFormat);   //当前路径直接写名字，绝对路径需要绝对路径
//  settings.beginGroup("2");
//  int i =  settings.contains("loose");        //  判断KEY是否存在，从而判断组是否存在
//   settings.endGroup();


//    //删除文件
//   QFile fileTemp(CONFIG_NAME);
//   fileTemp.remove();

//   contains() 判断一个指定的键是否存在
//   remove() 删除相关的键
//   allKeys() 获取所有键
//   clear() 删除所有键

}

void configWindow::tablewiget_SetData(int Group,Motor_dev Motor)
{
    //设置表格显示数据


        if(Group == 1)
        {
            ui->tableWidget->setRowCount(Motor.page1_num);  //设置显示行数
            for (int i = 0;i < Motor.page1_num ;i++ )
            {
                QTableWidgetItem *check = new QTableWidgetItem();
                check->setCheckState(Qt::Checked);
                check->setFlags(Qt::ItemFlags(check->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable));    //设置控件不可选中
                ui->tableWidget->setItem(i,0,check); //插入复选框

                QString data;

                QTableWidgetItem *item1 = new QTableWidgetItem("1");        //组号
                QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(Motor.branch_oneGroup[i].branch.id));        //  Id

                data = QString("%1.%2").arg(Motor.branch_oneGroup[i].parameter.overload_value/10).arg(Motor.branch_oneGroup[i].parameter.overload_value%10);    //增加小数点
                QTableWidgetItem *item3 = new QTableWidgetItem(data);        //超载值

                data = QString("%1.%2").arg(Motor.branch_oneGroup[i].parameter.loss_value/10).arg(Motor.branch_oneGroup[i].parameter.loss_value%10);    //增加小数点
                QTableWidgetItem *item4 = new QTableWidgetItem(data);         //失载值

                data = QString("%1.%2").arg(Motor.branch_oneGroup[i].parameter.tighten_value/10).arg(Motor.branch_oneGroup[i].parameter.tighten_value%10);    //增加小数点
                QTableWidgetItem *item5 = new QTableWidgetItem(data);                     //紧钩值

                data = QString("%1.%2").arg(Motor.branch_oneGroup[i].parameter.loose_value/10).arg(Motor.branch_oneGroup[i].parameter.loose_value%10);    //增加小数点
                QTableWidgetItem *item6 = new QTableWidgetItem(data);                     //松钩值

                data = QString("%1.%2").arg(Motor.branch_oneGroup[i].parameter.current_value/10).arg(Motor.branch_oneGroup[i].parameter.current_value%10);    //增加小数点
                QTableWidgetItem *item7 = new QTableWidgetItem(data);                   //电流值

                item1->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,1,item1);

                item2->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,2,item2);

                item3->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,3,item3);

                item4->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,4,item4);

                item5->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,5,item5);

                item6->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,6,item6);

                item7->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,7,item7);

            }

        }else if (Group == 2)
        {
            ui->tableWidget->setRowCount(Motor.page2_num);  //设置显示行数
            for (int i = 0;i < Motor.page2_num ;i++ )
            {
                QTableWidgetItem *check = new QTableWidgetItem();
                check->setCheckState(Qt::Checked);
                check->setFlags(Qt::ItemFlags(check->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable));    //设置控件不可选中
                ui->tableWidget->setItem(i,0,check); //插入复选框

                QString data;

                QTableWidgetItem *item1 = new QTableWidgetItem("2");        //组号
                QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(Motor.branch_twoGroup[i].branch.id));        //  Id

                data = QString("%1.%2").arg(Motor.branch_twoGroup[i].parameter.overload_value/10).arg(Motor.branch_twoGroup[i].parameter.overload_value%10);    //增加小数点
                QTableWidgetItem *item3 = new QTableWidgetItem(data);        //超载值

                data = QString("%1.%2").arg(Motor.branch_twoGroup[i].parameter.loss_value/10).arg(Motor.branch_twoGroup[i].parameter.loss_value%10);    //增加小数点
                QTableWidgetItem *item4 = new QTableWidgetItem(data);         //失载值

                data = QString("%1.%2").arg(Motor.branch_twoGroup[i].parameter.tighten_value/10).arg(Motor.branch_twoGroup[i].parameter.tighten_value%10);    //增加小数点
                QTableWidgetItem *item5 = new QTableWidgetItem(data);                     //紧钩值

                data = QString("%1.%2").arg(Motor.branch_twoGroup[i].parameter.loose_value/10).arg(Motor.branch_twoGroup[i].parameter.loose_value%10);    //增加小数点
                QTableWidgetItem *item6 = new QTableWidgetItem(data);                     //松钩值

                data = QString("%1.%2").arg(Motor.branch_twoGroup[i].parameter.current_value/10).arg(Motor.branch_twoGroup[i].parameter.current_value%10);    //增加小数点
                QTableWidgetItem *item7 = new QTableWidgetItem(data);                   //电流值

                item1->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,1,item1);

                item2->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,2,item2);

                item3->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,3,item3);

                item4->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,4,item4);

                item5->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,5,item5);

                item6->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,6,item6);

                item7->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,7,item7);

            }


        }else
        {
            ui->tableWidget->setRowCount(Motor.page1_num + Motor.page2_num );  //设置显示行数
            for (int i = 0;i < Motor.page1_num ;i++ )
            {
                QTableWidgetItem *check = new QTableWidgetItem();
                check->setCheckState(Qt::Checked);
                check->setFlags(Qt::ItemFlags(check->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable));    //设置控件不可选中
                ui->tableWidget->setItem(i,0,check); //插入复选框

                QString data = "";

                QTableWidgetItem *item1 = new QTableWidgetItem("1");        //组号
                QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(Motor.branch_oneGroup[i].branch.id));        //  Id

                data = QString("%1.%2").arg(Motor.branch_oneGroup[i].parameter.overload_value/10).arg(Motor.branch_oneGroup[i].parameter.overload_value%10);    //增加小数点
                QTableWidgetItem *item3 = new QTableWidgetItem(data);        //超载值

                data = QString("%1.%2").arg(Motor.branch_oneGroup[i].parameter.loss_value/10).arg(Motor.branch_oneGroup[i].parameter.loss_value%10);    //增加小数点
                QTableWidgetItem *item4 = new QTableWidgetItem(data);         //失载值

                data = QString("%1.%2").arg(Motor.branch_oneGroup[i].parameter.tighten_value/10).arg(Motor.branch_oneGroup[i].parameter.tighten_value%10);    //增加小数点
                QTableWidgetItem *item5 = new QTableWidgetItem(data);                     //紧钩值

                data = QString("%1.%2").arg(Motor.branch_oneGroup[i].parameter.loose_value/10).arg(Motor.branch_oneGroup[i].parameter.loose_value%10);    //增加小数点
                QTableWidgetItem *item6 = new QTableWidgetItem(data);                     //松钩值

                data = QString("%1.%2").arg(Motor.branch_oneGroup[i].parameter.current_value/10).arg(Motor.branch_oneGroup[i].parameter.current_value%10);    //增加小数点
                QTableWidgetItem *item7 = new QTableWidgetItem(data);                   //电流值

                item1->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,1,item1);

                item2->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,2,item2);

                item3->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,3,item3);

                item4->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,4,item4);

                item5->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,5,item5);

                item6->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,6,item6);

                item7->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,7,item7);

            }
            for (int i = Motor.page1_num;i < (Motor.page2_num + Motor.page1_num);i++ )
            {
                QTableWidgetItem *check = new QTableWidgetItem();
                check->setCheckState(Qt::Checked);
                check->setFlags(Qt::ItemFlags(check->flags() & ~Qt::ItemIsEnabled & ~Qt::ItemIsSelectable));    //设置控件不可选中
                ui->tableWidget->setItem(i,0,check); //插入复选框

                QString data = "";

                QTableWidgetItem *item1 = new QTableWidgetItem("2");        //组号
                QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(Motor.branch_twoGroup[i - Motor.page1_num].branch.id));        //  Id

                data = QString("%1.%2").arg(Motor.branch_twoGroup[i - Motor.page1_num].parameter.overload_value/10).arg(Motor.branch_twoGroup[i - Motor.page1_num].parameter.overload_value%10);    //增加小数点
                QTableWidgetItem *item3 = new QTableWidgetItem(data);        //超载值

                data = QString("%1.%2").arg(Motor.branch_twoGroup[i - Motor.page1_num].parameter.loss_value/10).arg(Motor.branch_twoGroup[i - Motor.page1_num].parameter.loss_value%10);    //增加小数点
                QTableWidgetItem *item4 = new QTableWidgetItem(data);         //失载值

                data = QString("%1.%2").arg(Motor.branch_twoGroup[i - Motor.page1_num].parameter.tighten_value/10).arg(Motor.branch_twoGroup[i - Motor.page1_num].parameter.tighten_value%10);    //增加小数点
                QTableWidgetItem *item5 = new QTableWidgetItem(data);                     //紧钩值

                data = QString("%1.%2").arg(Motor.branch_twoGroup[i - Motor.page1_num].parameter.loose_value/10).arg(Motor.branch_twoGroup[i - Motor.page1_num].parameter.loose_value%10);    //增加小数点
                QTableWidgetItem *item6 = new QTableWidgetItem(data);                     //松钩值

                data = QString("%1.%2").arg(Motor.branch_twoGroup[i - Motor.page1_num].parameter.current_value/10).arg(Motor.branch_twoGroup[i - Motor.page1_num].parameter.current_value%10);    //增加小数点
                QTableWidgetItem *item7 = new QTableWidgetItem(data);                   //电流值

                item1->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,1,item1);

                item2->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,2,item2);

                item3->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,3,item3);

                item4->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,4,item4);

                item5->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,5,item5);

                item6->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,6,item6);

                item7->setTextAlignment(Qt::AlignCenter);
                ui->tableWidget->setItem(i,7,item7);

            }



        }






//        ui->tableWidget->clear();
//        ui->tableWidget->clearContents();
//for(int row = ui->tableWidget->rowCount() - 1;row >= 0; row--)
//        {
//            ui->tableWidget->removeRow(row);
//        }


}

//void configWindow::Refarsh_Tablewiget(Motor_dev Motor_newdata)
//{
//    //刷新表格数据
//    if(Motor_newdata.Group == 1)
//    {
//        for (int i = 0;i < Motor_newdata.page1_num ;i++ )
//        {
//            QString data;

//            QTableWidgetItem *item1 = new QTableWidgetItem("1");        //组号
//            QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(Motor_newdata.branch_oneGroup[i].branch.id));        //  Id

//            data = QString("%1.%2").arg(Motor_newdata.branch_oneGroup[i].parameter.overload_value/10).arg(Motor_newdata.branch_oneGroup[i].parameter.overload_value%10);    //增加小数点
//            QTableWidgetItem *item3 = new QTableWidgetItem(data);        //超载值

//            data = QString("%1.%2").arg(Motor_newdata.branch_oneGroup[i].parameter.loss_value/10).arg(Motor_newdata.branch_oneGroup[i].parameter.loss_value%10);    //增加小数点
//            QTableWidgetItem *item4 = new QTableWidgetItem(data);         //失载值

//            data = QString("%1.%2").arg(Motor_newdata.branch_oneGroup[i].parameter.tighten_value/10).arg(Motor_newdata.branch_oneGroup[i].parameter.tighten_value%10);    //增加小数点
//            QTableWidgetItem *item5 = new QTableWidgetItem(data);                     //紧钩值

//            data = QString("%1.%2").arg(Motor_newdata.branch_oneGroup[i].parameter.loose_value/10).arg(Motor_newdata.branch_oneGroup[i].parameter.loose_value%10);    //增加小数点
//            QTableWidgetItem *item6 = new QTableWidgetItem(data);                     //松钩值

//            data = QString("%1.%2").arg(Motor_newdata.branch_oneGroup[i].parameter.current_value/10).arg(Motor_newdata.branch_oneGroup[i].parameter.current_value%10);    //增加小数点
//            QTableWidgetItem *item7 = new QTableWidgetItem(data);                   //电流值

//            item1->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,1,item1);

//            item2->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,2,item2);

//            item3->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,3,item3);

//            item4->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,4,item4);

//            item5->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,5,item5);

//            item6->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,6,item6);

//            item7->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,7,item7);

//        }


//    }else if (Motor_newdata.Group == 2)
//    {
//        for (int i = 0;i < Motor_newdata.page2_num ;i++ )
//        {


//            QString data;

//            QTableWidgetItem *item1 = new QTableWidgetItem("2");        //组号
//            QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(Motor_newdata.branch_twoGroup[i].branch.id));        //  Id

//            data = QString("%1.%2").arg(Motor_newdata.branch_twoGroup[i].parameter.overload_value/10).arg(Motor_newdata.branch_twoGroup[i].parameter.overload_value%10);    //增加小数点
//            QTableWidgetItem *item3 = new QTableWidgetItem(data);        //超载值

//            data = QString("%1.%2").arg(Motor_newdata.branch_twoGroup[i].parameter.loss_value/10).arg(Motor_newdata.branch_twoGroup[i].parameter.loss_value%10);    //增加小数点
//            QTableWidgetItem *item4 = new QTableWidgetItem(data);         //失载值

//            data = QString("%1.%2").arg(Motor_newdata.branch_twoGroup[i].parameter.tighten_value/10).arg(Motor_newdata.branch_twoGroup[i].parameter.tighten_value%10);    //增加小数点
//            QTableWidgetItem *item5 = new QTableWidgetItem(data);                     //紧钩值

//            data = QString("%1.%2").arg(Motor_newdata.branch_twoGroup[i].parameter.loose_value/10).arg(Motor_newdata.branch_twoGroup[i].parameter.loose_value%10);    //增加小数点
//            QTableWidgetItem *item6 = new QTableWidgetItem(data);                     //松钩值

//            data = QString("%1.%2").arg(Motor_newdata.branch_twoGroup[i].parameter.current_value/10).arg(Motor_newdata.branch_twoGroup[i].parameter.current_value%10);    //增加小数点
//            QTableWidgetItem *item7 = new QTableWidgetItem(data);                   //电流值

//            item1->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,1,item1);

//            item2->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,2,item2);

//            item3->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,3,item3);

//            item4->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,4,item4);

//            item5->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,5,item5);

//            item6->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,6,item6);

//            item7->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,7,item7);

//        }

//    }else if (Motor_newdata.Group == 0)
//    {
//        for (int i = 0;i < Motor_newdata.page1_num ;i++ )
//        {


//            QString data = "";

//            QTableWidgetItem *item1 = new QTableWidgetItem("1");        //组号
//            QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(Motor_newdata.branch_oneGroup[i].branch.id));        //  Id

//            data = QString("%1.%2").arg(Motor_newdata.branch_oneGroup[i].parameter.overload_value/10).arg(Motor_newdata.branch_oneGroup[i].parameter.overload_value%10);    //增加小数点
//            QTableWidgetItem *item3 = new QTableWidgetItem(data);        //超载值

//            data = QString("%1.%2").arg(Motor_newdata.branch_oneGroup[i].parameter.loss_value/10).arg(Motor_newdata.branch_oneGroup[i].parameter.loss_value%10);    //增加小数点
//            QTableWidgetItem *item4 = new QTableWidgetItem(data);         //失载值

//            data = QString("%1.%2").arg(Motor_newdata.branch_oneGroup[i].parameter.tighten_value/10).arg(Motor_newdata.branch_oneGroup[i].parameter.tighten_value%10);    //增加小数点
//            QTableWidgetItem *item5 = new QTableWidgetItem(data);                     //紧钩值

//            data = QString("%1.%2").arg(Motor_newdata.branch_oneGroup[i].parameter.loose_value/10).arg(Motor_newdata.branch_oneGroup[i].parameter.loose_value%10);    //增加小数点
//            QTableWidgetItem *item6 = new QTableWidgetItem(data);                     //松钩值

//            data = QString("%1.%2").arg(Motor_newdata.branch_oneGroup[i].parameter.current_value/10).arg(Motor_newdata.branch_oneGroup[i].parameter.current_value%10);    //增加小数点
//            QTableWidgetItem *item7 = new QTableWidgetItem(data);                   //电流值

//            item1->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,1,item1);

//            item2->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,2,item2);

//            item3->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,3,item3);

//            item4->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,4,item4);

//            item5->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,5,item5);

//            item6->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,6,item6);

//            item7->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,7,item7);

//        }
//        for (int i = Motor_newdata.page1_num;i < (Motor_newdata.page2_num + Motor_newdata.page1_num);i++ )
//        {


//            QString data = "";

//            QTableWidgetItem *item1 = new QTableWidgetItem("2");        //组号
//            QTableWidgetItem *item2 = new QTableWidgetItem(QString::number(Motor_newdata.branch_twoGroup[i - Motor_newdata.page1_num].branch.id));        //  Id

//            data = QString("%1.%2").arg(Motor_newdata.branch_twoGroup[i - Motor_newdata.page1_num].parameter.overload_value/10).arg(Motor_newdata.branch_twoGroup[i - Motor_newdata.page1_num].parameter.overload_value%10);    //增加小数点
//            QTableWidgetItem *item3 = new QTableWidgetItem(data);        //超载值

//            data = QString("%1.%2").arg(Motor_newdata.branch_twoGroup[i - Motor_newdata.page1_num].parameter.loss_value/10).arg(Motor_newdata.branch_twoGroup[i - Motor_newdata.page1_num].parameter.loss_value%10);    //增加小数点
//            QTableWidgetItem *item4 = new QTableWidgetItem(data);         //失载值

//            data = QString("%1.%2").arg(Motor_newdata.branch_twoGroup[i - Motor_newdata.page1_num].parameter.tighten_value/10).arg(Motor_newdata.branch_twoGroup[i - Motor_newdata.page1_num].parameter.tighten_value%10);    //增加小数点
//            QTableWidgetItem *item5 = new QTableWidgetItem(data);                     //紧钩值

//            data = QString("%1.%2").arg(Motor_newdata.branch_twoGroup[i - Motor_newdata.page1_num].parameter.loose_value/10).arg(Motor_newdata.branch_twoGroup[i - Motor_newdata.page1_num].parameter.loose_value%10);    //增加小数点
//            QTableWidgetItem *item6 = new QTableWidgetItem(data);                     //松钩值

//            data = QString("%1.%2").arg(Motor_newdata.branch_twoGroup[i - Motor_newdata.page1_num].parameter.current_value/10).arg(Motor_newdata.branch_twoGroup[i - Motor_newdata.page1_num].parameter.current_value%10);    //增加小数点
//            QTableWidgetItem *item7 = new QTableWidgetItem(data);                   //电流值

//            item1->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,1,item1);

//            item2->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,2,item2);

//            item3->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,3,item3);

//            item4->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,4,item4);

//            item5->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,5,item5);

//            item6->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,6,item6);

//            item7->setTextAlignment(Qt::AlignCenter);
//            ui->tableWidget->setItem(i,7,item7);

//        }


//    }

//}

bool configWindow::eventFilter(QObject *watch, QEvent *event)
{
    //获取鼠标点击事件
    //获取密码输入框焦点
    if((watch == ui->lineEdit_load) && (event->type() == QEvent::MouseButtonPress))
    {
      //  过载
        KeyValue.clear();
        key_point = 1;
       ui->widget_Key->show();
    }else if ((watch == ui->lineEdit_loss) && (event->type() == QEvent::MouseButtonPress))
    {
        //失载
           KeyValue.clear();
          key_point = 2;
         ui->widget_Key->show();


    }else if ((watch == ui->lineEdit_tight) && (event->type() == QEvent::MouseButtonPress))
    {
        //紧钩
         KeyValue.clear();
          key_point = 3;
         ui->widget_Key->show();


    }else if ((watch == ui->lineEdit_loose) && (event->type() == QEvent::MouseButtonPress))
    {
        //松钩
          KeyValue.clear();
          key_point = 4;
         ui->widget_Key->show();


    }else if ((watch == ui->lineEdit_current) && (event->type() == QEvent::MouseButtonPress))
    {
        //电流
         KeyValue.clear();
         key_point = 5;
         ui->widget_Key->show();


    }


    return false;

}

//键盘相关
void configWindow::on_pushButton_NO_clicked()
{
    //取消
    KeyValue.clear();
    switch (key_point)
    {
      case 1:ui->lineEdit_load->clear() ; break;      //  "过载"
      case 2:ui->lineEdit_loss->clear() ; break;      //  "失载"
      case 3:ui->lineEdit_tight->clear() ; break;     //  "紧钩"
      case 4:ui->lineEdit_loose->clear() ; break;     // "松钩"
      case 5:ui->lineEdit_current->clear() ; break;   //  "电流"
      default:break;

    }

 //   ui->widget_Key->hide();



}


void configWindow::on_pushButton_del_clicked()
{
    //删除
    if(!KeyValue.isEmpty())
    {
      KeyValue = KeyValue.left(KeyValue.size() -1);

      switch (key_point)
      {
        case 1:ui->lineEdit_load->setText(KeyValue) ; break;      //  "过载"
        case 2:ui->lineEdit_loss->setText(KeyValue) ; break;      //  "失载"
        case 3:ui->lineEdit_tight->setText(KeyValue) ; break;      //  "紧钩"
        case 4:ui->lineEdit_loose->setText(KeyValue) ; break;      // "松钩"
        case 5:ui->lineEdit_current->setText(KeyValue) ; break;      //  "电流"
        default:break;

      }

    }


}


void configWindow::on_pushButton_ok_clicked()
{
    KeyValue.clear();
    ui->widget_Key->hide();


}


void configWindow::on_pushButton_point_clicked()
{
    KeyValue += ".";
    switch (key_point)
    {
      case 1:ui->lineEdit_load->setText(KeyValue) ; break;      //  "过载"
      case 2:ui->lineEdit_loss->setText(KeyValue) ; break;      //  "失载"
      case 3:ui->lineEdit_tight->setText(KeyValue) ; break;     //  "紧钩"
      case 4:ui->lineEdit_loose->setText(KeyValue) ; break;     //  "松钩"
      case 5:ui->lineEdit_current->setText(KeyValue) ; break;   //  "电流"
      default:break;

    }


}


void configWindow::on_pushButton_00_clicked()
{
      KeyValue += "0";
      switch (key_point)
      {
        case 1:ui->lineEdit_load->setText(KeyValue) ; break;      //  "过载"
        case 2:ui->lineEdit_loss->setText(KeyValue) ; break;      //  "失载"
        case 3:ui->lineEdit_tight->setText(KeyValue) ; break;      //  "紧钩"
        case 4:ui->lineEdit_loose->setText(KeyValue) ; break;      // "松钩"
        case 5:ui->lineEdit_current->setText(KeyValue) ; break;      //  "电流"
        default:break;

      }


}



void configWindow::on_pushButton_01_clicked()
{
     KeyValue += "1";
     switch (key_point)
     {
       case 1:ui->lineEdit_load->setText(KeyValue) ; break;      //  "过载"
       case 2:ui->lineEdit_loss->setText(KeyValue) ; break;      //  "失载"
       case 3:ui->lineEdit_tight->setText(KeyValue) ; break;      //  "紧钩"
       case 4:ui->lineEdit_loose->setText(KeyValue) ; break;      // "松钩"
       case 5:ui->lineEdit_current->setText(KeyValue) ; break;      //  "电流"
       default:break;

     }


}

void configWindow::on_pushButton_02_clicked()
{
     KeyValue += "2";
     switch (key_point)
     {
       case 1:ui->lineEdit_load->setText(KeyValue) ; break;      //  "过载"
       case 2:ui->lineEdit_loss->setText(KeyValue) ; break;      //  "失载"
       case 3:ui->lineEdit_tight->setText(KeyValue) ; break;      //  "紧钩"
       case 4:ui->lineEdit_loose->setText(KeyValue) ; break;      // "松钩"
       case 5:ui->lineEdit_current->setText(KeyValue) ; break;      //  "电流"
       default:break;

     }


}

void configWindow::on_pushButton_03_clicked()
{
     KeyValue += "3";
     switch (key_point)
     {
       case 1:ui->lineEdit_load->setText(KeyValue) ; break;      //  "过载"
       case 2:ui->lineEdit_loss->setText(KeyValue) ; break;      //  "失载"
       case 3:ui->lineEdit_tight->setText(KeyValue) ; break;      //  "紧钩"
       case 4:ui->lineEdit_loose->setText(KeyValue) ; break;      // "松钩"
       case 5:ui->lineEdit_current->setText(KeyValue) ; break;      //  "电流"
       default:break;

     }


}

void configWindow::on_pushButton_04_clicked()
{
     KeyValue += "4";
     switch (key_point)
     {
       case 1:ui->lineEdit_load->setText(KeyValue) ; break;      //  "过载"
       case 2:ui->lineEdit_loss->setText(KeyValue) ; break;      //  "失载"
       case 3:ui->lineEdit_tight->setText(KeyValue) ; break;      //  "紧钩"
       case 4:ui->lineEdit_loose->setText(KeyValue) ; break;      // "松钩"
       case 5:ui->lineEdit_current->setText(KeyValue) ; break;      //  "电流"
       default:break;

     }


}

void configWindow::on_pushButton_05_clicked()
{
     KeyValue += "5";
     switch (key_point)
     {
       case 1:ui->lineEdit_load->setText(KeyValue) ; break;      //  "过载"
       case 2:ui->lineEdit_loss->setText(KeyValue) ; break;      //  "失载"
       case 3:ui->lineEdit_tight->setText(KeyValue) ; break;      //  "紧钩"
       case 4:ui->lineEdit_loose->setText(KeyValue) ; break;      // "松钩"
       case 5:ui->lineEdit_current->setText(KeyValue) ; break;      //  "电流"
       default:break;

     }


}

void configWindow::on_pushButton_06_clicked()
{
     KeyValue += "6";
     switch (key_point)
     {
       case 1:ui->lineEdit_load->setText(KeyValue) ; break;      //  "过载"
       case 2:ui->lineEdit_loss->setText(KeyValue) ; break;      //  "失载"
       case 3:ui->lineEdit_tight->setText(KeyValue) ; break;      //  "紧钩"
       case 4:ui->lineEdit_loose->setText(KeyValue) ; break;      // "松钩"
       case 5:ui->lineEdit_current->setText(KeyValue) ; break;      //  "电流"
       default:break;

     }


}

void configWindow::on_pushButton_07_clicked()
{
     KeyValue += "7";
     switch (key_point)
     {
       case 1:ui->lineEdit_load->setText(KeyValue) ; break;      //  "过载"
       case 2:ui->lineEdit_loss->setText(KeyValue) ; break;      //  "失载"
       case 3:ui->lineEdit_tight->setText(KeyValue) ; break;      //  "紧钩"
       case 4:ui->lineEdit_loose->setText(KeyValue) ; break;      // "松钩"
       case 5:ui->lineEdit_current->setText(KeyValue) ; break;      //  "电流"
       default:break;

     }


}


void configWindow::on_pushButton_08_clicked()
{
     KeyValue += "8";
     switch (key_point)
     {
       case 1:ui->lineEdit_load->setText(KeyValue) ; break;      //  "过载"
       case 2:ui->lineEdit_loss->setText(KeyValue) ; break;      //  "失载"
       case 3:ui->lineEdit_tight->setText(KeyValue) ; break;      //  "紧钩"
       case 4:ui->lineEdit_loose->setText(KeyValue) ; break;      // "松钩"
       case 5:ui->lineEdit_current->setText(KeyValue) ; break;      //  "电流"
       default:break;

     }


}


void configWindow::on_pushButton_09_clicked()
{
     KeyValue += "9";
     switch (key_point)
     {
       case 1:ui->lineEdit_load->setText(KeyValue) ; break;      //  "过载"
       case 2:ui->lineEdit_loss->setText(KeyValue) ; break;      //  "失载"
       case 3:ui->lineEdit_tight->setText(KeyValue) ; break;      //  "紧钩"
       case 4:ui->lineEdit_loose->setText(KeyValue) ; break;      // "松钩"
       case 5:ui->lineEdit_current->setText(KeyValue) ; break;      //  "电流"
       default:break;

     }

}



void configWindow::on_pushButton_Scan_clicked()
{
    //分控扫描按键

    if(Motor1.Motor_state != Scan && Motor1.Motor_state != Set)
    {
        //清空状态
        QString Sty = "QLabel{background-color: rgb(165, 165, 165);color: rgb(255, 255, 255);}";
        for (int i =1; i < 151 ;i++ )
        {
            label_NumSet(i,Sty);

        }
        ui->pushButton_ScanOrStop->show();
        ui->pushButton_ScanCancel->hide();
        ui->pushButton_ScanSave->hide();

        QString num = QString("扫描分控数量：%1/%2").arg(Motor1.all_num).arg(ALL_Num);
        ui->label_ScanNum->setText(num);
        ui->label_SCANLabel->setText("分控扫描中....");

        ui->widget_SCAN->show();       //显示控件
        Motor1.Motor_state = Scan;      //  扫描
        ID_location = 1;  //从机位号1开始扫描
        Motor1.Group = 0;
        timer4->start(100);

    }




    //background-color: rgb(18, 99, 139);


}


void configWindow::on_pushButton_ScanOrStop_clicked()
{
    //分控扫描中停止按键
    Motor1.Motor_state = Wait;
  //  Motor1.Motor_state = Stop;
    Motor1_new  = Motor_clear;     //清空
    ui->widget_SCAN->hide();



}


void configWindow::on_pushButton_ScanSave_clicked()
{
    //保存扫描结果

      //复制旧状态
    Motor1_new.Remote_lock = Motor1.Remote_lock;
    Motor1_new.login = Motor1.login;
    Motor1_new.power = Motor1.power;
    Motor1_new.Motor_state = Motor1.Motor_state;
    Motor1 = Motor1_new;            //  保存数据
    Motor1_new  = Motor_clear;     //清空
    //刷新表格
    ui->tableWidget->clearContents();
    Motor1.Motor_state = Wait;
   // Motor1.Motor_state = Stop;
    Motor1.Group = 0;   //全选
   tablewiget_SetData( Motor1.Group,Motor1);  //显示全部参数

    //先删除源文件，再依次保存
    //删除原文件
    Motor1.Group =1;
    QFile fileOne(CONFIG_oneGroupNAME);
    fileOne.remove();
    Config_Setting(Motor1,CONFIG_oneGroupNAME);
     Motor1.Group =2;
    QFile fileTwo(CONFIG_twoGroupNAME);
    fileTwo.remove();
    Config_Setting(Motor1,CONFIG_twoGroupNAME);

    //更新按键里面单控个数
    ui->pushButton_allGroup->setText(QString("全部(%1)").arg(Motor1.all_num));
    ui->pushButton_oneGroup->setText(QString("1组(%1)").arg(Motor1.page1_num));
    ui->pushButton_twoGroup->setText(QString("2组(%1)").arg(Motor1.page2_num));
    //g更新选中个数
     CheckBox_num =Motor1.all_num;                //选中个数为总数
     ui->label_Selected->setText(QString("已选数量：%1").arg(CheckBox_num));   //设置已选数量
     ui->widget_SCAN->hide();

}


void configWindow::on_pushButton_ScanCancel_clicked()
{
    //取消扫描结果  不保存
     Motor1_new  = Motor_clear;     //清空
    ui->widget_SCAN->hide();


}


void configWindow::on_pushButton_allGroup_clicked()
{
    //点击全部组
    //清除表格数据
    ui->tableWidget->clearContents();
    ui->checkBox->setCheckState(Qt::Checked);
    CheckBox_num =Motor1.all_num;                //选中个数为总数
    ui->label_Selected->setText(QString("已选数量：%1").arg(CheckBox_num));   //设置已选数量
    if(Motor1.all_num == 0)
    {
        ui->label_Nomotor->show();
    }else
    {
        ui->label_Nomotor->hide();

    }

    for (int i = 0; i  < Motor1.page1_num ;i++ )    //设置1组状态为选中
    {

        Motor1.branch_oneGroup[i].branch.choose = true;
    }
    for (int i = 0; i  < Motor1.page1_num ;i++ )    //设置2组状态为选中
    {

        Motor1.branch_twoGroup[i].branch.choose = true;
    }

    Motor1.Group = 0;   //全选
   tablewiget_SetData( Motor1.Group,Motor1);  //显示全部参数


   //设置按钮颜色
   ui->pushButton_allGroup->setStyleSheet("QPushButton{color: rgb(255, 255, 255);background-color: rgb(9, 82, 120);}");  //选中
   ui->pushButton_oneGroup->setStyleSheet("QPushButton{color: rgb(0, 0, 0);background-color: rgb(200, 200, 200);}");  //未选中
   ui->pushButton_twoGroup->setStyleSheet("QPushButton{color: rgb(0, 0, 0);background-color: rgb(200, 200, 200);}");  //未选中

}


void configWindow::on_pushButton_oneGroup_clicked()
{
    //点击显示1组数据
    //清除表格数据
    ui->tableWidget->clearContents();
    ui->checkBox->setCheckState(Qt::Checked);
    CheckBox_num = Motor1.page1_num;                //选中个数为1组
    ui->label_Selected->setText(QString("已选数量：%1").arg(CheckBox_num));   //设置已选数量

    if(Motor1.page1_num == 0)
    {
        ui->label_Nomotor->show();
    }else
    {
        ui->label_Nomotor->hide();

    }
    for (int i = 0; i  < Motor1.page1_num ;i++ )    //设置1组状态为选中
    {

        Motor1.branch_oneGroup[i].branch.choose = true;
    }

     Motor1.Group = 1;   //1组

    tablewiget_SetData(Motor1.Group,Motor1);  //显示1组参数

    //设置按钮颜色
    ui->pushButton_allGroup->setStyleSheet("QPushButton{color: rgb(0, 0, 0);background-color: rgb(200, 200, 200);}");  //选中
    ui->pushButton_oneGroup->setStyleSheet("QPushButton{color: rgb(255, 255, 255);background-color: rgb(9, 82, 120);}");  //未选中
    ui->pushButton_twoGroup->setStyleSheet("QPushButton{color: rgb(0, 0, 0);background-color: rgb(200, 200, 200);}");  //未选中

}


void configWindow::on_pushButton_twoGroup_clicked()
{
    //点击2组数据

    //清除表格数据
    ui->tableWidget->clearContents();
    ui->checkBox->setCheckState(Qt::Checked);
    CheckBox_num = Motor1.page2_num;                //选中个数为2组
    ui->label_Selected->setText(QString("已选数量：%1").arg(CheckBox_num));   //设置已选数量
    if(Motor1.page2_num == 0)
    {
        ui->label_Nomotor->show();
    }else
    {
        ui->label_Nomotor->hide();

    }

    for (int i = 0; i  < Motor1.page1_num ;i++ )    //设置2组状态为选中
    {

        Motor1.branch_twoGroup[i].branch.choose = true;
    }

    Motor1.Group = 2;   //2组
    tablewiget_SetData(Motor1.Group,Motor1);  //显示2组参数

    //设置按钮颜色
    ui->pushButton_allGroup->setStyleSheet("QPushButton{color: rgb(0, 0, 0);background-color: rgb(200, 200, 200);}");  //选中
    ui->pushButton_oneGroup->setStyleSheet("QPushButton{color: rgb(0, 0, 0);background-color:rgb(200, 200, 200);}");  //未选中
    ui->pushButton_twoGroup->setStyleSheet("QPushButton{color: rgb(255, 255, 255);background-color: rgb(9, 82, 120);}");  //未选中


}




void configWindow::on_pushButton_control_clicked()
{
    //分控操作页面
    if(Motor1.Motor_state != Scan && Motor1.Motor_state != Set)
    {

        oneWindow *one = new oneWindow;
        one->show();
        this->close();
    }else
    {
        qDebug() << "请先关闭扫描";

     }

}


void configWindow::on_pushButton_remote_clicked()
{
    //遥控设置
    if(Motor1.Motor_state != Scan && Motor1.Motor_state != Set)
    {

        remoteSet *remote = new remoteSet;
        remote->show();
        this->close();
    }else
    {
        qDebug() << "请先关闭扫描";

     }

}


void configWindow::on_pushButton_project_clicked()
{
    //项目信息
    if(Motor1.Motor_state != Scan && Motor1.Motor_state != Set)
    {

        Project_information  *project = new Project_information;
        project->show();
        this->close();
    }else
    {
        qDebug() << "请先关闭扫描";

     }

}


void configWindow::on_pushButton_system_clicked()
{
    //系统信息
    if(Motor1.Motor_state != Scan && Motor1.Motor_state != Set)
    {

        Syetem_information *system = new Syetem_information;
        system->show();
        this->close();
    }else
    {
        qDebug() << "请先关闭扫描";

     }

}


void configWindow::on_pushButton_dairy_clicked()
{
    //操作日志
    if(Motor1.Motor_state != Scan && Motor1.Motor_state != Set)
    {

        DiaryWindow  *diary = new DiaryWindow;
        diary->show();
        this->close();
    }else
    {
        qDebug() << "请先关闭扫描";

     }

}

