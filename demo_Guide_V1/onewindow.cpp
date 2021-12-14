 #include "onewindow.h"
#include "ui_onewindow.h"

oneWindow::oneWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::oneWindow)
{
    ui->setupUi(this);
//    ui->oneTeamButton->setEnabled(false);
//    ui->oneTeamButton->setStyleSheet("QPushButton{background-color: rgb(240, 240, 240);}");
    RounBar_set();
//    this->setObjectName("onewindow");
    this->setStyleSheet("#oneWindow{background-image: url(:/back/Image/button/4.png);}");      //:/back/Image/2.png
    pushButton_Set();   //  设置成圆形

     //初始化
    ui->widget_WarningOne->hide();       //故障提示框隐藏
    Motor1.Motor_state = Stop;          //初始化为停机状态
    Motor_OldState = Motor1.Motor_state;       //
    QString button_num = QString("      %1(1组)").arg(Motor1.page1_num);
    ui->oneTeamButton->setText(button_num);
    button_num = QString("      %1(2组)").arg(Motor1.page2_num);
    ui->twoTeamButton->setText(button_num);
    Motor1.Group = 1;            //初始化为一组
    for (int i  = 0 ; i < Motor1.page1_num ; i++ )
    {
         Motor1.branch_oneGroup[i].branch.choose = YES;

    }
    for (int j = 0; j <Motor1.page2_num ;j++ )
    {
          Motor1.branch_twoGroup[j].branch.choose = YES;
    }

    Icon_hide(Motor1.page1_num);  //初始化为1组
    ID_IconSet(Motor1);
    ui->label_motorNumber->setText(QString("分控数量为:%1").arg(Motor1.all_num));
    ui->oneTeamButton->setText(QString("%1(1组)").arg(Motor1.page1_num));
    ui->twoTeamButton->setText(QString("%1(2组)").arg(Motor1.page2_num));



    //定时器
    timer1 = new QTimer(this);   //定时更新
    connect(timer1,SIGNAL(timeout()),this,SLOT(TimeOut1()));


    timer2 = new QTimer(this);   //定时更新
    connect(timer2,SIGNAL(timeout()),this,SLOT(TimeOut2()));
    timer2->start(1000);      //一秒中断一次

    timer3 = new QTimer(this);   //定时更新
    connect(timer3,SIGNAL(timeout()),this,SLOT(TimeOut3()));





    //设置显示编号
//    array[0] = 0x01;
//    array[1] = 0x02 ;
//    ID_IconSet(array.length(),array);

    //更新数据

//     Set_ProgressBarData(5,49);   //设置显示数据
//     Set_ProgressBarState(5,Overload);


     //停机按键重装点击事件
     ui->RounBar->installEventFilter(this);
  //   connect(this,&oneWindow::updata_fale,this,&oneWindow::Update_data);


}

oneWindow::~oneWindow()
{
    timer1->stop();
    timer2->stop();
    delete timer3;
    delete timer2;
    delete timer1;
    delete ui;
}

void oneWindow::TimeOut1()
{
    //定时1回调  更新分控数据
    Update_data();  //数据处理

}

void oneWindow::TimeOut2()
{
    //定时2回调
    if(time_mark == true)               //计算爬升高度
    {
        // 爬架升降速度  单位  速度 2mm/s 更新上升高度
        //进度条数值计算公式  data =   X/（K *10） ; X = 2 * 秒 ； K是总高度
         time_fale++;
         Roun_Hight = 2*time_fale/(4*10);

         if(Roun_Hight >=100)
         {
             time_mark = false;
             Roun_Hight = 100;
             time_fale = 100*(4*10)/2;
         }

         QString cmd = QString("停机\n%1%").arg(Roun_Hight);

         ui->RounBar->setValue(Roun_Hight);
          ui->RounBar->setFormat(cmd);
    }

//    if(Motor_OldState != Motor1.Motor_state && Motor1.Motor_state != Wait)        //由于状态改变导致停机
//    {
//         qDebug() <<  Motor_OldState  << Motor1.Motor_state;
//        time_mark = false;    //上升停止计时

//        Motor1.Motor_state = Stop;       //  改变状态为停机
//        Motor_OldState = Motor1.Motor_state;       //
//         ID_location = 0;              //下标标志位复位
//        ui->pushButton_rising->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/113.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
//                                                        "QPushButton:hover{background-image: url(:/back/Image/button/113.png); color: black;}"
//                                                        "QPushButton:pressed{background-image: url(:/back/Image/button/846.png);border-style: inset;}");

//        ui->pushButton_tight->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/111.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
//                                                        "QPushButton:hover{background-image: url(:/back/Image/button/111.png); color: black;}"
//                                                        "QPushButton:pressed{background-image: url(:/back/Image/button/845.png);border-style: inset;}");

//        ui->pushButton_Loose  ->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/112.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
//                                                        "QPushButton:hover{background-image: url(:/back/Image/button/112.png); color: black;}"
//                                                        "QPushButton:pressed{background-image: url(:/back/Image/button/847.png);border-style: inset;}");
//        qDebug() << "异常停机";

//    }
     timer1->start(20);  //50ms中断一次
      ui->label_time->setText(now_data);      //更新时间

      //更新电流电压总数
      if(Motor1.Group == 1)
      {
          int allload = 0;
          int allcurrent = 0;
          for (int i = 0; i < Motor1.page1_num ;i++ )
          {
              allload += Motor1.branch_oneGroup[i].branch.weight;
              allcurrent += Motor1.branch_oneGroup[i].branch.current;
          }

          ui->label_allLoad->setText(QString("总负荷：%1.%2T").arg(allload/10).arg(allload%10));
          ui->label_allCurrent->setText(QString("总电流：%1.%2A").arg(allcurrent/10).arg(allcurrent%10));


      }else if (Motor1.Group == 2)
      {
          int allload = 0;
          int allcurrent = 0;
          for (int i = 0; i < Motor1.page2_num ;i++ )
          {
              allload += Motor1.branch_twoGroup[i].branch.weight;
              allcurrent += Motor1.branch_twoGroup[i].branch.current;
          }

          ui->label_allLoad->setText(QString("总负荷：%1.%2T").arg(allload/10).arg(allload%10));
          ui->label_allCurrent->setText(QString("总电流：%1.%2A").arg(allcurrent/10).arg(allcurrent%10));

       }

}

void oneWindow::TimeOut3()
{
    //定时器3回调函数
    if(Motor1.Motor_state == Tight)      //紧钩结束条件判断
    {
        if(Motor1.Group == 1)
        {
            for (int i =0; i < Motor1.page1_num ;i++ )
            {
                if(Motor1.branch_oneGroup[i].branch.choose == YES)  //选择的
                {
                   if(Motor1.branch_oneGroup[i].branch.state != Stop)
                   {
                       return;          //紧钩未完成
                   }
                }

            }

            //紧钩完成
            timer3->stop();
            Motor1.Motor_state = Stop;       //  改变状态为停机
            Motor_OldState = Motor1.Motor_state;       //
            ID_location = 0;              //下标标志位复位
            ui->pushButton_rising->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/113.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                            "QPushButton:hover{background-image: url(:/back/Image/button/113.png); color: black;}"
                                                            "QPushButton:pressed{background-image: url(:/back/Image/button/846.png);border-style: inset;}");

            ui->pushButton_tight->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/111.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                            "QPushButton:hover{background-image: url(:/back/Image/button/111.png); color: black;}"
                                                            "QPushButton:pressed{background-image: url(:/back/Image/button/845.png);border-style: inset;}");

            ui->pushButton_Loose  ->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/112.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                            "QPushButton:hover{background-image: url(:/back/Image/button/112.png); color: black;}"
                                                            "QPushButton:pressed{background-image: url(:/back/Image/button/847.png);border-style: inset;}");
            qDebug() << "紧钩完成";

        }else if (Motor1.Group == 2)                //2组
        {
            for (int i =0; i < Motor1.page2_num ;i++ )
            {
                if(Motor1.branch_twoGroup[i].branch.choose == YES)  //选择的
                {
                   if(Motor1.branch_twoGroup[i].branch.state != Stop)
                   {
                       return;          //紧钩未完成
                   }
                }

            }

            //紧钩完成
            timer3->stop();
            Motor1.Motor_state = Stop;       //  改变状态为停机
            Motor_OldState = Motor1.Motor_state;       //
            ID_location = 0;              //下标标志位复位
            ui->pushButton_rising->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/113.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                            "QPushButton:hover{background-image: url(:/back/Image/button/113.png); color: black;}"
                                                            "QPushButton:pressed{background-image: url(:/back/Image/button/846.png);border-style: inset;}");

            ui->pushButton_tight->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/111.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                            "QPushButton:hover{background-image: url(:/back/Image/button/111.png); color: black;}"
                                                            "QPushButton:pressed{background-image: url(:/back/Image/button/845.png);border-style: inset;}");

            ui->pushButton_Loose  ->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/112.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                            "QPushButton:hover{background-image: url(:/back/Image/button/112.png); color: black;}"
                                                            "QPushButton:pressed{background-image: url(:/back/Image/button/847.png);border-style: inset;}");
            qDebug() << "紧钩完成";

        }

    }else if (Motor1.Motor_state == Loose)      //松钩结束判断
    {
        if(Motor1.Group == 1)
        {
            for (int i =0; i < Motor1.page1_num ;i++ )
            {
                if(Motor1.branch_oneGroup[i].branch.choose == YES)  //选择的
                {
                   if(Motor1.branch_oneGroup[i].branch.state != Stop)
                   {
                       return;          //紧钩未完成
                   }
                }

            }

            //松钩完成
            timer3->stop();
            Motor1.Motor_state = Stop;       //  改变状态为停机
            Motor_OldState = Motor1.Motor_state;       //
            ID_location = 0;              //下标标志位复位
            ui->pushButton_rising->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/113.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                            "QPushButton:hover{background-image: url(:/back/Image/button/113.png); color: black;}"
                                                            "QPushButton:pressed{background-image: url(:/back/Image/button/846.png);border-style: inset;}");

            ui->pushButton_tight->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/111.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                            "QPushButton:hover{background-image: url(:/back/Image/button/111.png); color: black;}"
                                                            "QPushButton:pressed{background-image: url(:/back/Image/button/845.png);border-style: inset;}");

            ui->pushButton_Loose  ->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/112.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                            "QPushButton:hover{background-image: url(:/back/Image/button/112.png); color: black;}"
                                                            "QPushButton:pressed{background-image: url(:/back/Image/button/847.png);border-style: inset;}");
            qDebug() << "松钩完成";

    }else if (Motor1.Group == 2)                //2组
       {
            for (int i =0; i < Motor1.page2_num ;i++ )
            {
                if(Motor1.branch_twoGroup[i].branch.choose == YES)  //选择的
                {
                   if(Motor1.branch_twoGroup[i].branch.state != Stop)
                   {
                       return;          //紧钩未完成
                   }
                }

            }

            //松钩完成
            timer3->stop();
            Motor1.Motor_state = Stop;       //  改变状态为停机
            Motor_OldState = Motor1.Motor_state;       //
            ID_location = 0;              //下标标志位复位
            ui->pushButton_rising->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/113.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                            "QPushButton:hover{background-image: url(:/back/Image/button/113.png); color: black;}"
                                                            "QPushButton:pressed{background-image: url(:/back/Image/button/846.png);border-style: inset;}");

            ui->pushButton_tight->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/111.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                            "QPushButton:hover{background-image: url(:/back/Image/button/111.png); color: black;}"
                                                            "QPushButton:pressed{background-image: url(:/back/Image/button/845.png);border-style: inset;}");

            ui->pushButton_Loose  ->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/112.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                            "QPushButton:hover{background-image: url(:/back/Image/button/112.png); color: black;}"
                                                            "QPushButton:pressed{background-image: url(:/back/Image/button/847.png);border-style: inset;}");
            qDebug() << "松钩完成";


       }


}

}

bool oneWindow::eventFilter(QObject *watch, QEvent *event)
{
    //获取重装的点击事件
    //获取密码输入框焦点
    if((watch == ui->RounBar) && (event->type() == QEvent::MouseButtonPress))
    {
       time_mark = false;    //上升停止计时
       Motor1.Motor_state = Stop;       //  改变状态为停机
       //改变停机按钮
       RounBar_set();
       Motor_OldState = Motor1.Motor_state;       //
        ID_location = 0;              //下标标志位复位
       ui->pushButton_rising->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/113.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                       "QPushButton:hover{background-image: url(:/back/Image/button/113.png); color: black;}"
                                                       "QPushButton:pressed{background-image: url(:/back/Image/button/846.png);border-style: inset;}");

       ui->pushButton_tight->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/111.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                       "QPushButton:hover{background-image: url(:/back/Image/button/111.png); color: black;}"
                                                       "QPushButton:pressed{background-image: url(:/back/Image/button/845.png);border-style: inset;}");

       ui->pushButton_Loose  ->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/112.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                       "QPushButton:hover{background-image: url(:/back/Image/button/112.png); color: black;}"
                                                       "QPushButton:pressed{background-image: url(:/back/Image/button/847.png);border-style: inset;}");
       qDebug() << "停机";

    }
    return false;

}





void oneWindow::RounBar_set()
{
    //停机按钮
    //外部库   设置停机按钮
    ui->RounBar->setFormat("停机\n0%");
    ui->RounBar->setMaximum(100);
    ui->RounBar->setMinimum(0);
    ui->RounBar->setDecimals(0);
    ui->RounBar->setValue(0);

   // ui->RounBar->setOutlinePenWidth(18);
   // ui->RounBar->setDataPenWidth(10);
    QPalette paltte;
    if(Motor1.Motor_state == Stop)
    {
         paltte.setBrush(QPalette::AlternateBase,QColor(220,220,220));
    }else
    {
        paltte.setBrush(QPalette::AlternateBase,QColor(250,0,0));

    }

    paltte.setColor(QPalette::Text,Qt::white);   //字体色
    paltte.setColor(QPalette::Window, QColor(10, 111, 165));  //背景色
    paltte.setColor(QPalette::Shadow,Qt::white);  //未填充圆环边框色
    paltte.setColor(QPalette::Base,Qt::white);  //未填充圆环边框色

 //   paltte.setColor(QPalette::Shadow,Qt::black);  //未填充圆环边框色
    paltte.setColor(QPalette::Base,Qt::white);  //未填充圆环边框色



    ui->RounBar->setPalette(paltte);
    QGradientStops gradientPoints;
    gradientPoints << QGradientStop(0,QColor(254,170,0) ) << QGradientStop(0.5,QColor(254,170,0)) << QGradientStop(1, QColor(254,170,0)); //255, 170, 0
    // and set it
    ui->RounBar->setDataColors(gradientPoints);



}

void oneWindow::pushButton_Set()
{
    //设置标号按钮
    ui->pushButton_01->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"  //
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"  //按键本色
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_02->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"  //停放时候色彩
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");    //按下时候色彩
    ui->pushButton_03->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_04->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255); color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_05->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_06->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_07->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_08->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_09->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255); color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_10->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_11->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_12->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_13->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_14->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255); color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_15->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_16->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_17->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_18->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_19->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_20->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_21->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_22->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_23->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255); color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_24->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255); color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_25->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_26->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_27->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255); color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_28->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255); color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_29->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255); color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_30->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255); color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_31->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255); color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_32->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255); color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_33->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_34->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_35->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255); color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_36->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_37->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_38->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255); color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_39->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");
    ui->pushButton_40->setStyleSheet("QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"
                                           "QPushButton:hover{background-color: rgb(85, 170, 255); color: rgb(255, 255, 255);}"
                                           "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}");

    //设置按钮
    ui->pushButton_rising->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/113.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                    "QPushButton:hover{background-image: url(:/back/Image/button/113.png); color: black;}"
                                                    "QPushButton:pressed{background-image: url(:/back/Image/button/846.png);border-style: inset;}");

    ui->pushButton_tight->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/111.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                    "QPushButton:hover{background-image: url(:/back/Image/button/111.png); color: black;}"
                                                    "QPushButton:pressed{background-image: url(:/back/Image/button/845.png);border-style: inset;}");

    ui->pushButton_Loose  ->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/112.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                    "QPushButton:hover{background-image: url(:/back/Image/button/112.png); color: black;}"
                                                    "QPushButton:pressed{background-image: url(:/back/Image/button/847.png);border-style: inset;}");

    ui->YesOrNOButton->setStyleSheet("QPushButton{background-color:rgb(255, 170, 0);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;}"
                                           "QPushButton:hover{background-color:rgb(255, 170, 0); color: black;}"
                                           "QPushButton:pressed{background-color: rgb(255, 170, 0);border-style: inset;}");


}

void oneWindow::Icon_hide(int num)
{
    //隐藏柱子   num为分控数量
    num++;
    switch (num)
    {

    case 1: num++;ui->label_01->hide();ui->progressBar_01->hide();ui->label_101->hide();ui->pushButton_01->hide();
    case 2: num++;ui->label_02->hide();ui->progressBar_02->hide();ui->label_102->hide();ui->pushButton_02->hide();
    case 3: num++;ui->label_03->hide();ui->progressBar_03->hide();ui->label_103->hide();ui->pushButton_03->hide();
    case 4: num++;ui->label_04->hide();ui->progressBar_04->hide();ui->label_104->hide();ui->pushButton_04->hide();
    case 5: num++;ui->label_05->hide();ui->progressBar_05->hide();ui->label_105->hide();ui->pushButton_05->hide();
    case 6: num++;ui->label_06->hide();ui->progressBar_06->hide();ui->label_106->hide();ui->pushButton_06->hide();
    case 7: num++;ui->label_07->hide();ui->progressBar_07->hide();ui->label_107->hide();ui->pushButton_07->hide();
    case 8: num++;ui->label_08->hide();ui->progressBar_08->hide();ui->label_108->hide();ui->pushButton_08->hide();
    case 9: num++;ui->label_09->hide();ui->progressBar_09->hide();ui->label_109->hide();ui->pushButton_09->hide();
    case 10: num++;ui->label_10->hide();ui->progressBar_10->hide();ui->label_110->hide();ui->pushButton_10->hide();
    case 11: num++;ui->label_11->hide();ui->progressBar_11->hide();ui->label_111->hide();ui->pushButton_11->hide();
    case 12: num++;ui->label_12->hide();ui->progressBar_12->hide();ui->label_112->hide();ui->pushButton_12->hide();
    case 13: num++;ui->label_13->hide();ui->progressBar_13->hide();ui->label_113->hide();ui->pushButton_13->hide();
    case 14: num++;ui->label_14->hide();ui->progressBar_14->hide();ui->label_114->hide();ui->pushButton_14->hide();
    case 15: num++;ui->label_15->hide();ui->progressBar_15->hide();ui->label_115->hide();ui->pushButton_15->hide();
    case 16: num++;ui->label_16->hide();ui->progressBar_16->hide();ui->label_116->hide();ui->pushButton_16->hide();
    case 17: num++;ui->label_17->hide();ui->progressBar_17->hide();ui->label_117->hide();ui->pushButton_17->hide();
    case 18: num++;ui->label_18->hide();ui->progressBar_18->hide();ui->label_118->hide();ui->pushButton_18->hide();
    case 19: num++;ui->label_19->hide();ui->progressBar_19->hide();ui->label_119->hide();ui->pushButton_19->hide();
    case 20: num++;ui->label_20->hide();ui->progressBar_20->hide();ui->label_120->hide();ui->pushButton_20->hide();
    case 21: num++;ui->label_21->hide();ui->progressBar_21->hide();ui->label_121->hide();ui->pushButton_21->hide();
    case 22: num++;ui->label_22->hide();ui->progressBar_22->hide();ui->label_122->hide();ui->pushButton_22->hide();
    case 23: num++;ui->label_23->hide();ui->progressBar_23->hide();ui->label_123->hide();ui->pushButton_23->hide();
    case 24: num++;ui->label_24->hide();ui->progressBar_24->hide();ui->label_124->hide();ui->pushButton_24->hide();
    case 25: num++;ui->label_25->hide();ui->progressBar_25->hide();ui->label_125->hide();ui->pushButton_25->hide();
    case 26: num++;ui->label_26->hide();ui->progressBar_26->hide();ui->label_126->hide();ui->pushButton_26->hide();
    case 27: num++;ui->label_27->hide();ui->progressBar_27->hide();ui->label_127->hide();ui->pushButton_27->hide();
    case 28: num++;ui->label_28->hide();ui->progressBar_28->hide();ui->label_128->hide();ui->pushButton_28->hide();
    case 29: num++;ui->label_29->hide();ui->progressBar_29->hide();ui->label_129->hide();ui->pushButton_29->hide();
    case 30: num++;ui->label_30->hide();ui->progressBar_30->hide();ui->label_130->hide();ui->pushButton_30->hide();
    case 31: num++;ui->label_31->hide();ui->progressBar_31->hide();ui->label_131->hide();ui->pushButton_31->hide();
    case 32: num++;ui->label_32->hide();ui->progressBar_32->hide();ui->label_132->hide();ui->pushButton_32->hide();
    case 33: num++;ui->label_33->hide();ui->progressBar_33->hide();ui->label_133->hide();ui->pushButton_33->hide();
    case 34: num++;ui->label_34->hide();ui->progressBar_34->hide();ui->label_134->hide();ui->pushButton_34->hide();
    case 35: num++;ui->label_35->hide();ui->progressBar_35->hide();ui->label_135->hide();ui->pushButton_35->hide();
    case 36: num++;ui->label_36->hide();ui->progressBar_36->hide();ui->label_136->hide();ui->pushButton_36->hide();
    case 37: num++;ui->label_37->hide();ui->progressBar_37->hide();ui->label_137->hide();ui->pushButton_37->hide();
    case 38: num++;ui->label_38->hide();ui->progressBar_38->hide();ui->label_138->hide();ui->pushButton_38->hide();
    case 39: num++;ui->label_39->hide();ui->progressBar_39->hide();ui->label_139->hide();ui->pushButton_39->hide();
    case 40: num++;ui->label_40->hide();ui->progressBar_40->hide();ui->label_140->hide();ui->pushButton_40->hide();
    default: break;

     }



}

void oneWindow::Icon_show()
{
    //显示所有柱子
        ui->label_01->show();ui->progressBar_01->show();ui->label_101->show();ui->pushButton_01->show();
        ui->label_02->show();ui->progressBar_02->show();ui->label_102->show();ui->pushButton_02->show();
        ui->label_03->show();ui->progressBar_03->show();ui->label_103->show();ui->pushButton_03->show();
        ui->label_04->show();ui->progressBar_04->show();ui->label_104->show();ui->pushButton_04->show();
        ui->label_05->show();ui->progressBar_05->show();ui->label_105->show();ui->pushButton_05->show();
        ui->label_06->show();ui->progressBar_06->show();ui->label_106->show();ui->pushButton_06->show();
        ui->label_07->show();ui->progressBar_07->show();ui->label_107->show();ui->pushButton_07->show();
        ui->label_08->show();ui->progressBar_08->show();ui->label_108->show();ui->pushButton_08->show();
        ui->label_09->show();ui->progressBar_09->show();ui->label_109->show();ui->pushButton_09->show();
        ui->label_10->show();ui->progressBar_10->show();ui->label_110->show();ui->pushButton_10->show();
        ui->label_11->show();ui->progressBar_11->show();ui->label_111->show();ui->pushButton_11->show();
        ui->label_12->show();ui->progressBar_12->show();ui->label_112->show();ui->pushButton_12->show();
        ui->label_13->show();ui->progressBar_13->show();ui->label_113->show();ui->pushButton_13->show();
        ui->label_14->show();ui->progressBar_14->show();ui->label_114->show();ui->pushButton_14->show();
        ui->label_15->show();ui->progressBar_15->show();ui->label_115->show();ui->pushButton_15->show();
        ui->label_16->show();ui->progressBar_16->show();ui->label_116->show();ui->pushButton_16->show();
        ui->label_17->show();ui->progressBar_17->show();ui->label_117->show();ui->pushButton_17->show();
        ui->label_18->show();ui->progressBar_18->show();ui->label_118->show();ui->pushButton_18->show();
        ui->label_19->show();ui->progressBar_19->show();ui->label_119->show();ui->pushButton_19->show();
        ui->label_20->show();ui->progressBar_20->show();ui->label_120->show();ui->pushButton_20->show();
        ui->label_21->show();ui->progressBar_21->show();ui->label_121->show();ui->pushButton_21->show();
        ui->label_22->show();ui->progressBar_22->show();ui->label_122->show();ui->pushButton_22->show();
        ui->label_23->show();ui->progressBar_23->show();ui->label_123->show();ui->pushButton_23->show();
        ui->label_24->show();ui->progressBar_24->show();ui->label_124->show();ui->pushButton_24->show();
        ui->label_25->show();ui->progressBar_25->show();ui->label_125->show();ui->pushButton_25->show();
        ui->label_26->show();ui->progressBar_26->show();ui->label_126->show();ui->pushButton_26->show();
        ui->label_27->show();ui->progressBar_27->show();ui->label_127->show();ui->pushButton_27->show();
        ui->label_28->show();ui->progressBar_28->show();ui->label_128->show();ui->pushButton_28->show();
        ui->label_29->show();ui->progressBar_29->show();ui->label_129->show();ui->pushButton_29->show();
        ui->label_30->show();ui->progressBar_30->show();ui->label_130->show();ui->pushButton_30->show();
        ui->label_31->show();ui->progressBar_31->show();ui->label_131->show();ui->pushButton_31->show();
        ui->label_32->show();ui->progressBar_32->show();ui->label_132->show();ui->pushButton_32->show();
        ui->label_33->show();ui->progressBar_33->show();ui->label_133->show();ui->pushButton_33->show();
        ui->label_34->show();ui->progressBar_34->show();ui->label_134->show();ui->pushButton_34->show();
        ui->label_35->show();ui->progressBar_35->show();ui->label_135->show();ui->pushButton_35->show();
        ui->label_36->show();ui->progressBar_36->show();ui->label_136->show();ui->pushButton_36->show();
        ui->label_37->show();ui->progressBar_37->show();ui->label_137->show();ui->pushButton_37->show();
        ui->label_38->show();ui->progressBar_38->show();ui->label_138->show();ui->pushButton_38->show();
        ui->label_39->show();ui->progressBar_39->show();ui->label_139->show();ui->pushButton_39->show();
        ui->label_40->show();ui->progressBar_40->show();ui->label_140->show();ui->pushButton_40->show();



}



void oneWindow::ID_IconSet(Motor_dev MotorID)
{
    //设置按钮对应的分控号

     if(MotorID.Group == 1)     //1组
     {
         for (int i =0;i < MotorID.page1_num ;i++ )
         {
             switch (i)
             {
             case 0: ui->pushButton_01->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 1: ui->pushButton_02->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 2: ui->pushButton_03->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 3: ui->pushButton_04->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 4: ui->pushButton_05->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 5: ui->pushButton_06->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 6: ui->pushButton_07->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 7: ui->pushButton_08->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 8: ui->pushButton_09->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 9: ui->pushButton_10->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 10: ui->pushButton_11->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 11: ui->pushButton_12->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 12: ui->pushButton_13->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 13: ui->pushButton_14->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 14: ui->pushButton_15->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 15: ui->pushButton_16->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 16: ui->pushButton_17->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 17: ui->pushButton_18->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 18: ui->pushButton_19->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 19: ui->pushButton_20->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 20: ui->pushButton_21->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 21: ui->pushButton_22->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 22: ui->pushButton_23->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 23: ui->pushButton_24->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 24: ui->pushButton_25->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 25: ui->pushButton_26->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 26: ui->pushButton_27->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 27: ui->pushButton_28->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 28: ui->pushButton_29->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 29: ui->pushButton_30->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 30: ui->pushButton_31->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 31: ui->pushButton_32->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 32: ui->pushButton_33->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 33: ui->pushButton_34->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 34: ui->pushButton_35->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 35: ui->pushButton_36->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 36: ui->pushButton_37->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 37: ui->pushButton_38->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 38: ui->pushButton_39->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             case 39: ui->pushButton_40->setText(QString::number(MotorID.branch_oneGroup[i].branch.id));break;
             default: break;
             }


         }

     }else if (MotorID.Group == 2)    //2组
     {
         for (int i =0;i < MotorID.page2_num ;i++ )
         {
             switch (i)
             {
             case 0: ui->pushButton_01->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 1: ui->pushButton_02->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 2: ui->pushButton_03->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 3: ui->pushButton_04->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 4: ui->pushButton_05->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 5: ui->pushButton_06->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 6: ui->pushButton_07->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 7: ui->pushButton_08->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 8: ui->pushButton_09->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 9: ui->pushButton_10->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 10: ui->pushButton_11->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 11: ui->pushButton_12->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 12: ui->pushButton_13->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 13: ui->pushButton_14->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 14: ui->pushButton_15->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 15: ui->pushButton_16->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 16: ui->pushButton_17->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 17: ui->pushButton_18->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 18: ui->pushButton_19->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 19: ui->pushButton_20->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 20: ui->pushButton_21->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 21: ui->pushButton_22->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 22: ui->pushButton_23->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 23: ui->pushButton_24->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 24: ui->pushButton_25->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 25: ui->pushButton_26->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 26: ui->pushButton_27->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 27: ui->pushButton_28->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 28: ui->pushButton_29->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 29: ui->pushButton_30->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 30: ui->pushButton_31->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 31: ui->pushButton_32->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 32: ui->pushButton_33->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 33: ui->pushButton_34->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 34: ui->pushButton_35->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 35: ui->pushButton_36->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 36: ui->pushButton_37->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 37: ui->pushButton_38->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 38: ui->pushButton_39->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             case 39: ui->pushButton_40->setText(QString::number(MotorID.branch_twoGroup[i].branch.id));break;
             default: break;
             }


         }

    }

}

int oneWindow::Find_ID(int ID,QByteArray array1)
{
    //查找ID

    for (int i = 0;i <array1.length() ;i++ )
    {
        if(array1.at(i) == ID)
        {
            return  i+1;
        }

    }

    return -1;


}

void oneWindow::Set_ProgressBarData(int ID,int value)
{
    //设置进度条和压力值数据

    int progress_value = (value*100)/60;        //数据放大了十倍
    int lable1 = value/10;
    int lable2 = value%10;
    QString Label_Value = "";
    if(lable1 == 0 && lable2 == 0)
    {
     Label_Value = QString("---");
    }else
    {
     Label_Value = QString("%1.%2T").arg(lable1).arg(lable2);
    }

    switch (ID)
    {
    case 0:ui->progressBar_01->setValue(progress_value);ui->label_01->setText(Label_Value);break;
    case 1:ui->progressBar_02->setValue(progress_value);ui->label_02->setText(Label_Value);break;
    case 2:ui->progressBar_03->setValue(progress_value);ui->label_03->setText(Label_Value);break;
    case 3:ui->progressBar_04->setValue(progress_value);ui->label_04->setText(Label_Value);break;
    case 4:ui->progressBar_05->setValue(progress_value);ui->label_05->setText(Label_Value);break;
    case 5:ui->progressBar_06->setValue(progress_value);ui->label_06->setText(Label_Value);break;
    case 6:ui->progressBar_07->setValue(progress_value);ui->label_07->setText(Label_Value);break;
    case 7:ui->progressBar_08->setValue(progress_value);ui->label_08->setText(Label_Value);break;
    case 8:ui->progressBar_09->setValue(progress_value);ui->label_09->setText(Label_Value);break;
    case 9:ui->progressBar_10->setValue(progress_value);ui->label_10->setText(Label_Value);break;
    case 10:ui->progressBar_11->setValue(progress_value);ui->label_11->setText(Label_Value);break;
    case 11:ui->progressBar_12->setValue(progress_value);ui->label_12->setText(Label_Value);break;
    case 12:ui->progressBar_13->setValue(progress_value);ui->label_13->setText(Label_Value);break;
    case 13:ui->progressBar_14->setValue(progress_value);ui->label_14->setText(Label_Value);break;
    case 14:ui->progressBar_15->setValue(progress_value);ui->label_15->setText(Label_Value);break;
    case 15:ui->progressBar_16->setValue(progress_value);ui->label_16->setText(Label_Value);break;
    case 16:ui->progressBar_17->setValue(progress_value);ui->label_17->setText(Label_Value);break;
    case 17:ui->progressBar_18->setValue(progress_value);ui->label_18->setText(Label_Value);break;
    case 18:ui->progressBar_19->setValue(progress_value);ui->label_19->setText(Label_Value);break;
    case 19:ui->progressBar_20->setValue(progress_value);ui->label_20->setText(Label_Value);break;
    case 20:ui->progressBar_21->setValue(progress_value);ui->label_21->setText(Label_Value);break;
    case 21:ui->progressBar_22->setValue(progress_value);ui->label_22->setText(Label_Value);break;
    case 22:ui->progressBar_23->setValue(progress_value);ui->label_23->setText(Label_Value);break;
    case 23:ui->progressBar_24->setValue(progress_value);ui->label_24->setText(Label_Value);break;
    case 24:ui->progressBar_25->setValue(progress_value);ui->label_25->setText(Label_Value);break;
    case 25:ui->progressBar_26->setValue(progress_value);ui->label_26->setText(Label_Value);break;
    case 26:ui->progressBar_27->setValue(progress_value);ui->label_27->setText(Label_Value);break;
    case 27:ui->progressBar_28->setValue(progress_value);ui->label_28->setText(Label_Value);break;
    case 28:ui->progressBar_29->setValue(progress_value);ui->label_29->setText(Label_Value);break;
    case 29:ui->progressBar_30->setValue(progress_value);ui->label_30->setText(Label_Value);break;
    case 30:ui->progressBar_31->setValue(progress_value);ui->label_31->setText(Label_Value);break;
    case 31:ui->progressBar_32->setValue(progress_value);ui->label_32->setText(Label_Value);break;
    case 32:ui->progressBar_33->setValue(progress_value);ui->label_33->setText(Label_Value);break;
    case 33:ui->progressBar_34->setValue(progress_value);ui->label_34->setText(Label_Value);break;
    case 34:ui->progressBar_35->setValue(progress_value);ui->label_35->setText(Label_Value);break;
    case 35:ui->progressBar_36->setValue(progress_value);ui->label_36->setText(Label_Value);break;
    case 36:ui->progressBar_37->setValue(progress_value);ui->label_37->setText(Label_Value);break;
    case 37:ui->progressBar_38->setValue(progress_value);ui->label_38->setText(Label_Value);break;
    case 38:ui->progressBar_39->setValue(progress_value);ui->label_39->setText(Label_Value);break;
    case 39:ui->progressBar_40->setValue(progress_value);ui->label_40->setText(Label_Value);break;
    default: break;




    }



}

void oneWindow::Set_ProgressBarState(int ID, int State)
{
    //设置显示状态

//    Stop = 0x01,          //停机
//    Risining ,      //上升
//    Loose  ,         //松钩
//    Tight  ,         //紧钩
//    Falling ,       //下降
//    Scan,           //扫描
//    Set,             //设置参数
//    Overload ,       //超载
//    LOSS ,           //失载
//    CurrentOver ,   //电流过大
//    Wait,           //等待切换响应
//    Bolt,            //断线


    switch (ID)
    {
    case 0:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
         {
        case Stop:     ui->progressBar_01->setStyleSheet(Style_progress_risining);ui->label_01->setStyleSheet(Style_label_boder_MO);ui->label_101->setStyleSheet(Style_label_boder_MO);ui->label_101->setText("停机"); break;
        case Risining: ui->progressBar_01->setStyleSheet(Style_progress_risining);ui->label_01->setStyleSheet(Style_label_boder_move);ui->label_101->setStyleSheet(Style_label_boder_move);ui->label_101->setText("上升"); break;
        case Loose:ui->progressBar_01->setStyleSheet(Style_progress_risining);ui->label_01->setStyleSheet(Style_label_boder_MO);ui->label_101->setStyleSheet(Style_label_boder_MO);ui->label_101->setText("松钩");break;
        case Tight:ui->progressBar_01->setStyleSheet(Style_progress_risining);ui->label_01->setStyleSheet(Style_label_boder_MO);ui->label_101->setStyleSheet(Style_label_boder_MO);ui->label_101->setText("紧钩");break;
        case Falling:  ui->progressBar_01->setStyleSheet(Style_progress_risining);ui->label_01->setStyleSheet(Style_label_boder_move);ui->label_101->setStyleSheet(Style_label_boder_move);ui->label_101->setText("下降"); break;
        case Overload: ui->progressBar_01->setStyleSheet(Style_progress_overload);ui->label_01->setStyleSheet(Style_label_boder_NO);ui->label_101->setStyleSheet(Style_label_boder_NO);ui->label_101->setText("超载"); break;
        case LOSS:     ui->progressBar_01->setStyleSheet(Style_progress_overload);ui->label_01->setStyleSheet(Style_label_boder_NO);ui->label_101->setStyleSheet(Style_label_boder_NO);ui->label_101->setText("失载"); break;       
        case CurrentOver:     ui->progressBar_01->setStyleSheet(Style_progress_overload);ui->label_01->setStyleSheet(Style_label_boder_NO);ui->label_101->setStyleSheet(Style_label_boder_NO);ui->label_101->setText("电流"); break;
        case Wait:ui->progressBar_01->setStyleSheet(Style_progress_risining);ui->label_01->setStyleSheet(Style_label_boder_MO);ui->label_101->setStyleSheet(Style_label_boder_MO);ui->label_101->setText("等待");break;
        case Bolt:     ui->progressBar_01->setStyleSheet(Style_progress_overload);ui->label_01->setStyleSheet(Style_label_boder_NO);ui->label_101->setStyleSheet(Style_label_boder_NO);ui->label_101->setText("断线"); break;
        case Warning: ui->progressBar_01->setStyleSheet(Style_progress_warning);ui->label_01->setStyleSheet(Style_label_boder_warning);ui->label_101->setStyleSheet(Style_label_boder_warning);break;
        case Set: ui->progressBar_01->setStyleSheet(Style_progress_warning);ui->label_01->setStyleSheet(Style_label_boder_warning);ui->label_101->setStyleSheet(Style_label_boder_warning);ui->label_101->setText("设置");break;

         } ;break;

    case 1:switch (State)
        {
        case Stop:     ui->progressBar_02->setStyleSheet(Style_progress_risining);ui->label_02->setStyleSheet(Style_label_boder_MO);ui->label_102->setStyleSheet(Style_label_boder_MO);ui->label_102->setText("停机"); break;
        case Risining: ui->progressBar_02->setStyleSheet(Style_progress_risining);ui->label_02->setStyleSheet(Style_label_boder_move);ui->label_102->setStyleSheet(Style_label_boder_move);ui->label_102->setText("上升"); break;
        case Loose:    ui->progressBar_02->setStyleSheet(Style_progress_risining);ui->label_02->setStyleSheet(Style_label_boder_MO);ui->label_102->setStyleSheet(Style_label_boder_MO);ui->label_102->setText("松钩"); break;
        case Tight:    ui->progressBar_02->setStyleSheet(Style_progress_risining);ui->label_02->setStyleSheet(Style_label_boder_MO);ui->label_102->setStyleSheet(Style_label_boder_MO);ui->label_102->setText("紧钩"); break;
        case Falling:  ui->progressBar_02->setStyleSheet(Style_progress_risining);ui->label_02->setStyleSheet(Style_label_boder_move);ui->label_102->setStyleSheet(Style_label_boder_move);ui->label_102->setText("下降"); break;
        case Overload: ui->progressBar_02->setStyleSheet(Style_progress_overload);ui->label_02->setStyleSheet(Style_label_boder_NO);ui->label_102->setStyleSheet(Style_label_boder_NO);ui->label_102->setText("超载"); break;
        case LOSS:     ui->progressBar_02->setStyleSheet(Style_progress_overload);ui->label_02->setStyleSheet(Style_label_boder_NO);ui->label_102->setStyleSheet(Style_label_boder_NO);ui->label_102->setText("失载"); break;
        case CurrentOver:  ui->progressBar_02->setStyleSheet(Style_progress_overload);ui->label_02->setStyleSheet(Style_label_boder_NO);ui->label_102->setStyleSheet(Style_label_boder_NO);ui->label_102->setText("电流"); break;
        case Wait:      ui->progressBar_02->setStyleSheet(Style_progress_risining);ui->label_02->setStyleSheet(Style_label_boder_MO);ui->label_102->setStyleSheet(Style_label_boder_MO);ui->label_102->setText("等待"); break;
        case Bolt:      ui->progressBar_02->setStyleSheet(Style_progress_overload);ui->label_02->setStyleSheet(Style_label_boder_NO);ui->label_102->setStyleSheet(Style_label_boder_NO);ui->label_102->setText("断线"); break;
        case Warning: ui->progressBar_02->setStyleSheet(Style_progress_warning);ui->label_02->setStyleSheet(Style_label_boder_warning);ui->label_102->setStyleSheet(Style_label_boder_warning);break;
        case Set: ui->progressBar_02->setStyleSheet(Style_progress_warning);ui->label_02->setStyleSheet(Style_label_boder_warning);ui->label_102->setStyleSheet(Style_label_boder_warning);ui->label_102->setText("设置");break;


        } ;break;
    case 2:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_03->setStyleSheet(Style_progress_risining);ui->label_03->setStyleSheet(Style_label_boder_MO);ui->label_103->setStyleSheet(Style_label_boder_MO);ui->label_103->setText("停机"); break;
        case Risining: ui->progressBar_03->setStyleSheet(Style_progress_risining);ui->label_03->setStyleSheet(Style_label_boder_move);ui->label_103->setStyleSheet(Style_label_boder_move);ui->label_103->setText("上升"); break;
        case Loose:    ui->progressBar_03->setStyleSheet(Style_progress_risining);ui->label_03->setStyleSheet(Style_label_boder_MO);ui->label_103->setStyleSheet(Style_label_boder_MO);ui->label_103->setText("松钩"); break;
        case Tight:    ui->progressBar_03->setStyleSheet(Style_progress_risining);ui->label_03->setStyleSheet(Style_label_boder_MO);ui->label_103->setStyleSheet(Style_label_boder_MO);ui->label_103->setText("紧钩"); break;
        case Falling:  ui->progressBar_03->setStyleSheet(Style_progress_risining);ui->label_03->setStyleSheet(Style_label_boder_move);ui->label_103->setStyleSheet(Style_label_boder_move);ui->label_103->setText("下降"); break;
        case Overload: ui->progressBar_03->setStyleSheet(Style_progress_overload);ui->label_03->setStyleSheet(Style_label_boder_NO);ui->label_103->setStyleSheet(Style_label_boder_NO);ui->label_103->setText("超载"); break;
        case LOSS:     ui->progressBar_03->setStyleSheet(Style_progress_overload);ui->label_03->setStyleSheet(Style_label_boder_NO);ui->label_103->setStyleSheet(Style_label_boder_NO);ui->label_103->setText("失载"); break;
        case CurrentOver:  ui->progressBar_03->setStyleSheet(Style_progress_overload);ui->label_03->setStyleSheet(Style_label_boder_NO);ui->label_103->setStyleSheet(Style_label_boder_NO);ui->label_103->setText("电流"); break;
        case Wait:      ui->progressBar_03->setStyleSheet(Style_progress_risining);ui->label_03->setStyleSheet(Style_label_boder_MO);ui->label_103->setStyleSheet(Style_label_boder_MO);ui->label_103->setText("等待"); break;
        case Bolt:      ui->progressBar_03->setStyleSheet(Style_progress_overload);ui->label_03->setStyleSheet(Style_label_boder_NO);ui->label_103->setStyleSheet(Style_label_boder_NO);ui->label_103->setText("断线"); break;
        case Warning: ui->progressBar_03->setStyleSheet(Style_progress_warning);ui->label_03->setStyleSheet(Style_label_boder_warning);ui->label_103->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_03->setStyleSheet(Style_progress_warning);ui->label_03->setStyleSheet(Style_label_boder_warning);ui->label_103->setStyleSheet(Style_label_boder_warning);ui->label_103->setText("设置"); break;

        } ;break;
    case 3:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_04->setStyleSheet(Style_progress_risining);ui->label_04->setStyleSheet(Style_label_boder_MO);ui->label_104->setStyleSheet(Style_label_boder_MO);ui->label_104->setText("停机"); break;
        case Risining: ui->progressBar_04->setStyleSheet(Style_progress_risining);ui->label_04->setStyleSheet(Style_label_boder_move);ui->label_104->setStyleSheet(Style_label_boder_move);ui->label_104->setText("上升"); break;
        case Loose:    ui->progressBar_04->setStyleSheet(Style_progress_risining);ui->label_04->setStyleSheet(Style_label_boder_MO);ui->label_104->setStyleSheet(Style_label_boder_MO);ui->label_104->setText("松钩"); break;
        case Tight:    ui->progressBar_04->setStyleSheet(Style_progress_risining);ui->label_04->setStyleSheet(Style_label_boder_MO);ui->label_104->setStyleSheet(Style_label_boder_MO);ui->label_104->setText("紧钩"); break;
        case Falling:  ui->progressBar_04->setStyleSheet(Style_progress_risining);ui->label_04->setStyleSheet(Style_label_boder_move);ui->label_104->setStyleSheet(Style_label_boder_move);ui->label_104->setText("下降"); break;
        case Overload: ui->progressBar_04->setStyleSheet(Style_progress_overload);ui->label_04->setStyleSheet(Style_label_boder_NO);ui->label_104->setStyleSheet(Style_label_boder_NO);ui->label_104->setText("超载"); break;
        case LOSS:     ui->progressBar_04->setStyleSheet(Style_progress_overload);ui->label_04->setStyleSheet(Style_label_boder_NO);ui->label_104->setStyleSheet(Style_label_boder_NO);ui->label_104->setText("失载"); break;
        case CurrentOver:  ui->progressBar_04->setStyleSheet(Style_progress_overload);ui->label_04->setStyleSheet(Style_label_boder_NO);ui->label_104->setStyleSheet(Style_label_boder_NO);ui->label_104->setText("电流"); break;
        case Wait:      ui->progressBar_04->setStyleSheet(Style_progress_risining);ui->label_04->setStyleSheet(Style_label_boder_MO);ui->label_104->setStyleSheet(Style_label_boder_MO);ui->label_104->setText("等待"); break;
        case Bolt:      ui->progressBar_04->setStyleSheet(Style_progress_overload);ui->label_04->setStyleSheet(Style_label_boder_NO);ui->label_104->setStyleSheet(Style_label_boder_NO);ui->label_104->setText("断线"); break;
        case Warning: ui->progressBar_04->setStyleSheet(Style_progress_warning);ui->label_04->setStyleSheet(Style_label_boder_warning);ui->label_104->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_04->setStyleSheet(Style_progress_warning);ui->label_04->setStyleSheet(Style_label_boder_warning);ui->label_104->setStyleSheet(Style_label_boder_warning);ui->label_104->setText("设置"); break;

        } ;break;
    case 4:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_05->setStyleSheet(Style_progress_risining);ui->label_05->setStyleSheet(Style_label_boder_MO);ui->label_105->setStyleSheet(Style_label_boder_MO);ui->label_105->setText("停机"); break;
        case Risining: ui->progressBar_05->setStyleSheet(Style_progress_risining);ui->label_05->setStyleSheet(Style_label_boder_move);ui->label_105->setStyleSheet(Style_label_boder_move);ui->label_105->setText("上升"); break;
        case Loose:    ui->progressBar_05->setStyleSheet(Style_progress_risining);ui->label_05->setStyleSheet(Style_label_boder_MO);ui->label_105->setStyleSheet(Style_label_boder_MO);ui->label_105->setText("松钩"); break;
        case Tight:    ui->progressBar_05->setStyleSheet(Style_progress_risining);ui->label_05->setStyleSheet(Style_label_boder_MO);ui->label_105->setStyleSheet(Style_label_boder_MO);ui->label_105->setText("紧钩"); break;
        case Falling:  ui->progressBar_05->setStyleSheet(Style_progress_risining);ui->label_05->setStyleSheet(Style_label_boder_move);ui->label_105->setStyleSheet(Style_label_boder_move);ui->label_105->setText("下降"); break;
        case Overload: ui->progressBar_05->setStyleSheet(Style_progress_overload);ui->label_05->setStyleSheet(Style_label_boder_NO);ui->label_105->setStyleSheet(Style_label_boder_NO);ui->label_105->setText("超载"); break;
        case LOSS:     ui->progressBar_05->setStyleSheet(Style_progress_overload);ui->label_05->setStyleSheet(Style_label_boder_NO);ui->label_105->setStyleSheet(Style_label_boder_NO);ui->label_105->setText("失载"); break;
        case CurrentOver:  ui->progressBar_05->setStyleSheet(Style_progress_overload);ui->label_05->setStyleSheet(Style_label_boder_NO);ui->label_105->setStyleSheet(Style_label_boder_NO);ui->label_105->setText("电流"); break;
        case Wait:      ui->progressBar_05->setStyleSheet(Style_progress_risining);ui->label_05->setStyleSheet(Style_label_boder_MO);ui->label_105->setStyleSheet(Style_label_boder_MO);ui->label_105->setText("等待"); break;
        case Bolt:      ui->progressBar_05->setStyleSheet(Style_progress_overload);ui->label_05->setStyleSheet(Style_label_boder_NO);ui->label_105->setStyleSheet(Style_label_boder_NO);ui->label_105->setText("断线"); break;
        case Warning: ui->progressBar_05->setStyleSheet(Style_progress_warning);ui->label_05->setStyleSheet(Style_label_boder_warning);ui->label_105->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_05->setStyleSheet(Style_progress_warning);ui->label_05->setStyleSheet(Style_label_boder_warning);ui->label_105->setStyleSheet(Style_label_boder_warning);ui->label_105->setText("设置"); break;

        } ;break;
    case 5:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_06->setStyleSheet(Style_progress_risining);ui->label_06->setStyleSheet(Style_label_boder_MO);ui->label_106->setStyleSheet(Style_label_boder_MO);ui->label_106->setText("停机"); break;
        case Risining: ui->progressBar_06->setStyleSheet(Style_progress_risining);ui->label_06->setStyleSheet(Style_label_boder_move);ui->label_106->setStyleSheet(Style_label_boder_move);ui->label_106->setText("上升"); break;
        case Loose:    ui->progressBar_06->setStyleSheet(Style_progress_risining);ui->label_06->setStyleSheet(Style_label_boder_MO);ui->label_106->setStyleSheet(Style_label_boder_MO);ui->label_106->setText("松钩"); break;
        case Tight:    ui->progressBar_06->setStyleSheet(Style_progress_risining);ui->label_06->setStyleSheet(Style_label_boder_MO);ui->label_106->setStyleSheet(Style_label_boder_MO);ui->label_106->setText("紧钩"); break;
        case Falling:  ui->progressBar_06->setStyleSheet(Style_progress_risining);ui->label_06->setStyleSheet(Style_label_boder_move);ui->label_106->setStyleSheet(Style_label_boder_move);ui->label_106->setText("下降"); break;
        case Overload: ui->progressBar_06->setStyleSheet(Style_progress_overload);ui->label_06->setStyleSheet(Style_label_boder_NO);ui->label_106->setStyleSheet(Style_label_boder_NO);ui->label_106->setText("超载"); break;
        case LOSS:     ui->progressBar_06->setStyleSheet(Style_progress_overload);ui->label_06->setStyleSheet(Style_label_boder_NO);ui->label_106->setStyleSheet(Style_label_boder_NO);ui->label_106->setText("失载"); break;
        case CurrentOver:  ui->progressBar_06->setStyleSheet(Style_progress_overload);ui->label_06->setStyleSheet(Style_label_boder_NO);ui->label_106->setStyleSheet(Style_label_boder_NO);ui->label_106->setText("电流"); break;
        case Wait:      ui->progressBar_06->setStyleSheet(Style_progress_risining);ui->label_06->setStyleSheet(Style_label_boder_MO);ui->label_106->setStyleSheet(Style_label_boder_MO);ui->label_106->setText("等待"); break;
        case Bolt:      ui->progressBar_06->setStyleSheet(Style_progress_overload);ui->label_06->setStyleSheet(Style_label_boder_NO);ui->label_106->setStyleSheet(Style_label_boder_NO);ui->label_106->setText("断线"); break;
        case Warning: ui->progressBar_06->setStyleSheet(Style_progress_warning);ui->label_06->setStyleSheet(Style_label_boder_warning);ui->label_106->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_06->setStyleSheet(Style_progress_warning);ui->label_06->setStyleSheet(Style_label_boder_warning);ui->label_106->setStyleSheet(Style_label_boder_warning);ui->label_106->setText("设置"); break;

        } ;break;
    case 6:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_07->setStyleSheet(Style_progress_risining);ui->label_07->setStyleSheet(Style_label_boder_MO);ui->label_107->setStyleSheet(Style_label_boder_MO);ui->label_107->setText("停机"); break;
        case Risining: ui->progressBar_07->setStyleSheet(Style_progress_risining);ui->label_07->setStyleSheet(Style_label_boder_move);ui->label_107->setStyleSheet(Style_label_boder_move);ui->label_107->setText("上升"); break;
        case Loose:    ui->progressBar_07->setStyleSheet(Style_progress_risining);ui->label_07->setStyleSheet(Style_label_boder_MO);ui->label_107->setStyleSheet(Style_label_boder_MO);ui->label_107->setText("松钩"); break;
        case Tight:    ui->progressBar_07->setStyleSheet(Style_progress_risining);ui->label_07->setStyleSheet(Style_label_boder_MO);ui->label_107->setStyleSheet(Style_label_boder_MO);ui->label_107->setText("紧钩"); break;
        case Falling:  ui->progressBar_07->setStyleSheet(Style_progress_risining);ui->label_07->setStyleSheet(Style_label_boder_move);ui->label_107->setStyleSheet(Style_label_boder_move);ui->label_107->setText("下降"); break;
        case Overload: ui->progressBar_07->setStyleSheet(Style_progress_overload);ui->label_07->setStyleSheet(Style_label_boder_NO);ui->label_107->setStyleSheet(Style_label_boder_NO);ui->label_107->setText("超载"); break;
        case LOSS:     ui->progressBar_07->setStyleSheet(Style_progress_overload);ui->label_07->setStyleSheet(Style_label_boder_NO);ui->label_107->setStyleSheet(Style_label_boder_NO);ui->label_107->setText("失载"); break;
        case CurrentOver:  ui->progressBar_07->setStyleSheet(Style_progress_overload);ui->label_07->setStyleSheet(Style_label_boder_NO);ui->label_107->setStyleSheet(Style_label_boder_NO);ui->label_107->setText("电流"); break;
        case Wait:      ui->progressBar_07->setStyleSheet(Style_progress_risining);ui->label_07->setStyleSheet(Style_label_boder_MO);ui->label_107->setStyleSheet(Style_label_boder_MO);ui->label_107->setText("等待"); break;
        case Bolt:      ui->progressBar_07->setStyleSheet(Style_progress_overload);ui->label_07->setStyleSheet(Style_label_boder_NO);ui->label_107->setStyleSheet(Style_label_boder_NO);ui->label_107->setText("断线"); break;
        case Warning: ui->progressBar_07->setStyleSheet(Style_progress_warning);ui->label_07->setStyleSheet(Style_label_boder_warning);ui->label_107->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_07->setStyleSheet(Style_progress_warning);ui->label_07->setStyleSheet(Style_label_boder_warning);ui->label_107->setStyleSheet(Style_label_boder_warning);ui->label_107->setText("设置"); break;

        } ;break;
    case 7:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_08->setStyleSheet(Style_progress_risining);ui->label_08->setStyleSheet(Style_label_boder_MO);ui->label_108->setStyleSheet(Style_label_boder_MO);ui->label_108->setText("停机"); break;
        case Risining: ui->progressBar_08->setStyleSheet(Style_progress_risining);ui->label_08->setStyleSheet(Style_label_boder_move);ui->label_108->setStyleSheet(Style_label_boder_move);ui->label_108->setText("上升"); break;
        case Loose:    ui->progressBar_08->setStyleSheet(Style_progress_risining);ui->label_08->setStyleSheet(Style_label_boder_MO);ui->label_108->setStyleSheet(Style_label_boder_MO);ui->label_108->setText("松钩"); break;
        case Tight:    ui->progressBar_08->setStyleSheet(Style_progress_risining);ui->label_08->setStyleSheet(Style_label_boder_MO);ui->label_108->setStyleSheet(Style_label_boder_MO);ui->label_108->setText("紧钩"); break;
        case Falling:  ui->progressBar_08->setStyleSheet(Style_progress_risining);ui->label_08->setStyleSheet(Style_label_boder_move);ui->label_108->setStyleSheet(Style_label_boder_move);ui->label_108->setText("下降"); break;
        case Overload: ui->progressBar_08->setStyleSheet(Style_progress_overload);ui->label_08->setStyleSheet(Style_label_boder_NO);ui->label_108->setStyleSheet(Style_label_boder_NO);ui->label_108->setText("超载"); break;
        case LOSS:     ui->progressBar_08->setStyleSheet(Style_progress_overload);ui->label_08->setStyleSheet(Style_label_boder_NO);ui->label_108->setStyleSheet(Style_label_boder_NO);ui->label_108->setText("失载"); break;
        case CurrentOver:  ui->progressBar_08->setStyleSheet(Style_progress_overload);ui->label_08->setStyleSheet(Style_label_boder_NO);ui->label_108->setStyleSheet(Style_label_boder_NO);ui->label_108->setText("电流"); break;
        case Wait:      ui->progressBar_08->setStyleSheet(Style_progress_risining);ui->label_08->setStyleSheet(Style_label_boder_MO);ui->label_108->setStyleSheet(Style_label_boder_MO);ui->label_108->setText("等待"); break;
        case Bolt:      ui->progressBar_08->setStyleSheet(Style_progress_overload);ui->label_08->setStyleSheet(Style_label_boder_NO);ui->label_108->setStyleSheet(Style_label_boder_NO);ui->label_108->setText("断线"); break;
        case Warning: ui->progressBar_08->setStyleSheet(Style_progress_warning);ui->label_08->setStyleSheet(Style_label_boder_warning);ui->label_108->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_08->setStyleSheet(Style_progress_warning);ui->label_08->setStyleSheet(Style_label_boder_warning);ui->label_108->setStyleSheet(Style_label_boder_warning);ui->label_108->setText("设置"); break;

        } ;break;
    case 8:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_09->setStyleSheet(Style_progress_risining);ui->label_09->setStyleSheet(Style_label_boder_MO);ui->label_109->setStyleSheet(Style_label_boder_MO);ui->label_109->setText("停机"); break;
        case Risining: ui->progressBar_09->setStyleSheet(Style_progress_risining);ui->label_09->setStyleSheet(Style_label_boder_move);ui->label_109->setStyleSheet(Style_label_boder_move);ui->label_109->setText("上升"); break;
        case Loose:    ui->progressBar_09->setStyleSheet(Style_progress_risining);ui->label_09->setStyleSheet(Style_label_boder_MO);ui->label_109->setStyleSheet(Style_label_boder_MO);ui->label_109->setText("松钩"); break;
        case Tight:    ui->progressBar_09->setStyleSheet(Style_progress_risining);ui->label_09->setStyleSheet(Style_label_boder_MO);ui->label_109->setStyleSheet(Style_label_boder_MO);ui->label_109->setText("紧钩"); break;
        case Falling:  ui->progressBar_09->setStyleSheet(Style_progress_risining);ui->label_09->setStyleSheet(Style_label_boder_move);ui->label_109->setStyleSheet(Style_label_boder_move);ui->label_109->setText("下降"); break;
        case Overload: ui->progressBar_09->setStyleSheet(Style_progress_overload);ui->label_09->setStyleSheet(Style_label_boder_NO);ui->label_109->setStyleSheet(Style_label_boder_NO);ui->label_109->setText("超载"); break;
        case LOSS:     ui->progressBar_09->setStyleSheet(Style_progress_overload);ui->label_09->setStyleSheet(Style_label_boder_NO);ui->label_109->setStyleSheet(Style_label_boder_NO);ui->label_109->setText("失载"); break;
        case CurrentOver:  ui->progressBar_09->setStyleSheet(Style_progress_overload);ui->label_09->setStyleSheet(Style_label_boder_NO);ui->label_109->setStyleSheet(Style_label_boder_NO);ui->label_109->setText("电流"); break;
        case Wait:      ui->progressBar_09->setStyleSheet(Style_progress_risining);ui->label_09->setStyleSheet(Style_label_boder_MO);ui->label_109->setStyleSheet(Style_label_boder_MO);ui->label_109->setText("等待"); break;
        case Bolt:      ui->progressBar_09->setStyleSheet(Style_progress_overload);ui->label_09->setStyleSheet(Style_label_boder_NO);ui->label_109->setStyleSheet(Style_label_boder_NO);ui->label_109->setText("断线"); break;
        case Warning: ui->progressBar_09->setStyleSheet(Style_progress_warning);ui->label_09->setStyleSheet(Style_label_boder_warning);ui->label_109->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_09->setStyleSheet(Style_progress_warning);ui->label_09->setStyleSheet(Style_label_boder_warning);ui->label_109->setStyleSheet(Style_label_boder_warning);ui->label_109->setText("设置"); break;

        } ;break;
    case 9:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_10->setStyleSheet(Style_progress_risining);ui->label_10->setStyleSheet(Style_label_boder_MO);ui->label_110->setStyleSheet(Style_label_boder_MO);ui->label_110->setText("停机"); break;
        case Risining: ui->progressBar_10->setStyleSheet(Style_progress_risining);ui->label_10->setStyleSheet(Style_label_boder_move);ui->label_110->setStyleSheet(Style_label_boder_move);ui->label_110->setText("上升"); break;
        case Loose:    ui->progressBar_10->setStyleSheet(Style_progress_risining);ui->label_10->setStyleSheet(Style_label_boder_MO);ui->label_110->setStyleSheet(Style_label_boder_MO);ui->label_110->setText("松钩"); break;
        case Tight:    ui->progressBar_10->setStyleSheet(Style_progress_risining);ui->label_10->setStyleSheet(Style_label_boder_MO);ui->label_110->setStyleSheet(Style_label_boder_MO);ui->label_110->setText("紧钩"); break;
        case Falling:  ui->progressBar_10->setStyleSheet(Style_progress_risining);ui->label_10->setStyleSheet(Style_label_boder_move);ui->label_110->setStyleSheet(Style_label_boder_move);ui->label_110->setText("下降"); break;
        case Overload: ui->progressBar_10->setStyleSheet(Style_progress_overload);ui->label_10->setStyleSheet(Style_label_boder_NO);ui->label_110->setStyleSheet(Style_label_boder_NO);ui->label_110->setText("超载"); break;
        case LOSS:     ui->progressBar_10->setStyleSheet(Style_progress_overload);ui->label_10->setStyleSheet(Style_label_boder_NO);ui->label_110->setStyleSheet(Style_label_boder_NO);ui->label_110->setText("失载"); break;
        case CurrentOver:  ui->progressBar_10->setStyleSheet(Style_progress_overload);ui->label_10->setStyleSheet(Style_label_boder_NO);ui->label_110->setStyleSheet(Style_label_boder_NO);ui->label_110->setText("电流"); break;
        case Wait:      ui->progressBar_10->setStyleSheet(Style_progress_risining);ui->label_10->setStyleSheet(Style_label_boder_MO);ui->label_110->setStyleSheet(Style_label_boder_MO);ui->label_110->setText("等待"); break;
        case Bolt:      ui->progressBar_10->setStyleSheet(Style_progress_overload);ui->label_10->setStyleSheet(Style_label_boder_NO);ui->label_110->setStyleSheet(Style_label_boder_NO);ui->label_110->setText("断线"); break;
        case Warning: ui->progressBar_10->setStyleSheet(Style_progress_warning);ui->label_10->setStyleSheet(Style_label_boder_warning);ui->label_110->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_10->setStyleSheet(Style_progress_warning);ui->label_10->setStyleSheet(Style_label_boder_warning);ui->label_110->setStyleSheet(Style_label_boder_warning);ui->label_110->setText("设置"); break;

        } ;break;
    case 10:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_11->setStyleSheet(Style_progress_risining);ui->label_11->setStyleSheet(Style_label_boder_MO);ui->label_111->setStyleSheet(Style_label_boder_MO);ui->label_111->setText("停机"); break;
        case Risining: ui->progressBar_11->setStyleSheet(Style_progress_risining);ui->label_11->setStyleSheet(Style_label_boder_move);ui->label_111->setStyleSheet(Style_label_boder_move);ui->label_111->setText("上升"); break;
        case Loose:    ui->progressBar_11->setStyleSheet(Style_progress_risining);ui->label_11->setStyleSheet(Style_label_boder_MO);ui->label_111->setStyleSheet(Style_label_boder_MO);ui->label_111->setText("松钩"); break;
        case Tight:    ui->progressBar_11->setStyleSheet(Style_progress_risining);ui->label_11->setStyleSheet(Style_label_boder_MO);ui->label_111->setStyleSheet(Style_label_boder_MO);ui->label_111->setText("紧钩"); break;
        case Falling:  ui->progressBar_11->setStyleSheet(Style_progress_risining);ui->label_11->setStyleSheet(Style_label_boder_move);ui->label_111->setStyleSheet(Style_label_boder_move);ui->label_111->setText("下降"); break;
        case Overload: ui->progressBar_11->setStyleSheet(Style_progress_overload);ui->label_11->setStyleSheet(Style_label_boder_NO);ui->label_111->setStyleSheet(Style_label_boder_NO);ui->label_111->setText("超载"); break;
        case LOSS:     ui->progressBar_11->setStyleSheet(Style_progress_overload);ui->label_11->setStyleSheet(Style_label_boder_NO);ui->label_111->setStyleSheet(Style_label_boder_NO);ui->label_111->setText("失载"); break;
        case CurrentOver:  ui->progressBar_11->setStyleSheet(Style_progress_overload);ui->label_11->setStyleSheet(Style_label_boder_NO);ui->label_111->setStyleSheet(Style_label_boder_NO);ui->label_111->setText("电流"); break;
        case Wait:      ui->progressBar_11->setStyleSheet(Style_progress_risining);ui->label_11->setStyleSheet(Style_label_boder_MO);ui->label_111->setStyleSheet(Style_label_boder_MO);ui->label_111->setText("等待"); break;
        case Bolt:      ui->progressBar_11->setStyleSheet(Style_progress_overload);ui->label_11->setStyleSheet(Style_label_boder_NO);ui->label_111->setStyleSheet(Style_label_boder_NO);ui->label_111->setText("断线"); break;
        case Warning: ui->progressBar_11->setStyleSheet(Style_progress_warning);ui->label_11->setStyleSheet(Style_label_boder_warning);ui->label_111->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_11->setStyleSheet(Style_progress_warning);ui->label_11->setStyleSheet(Style_label_boder_warning);ui->label_111->setStyleSheet(Style_label_boder_warning);ui->label_111->setText("设置"); break;

        } ;break;
    case 11:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_12->setStyleSheet(Style_progress_risining);ui->label_12->setStyleSheet(Style_label_boder_MO);ui->label_112->setStyleSheet(Style_label_boder_MO);ui->label_112->setText("停机"); break;
        case Risining: ui->progressBar_12->setStyleSheet(Style_progress_risining);ui->label_12->setStyleSheet(Style_label_boder_move);ui->label_112->setStyleSheet(Style_label_boder_move);ui->label_112->setText("上升"); break;
        case Loose:    ui->progressBar_12->setStyleSheet(Style_progress_risining);ui->label_12->setStyleSheet(Style_label_boder_MO);ui->label_112->setStyleSheet(Style_label_boder_MO);ui->label_112->setText("松钩"); break;
        case Tight:    ui->progressBar_12->setStyleSheet(Style_progress_risining);ui->label_12->setStyleSheet(Style_label_boder_MO);ui->label_112->setStyleSheet(Style_label_boder_MO);ui->label_112->setText("紧钩"); break;
        case Falling:  ui->progressBar_12->setStyleSheet(Style_progress_risining);ui->label_12->setStyleSheet(Style_label_boder_move);ui->label_112->setStyleSheet(Style_label_boder_move);ui->label_112->setText("下降"); break;
        case Overload: ui->progressBar_12->setStyleSheet(Style_progress_overload);ui->label_12->setStyleSheet(Style_label_boder_NO);ui->label_112->setStyleSheet(Style_label_boder_NO);ui->label_112->setText("超载"); break;
        case LOSS:     ui->progressBar_12->setStyleSheet(Style_progress_overload);ui->label_12->setStyleSheet(Style_label_boder_NO);ui->label_112->setStyleSheet(Style_label_boder_NO);ui->label_112->setText("失载"); break;
        case CurrentOver:  ui->progressBar_12->setStyleSheet(Style_progress_overload);ui->label_12->setStyleSheet(Style_label_boder_NO);ui->label_112->setStyleSheet(Style_label_boder_NO);ui->label_112->setText("电流"); break;
        case Wait:      ui->progressBar_12->setStyleSheet(Style_progress_risining);ui->label_12->setStyleSheet(Style_label_boder_MO);ui->label_112->setStyleSheet(Style_label_boder_MO);ui->label_112->setText("等待"); break;
        case Bolt:      ui->progressBar_12->setStyleSheet(Style_progress_overload);ui->label_12->setStyleSheet(Style_label_boder_NO);ui->label_112->setStyleSheet(Style_label_boder_NO);ui->label_112->setText("断线"); break;
        case Warning: ui->progressBar_12->setStyleSheet(Style_progress_warning);ui->label_12->setStyleSheet(Style_label_boder_warning);ui->label_112->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_12->setStyleSheet(Style_progress_warning);ui->label_12->setStyleSheet(Style_label_boder_warning);ui->label_112->setStyleSheet(Style_label_boder_warning); ui->label_112->setText("设置");break;


        } ;break;
    case 12:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_13->setStyleSheet(Style_progress_risining);ui->label_13->setStyleSheet(Style_label_boder_MO);ui->label_113->setStyleSheet(Style_label_boder_MO);ui->label_113->setText("停机"); break;
        case Risining: ui->progressBar_13->setStyleSheet(Style_progress_risining);ui->label_13->setStyleSheet(Style_label_boder_move);ui->label_113->setStyleSheet(Style_label_boder_move);ui->label_113->setText("上升"); break;
        case Loose:    ui->progressBar_13->setStyleSheet(Style_progress_risining);ui->label_13->setStyleSheet(Style_label_boder_MO);ui->label_113->setStyleSheet(Style_label_boder_MO);ui->label_113->setText("松钩"); break;
        case Tight:    ui->progressBar_13->setStyleSheet(Style_progress_risining);ui->label_13->setStyleSheet(Style_label_boder_MO);ui->label_113->setStyleSheet(Style_label_boder_MO);ui->label_113->setText("紧钩"); break;
        case Falling:  ui->progressBar_13->setStyleSheet(Style_progress_risining);ui->label_13->setStyleSheet(Style_label_boder_move);ui->label_113->setStyleSheet(Style_label_boder_move);ui->label_113->setText("下降"); break;
        case Overload: ui->progressBar_13->setStyleSheet(Style_progress_overload);ui->label_13->setStyleSheet(Style_label_boder_NO);ui->label_113->setStyleSheet(Style_label_boder_NO);ui->label_113->setText("超载"); break;
        case LOSS:     ui->progressBar_13->setStyleSheet(Style_progress_overload);ui->label_13->setStyleSheet(Style_label_boder_NO);ui->label_113->setStyleSheet(Style_label_boder_NO);ui->label_113->setText("失载"); break;
        case CurrentOver:  ui->progressBar_13->setStyleSheet(Style_progress_overload);ui->label_13->setStyleSheet(Style_label_boder_NO);ui->label_113->setStyleSheet(Style_label_boder_NO);ui->label_113->setText("电流"); break;
        case Wait:      ui->progressBar_13->setStyleSheet(Style_progress_risining);ui->label_13->setStyleSheet(Style_label_boder_MO);ui->label_113->setStyleSheet(Style_label_boder_MO);ui->label_113->setText("等待"); break;
        case Bolt:      ui->progressBar_13->setStyleSheet(Style_progress_overload);ui->label_13->setStyleSheet(Style_label_boder_NO);ui->label_113->setStyleSheet(Style_label_boder_NO);ui->label_113->setText("断线"); break;
        case Warning: ui->progressBar_13->setStyleSheet(Style_progress_warning);ui->label_13->setStyleSheet(Style_label_boder_warning);ui->label_113->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_13->setStyleSheet(Style_progress_warning);ui->label_13->setStyleSheet(Style_label_boder_warning);ui->label_113->setStyleSheet(Style_label_boder_warning);ui->label_113->setText("设置"); break;


        } ;break;
    case 13:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_14->setStyleSheet(Style_progress_risining);ui->label_14->setStyleSheet(Style_label_boder_MO);ui->label_114->setStyleSheet(Style_label_boder_MO);ui->label_114->setText("停机"); break;
        case Risining: ui->progressBar_14->setStyleSheet(Style_progress_risining);ui->label_14->setStyleSheet(Style_label_boder_move);ui->label_114->setStyleSheet(Style_label_boder_move);ui->label_114->setText("上升"); break;
        case Loose:    ui->progressBar_14->setStyleSheet(Style_progress_risining);ui->label_14->setStyleSheet(Style_label_boder_MO);ui->label_114->setStyleSheet(Style_label_boder_MO);ui->label_114->setText("松钩"); break;
        case Tight:    ui->progressBar_14->setStyleSheet(Style_progress_risining);ui->label_14->setStyleSheet(Style_label_boder_MO);ui->label_114->setStyleSheet(Style_label_boder_MO);ui->label_114->setText("紧钩"); break;
        case Falling:  ui->progressBar_14->setStyleSheet(Style_progress_risining);ui->label_14->setStyleSheet(Style_label_boder_move);ui->label_114->setStyleSheet(Style_label_boder_move);ui->label_114->setText("下降"); break;
        case Overload: ui->progressBar_14->setStyleSheet(Style_progress_overload);ui->label_14->setStyleSheet(Style_label_boder_NO);ui->label_114->setStyleSheet(Style_label_boder_NO);ui->label_114->setText("超载"); break;
        case LOSS:     ui->progressBar_14->setStyleSheet(Style_progress_overload);ui->label_14->setStyleSheet(Style_label_boder_NO);ui->label_114->setStyleSheet(Style_label_boder_NO);ui->label_114->setText("失载"); break;
        case CurrentOver:  ui->progressBar_14->setStyleSheet(Style_progress_overload);ui->label_14->setStyleSheet(Style_label_boder_NO);ui->label_114->setStyleSheet(Style_label_boder_NO);ui->label_114->setText("电流"); break;
        case Wait:      ui->progressBar_14->setStyleSheet(Style_progress_risining);ui->label_14->setStyleSheet(Style_label_boder_MO);ui->label_114->setStyleSheet(Style_label_boder_MO);ui->label_114->setText("等待"); break;
        case Bolt:      ui->progressBar_14->setStyleSheet(Style_progress_overload);ui->label_14->setStyleSheet(Style_label_boder_NO);ui->label_114->setStyleSheet(Style_label_boder_NO);ui->label_114->setText("断线"); break;
        case Warning: ui->progressBar_14->setStyleSheet(Style_progress_warning);ui->label_14->setStyleSheet(Style_label_boder_warning);ui->label_114->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_14->setStyleSheet(Style_progress_warning);ui->label_14->setStyleSheet(Style_label_boder_warning);ui->label_114->setStyleSheet(Style_label_boder_warning);ui->label_114->setText("设置"); break;


        } ;break;
    case 14:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_15->setStyleSheet(Style_progress_risining);ui->label_15->setStyleSheet(Style_label_boder_MO);ui->label_115->setStyleSheet(Style_label_boder_MO);ui->label_115->setText("停机"); break;
        case Risining: ui->progressBar_15->setStyleSheet(Style_progress_risining);ui->label_15->setStyleSheet(Style_label_boder_move);ui->label_115->setStyleSheet(Style_label_boder_move);ui->label_115->setText("上升"); break;
        case Loose:    ui->progressBar_15->setStyleSheet(Style_progress_risining);ui->label_15->setStyleSheet(Style_label_boder_MO);ui->label_115->setStyleSheet(Style_label_boder_MO);ui->label_115->setText("松钩"); break;
        case Tight:    ui->progressBar_15->setStyleSheet(Style_progress_risining);ui->label_15->setStyleSheet(Style_label_boder_MO);ui->label_115->setStyleSheet(Style_label_boder_MO);ui->label_115->setText("紧钩"); break;
        case Falling:  ui->progressBar_15->setStyleSheet(Style_progress_risining);ui->label_15->setStyleSheet(Style_label_boder_move);ui->label_115->setStyleSheet(Style_label_boder_move);ui->label_115->setText("下降"); break;
        case Overload: ui->progressBar_15->setStyleSheet(Style_progress_overload);ui->label_15->setStyleSheet(Style_label_boder_NO);ui->label_115->setStyleSheet(Style_label_boder_NO);ui->label_115->setText("超载"); break;
        case LOSS:     ui->progressBar_15->setStyleSheet(Style_progress_overload);ui->label_15->setStyleSheet(Style_label_boder_NO);ui->label_115->setStyleSheet(Style_label_boder_NO);ui->label_115->setText("失载"); break;
        case CurrentOver:  ui->progressBar_15->setStyleSheet(Style_progress_overload);ui->label_15->setStyleSheet(Style_label_boder_NO);ui->label_115->setStyleSheet(Style_label_boder_NO);ui->label_115->setText("电流"); break;
        case Wait:      ui->progressBar_15->setStyleSheet(Style_progress_risining);ui->label_15->setStyleSheet(Style_label_boder_MO);ui->label_115->setStyleSheet(Style_label_boder_MO);ui->label_115->setText("等待"); break;
        case Bolt:      ui->progressBar_15->setStyleSheet(Style_progress_overload);ui->label_15->setStyleSheet(Style_label_boder_NO);ui->label_115->setStyleSheet(Style_label_boder_NO);ui->label_115->setText("断线"); break;
        case Warning: ui->progressBar_15->setStyleSheet(Style_progress_warning);ui->label_15->setStyleSheet(Style_label_boder_warning);ui->label_115->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_15->setStyleSheet(Style_progress_warning);ui->label_15->setStyleSheet(Style_label_boder_warning);ui->label_115->setStyleSheet(Style_label_boder_warning);ui->label_115->setText("设置"); break;


        } ;break;
    case 15:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_16->setStyleSheet(Style_progress_risining);ui->label_16->setStyleSheet(Style_label_boder_MO);ui->label_116->setStyleSheet(Style_label_boder_MO);ui->label_116->setText("停机"); break;
        case Risining: ui->progressBar_16->setStyleSheet(Style_progress_risining);ui->label_16->setStyleSheet(Style_label_boder_move);ui->label_116->setStyleSheet(Style_label_boder_move);ui->label_116->setText("上升"); break;
        case Loose:    ui->progressBar_16->setStyleSheet(Style_progress_risining);ui->label_16->setStyleSheet(Style_label_boder_MO);ui->label_116->setStyleSheet(Style_label_boder_MO);ui->label_116->setText("松钩"); break;
        case Tight:    ui->progressBar_16->setStyleSheet(Style_progress_risining);ui->label_16->setStyleSheet(Style_label_boder_MO);ui->label_116->setStyleSheet(Style_label_boder_MO);ui->label_116->setText("紧钩"); break;
        case Falling:  ui->progressBar_16->setStyleSheet(Style_progress_risining);ui->label_16->setStyleSheet(Style_label_boder_move);ui->label_116->setStyleSheet(Style_label_boder_move);ui->label_116->setText("下降"); break;
        case Overload: ui->progressBar_16->setStyleSheet(Style_progress_overload);ui->label_16->setStyleSheet(Style_label_boder_NO);ui->label_116->setStyleSheet(Style_label_boder_NO);ui->label_116->setText("超载"); break;
        case LOSS:     ui->progressBar_16->setStyleSheet(Style_progress_overload);ui->label_16->setStyleSheet(Style_label_boder_NO);ui->label_116->setStyleSheet(Style_label_boder_NO);ui->label_116->setText("失载"); break;
        case CurrentOver:  ui->progressBar_16->setStyleSheet(Style_progress_overload);ui->label_16->setStyleSheet(Style_label_boder_NO);ui->label_116->setStyleSheet(Style_label_boder_NO);ui->label_116->setText("电流"); break;
        case Wait:      ui->progressBar_16->setStyleSheet(Style_progress_risining);ui->label_16->setStyleSheet(Style_label_boder_MO);ui->label_116->setStyleSheet(Style_label_boder_MO);ui->label_116->setText("等待"); break;
        case Bolt:      ui->progressBar_16->setStyleSheet(Style_progress_overload);ui->label_16->setStyleSheet(Style_label_boder_NO);ui->label_116->setStyleSheet(Style_label_boder_NO);ui->label_116->setText("断线"); break;
        case Warning: ui->progressBar_16->setStyleSheet(Style_progress_warning);ui->label_16->setStyleSheet(Style_label_boder_warning);ui->label_116->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_16->setStyleSheet(Style_progress_warning);ui->label_16->setStyleSheet(Style_label_boder_warning);ui->label_116->setStyleSheet(Style_label_boder_warning);ui->label_116->setText("设置"); break;

        } ;break;
    case 16:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_17->setStyleSheet(Style_progress_risining);ui->label_17->setStyleSheet(Style_label_boder_MO);ui->label_117->setStyleSheet(Style_label_boder_MO);ui->label_117->setText("停机"); break;
        case Risining: ui->progressBar_17->setStyleSheet(Style_progress_risining);ui->label_17->setStyleSheet(Style_label_boder_move);ui->label_117->setStyleSheet(Style_label_boder_move);ui->label_117->setText("上升"); break;
        case Loose:    ui->progressBar_17->setStyleSheet(Style_progress_risining);ui->label_17->setStyleSheet(Style_label_boder_MO);ui->label_117->setStyleSheet(Style_label_boder_MO);ui->label_117->setText("松钩"); break;
        case Tight:    ui->progressBar_17->setStyleSheet(Style_progress_risining);ui->label_17->setStyleSheet(Style_label_boder_MO);ui->label_117->setStyleSheet(Style_label_boder_MO);ui->label_117->setText("紧钩"); break;
        case Falling:  ui->progressBar_17->setStyleSheet(Style_progress_risining);ui->label_17->setStyleSheet(Style_label_boder_move);ui->label_117->setStyleSheet(Style_label_boder_move);ui->label_117->setText("下降"); break;
        case Overload: ui->progressBar_17->setStyleSheet(Style_progress_overload);ui->label_17->setStyleSheet(Style_label_boder_NO);ui->label_117->setStyleSheet(Style_label_boder_NO);ui->label_117->setText("超载"); break;
        case LOSS:     ui->progressBar_17->setStyleSheet(Style_progress_overload);ui->label_17->setStyleSheet(Style_label_boder_NO);ui->label_117->setStyleSheet(Style_label_boder_NO);ui->label_117->setText("失载"); break;
        case CurrentOver:  ui->progressBar_17->setStyleSheet(Style_progress_overload);ui->label_17->setStyleSheet(Style_label_boder_NO);ui->label_117->setStyleSheet(Style_label_boder_NO);ui->label_117->setText("电流"); break;
        case Wait:      ui->progressBar_17->setStyleSheet(Style_progress_risining);ui->label_17->setStyleSheet(Style_label_boder_MO);ui->label_117->setStyleSheet(Style_label_boder_MO);ui->label_117->setText("等待"); break;
        case Bolt:      ui->progressBar_17->setStyleSheet(Style_progress_overload);ui->label_17->setStyleSheet(Style_label_boder_NO);ui->label_117->setStyleSheet(Style_label_boder_NO);ui->label_117->setText("断线"); break;
        case Warning: ui->progressBar_17->setStyleSheet(Style_progress_warning);ui->label_17->setStyleSheet(Style_label_boder_warning);ui->label_117->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_17->setStyleSheet(Style_progress_warning);ui->label_17->setStyleSheet(Style_label_boder_warning);ui->label_117->setStyleSheet(Style_label_boder_warning);ui->label_117->setText("设置"); break;

        } ;break;
    case 17:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_18->setStyleSheet(Style_progress_risining);ui->label_18->setStyleSheet(Style_label_boder_MO);ui->label_118->setStyleSheet(Style_label_boder_MO);ui->label_118->setText("停机"); break;
        case Risining: ui->progressBar_18->setStyleSheet(Style_progress_risining);ui->label_18->setStyleSheet(Style_label_boder_move);ui->label_118->setStyleSheet(Style_label_boder_move);ui->label_118->setText("上升"); break;
        case Loose:    ui->progressBar_18->setStyleSheet(Style_progress_risining);ui->label_18->setStyleSheet(Style_label_boder_MO);ui->label_118->setStyleSheet(Style_label_boder_MO);ui->label_118->setText("松钩"); break;
        case Tight:    ui->progressBar_18->setStyleSheet(Style_progress_risining);ui->label_18->setStyleSheet(Style_label_boder_MO);ui->label_118->setStyleSheet(Style_label_boder_MO);ui->label_118->setText("紧钩"); break;
        case Falling:  ui->progressBar_18->setStyleSheet(Style_progress_risining);ui->label_18->setStyleSheet(Style_label_boder_move);ui->label_118->setStyleSheet(Style_label_boder_move);ui->label_118->setText("下降"); break;
        case Overload: ui->progressBar_18->setStyleSheet(Style_progress_overload);ui->label_18->setStyleSheet(Style_label_boder_NO);ui->label_118->setStyleSheet(Style_label_boder_NO);ui->label_118->setText("超载"); break;
        case LOSS:     ui->progressBar_18->setStyleSheet(Style_progress_overload);ui->label_18->setStyleSheet(Style_label_boder_NO);ui->label_118->setStyleSheet(Style_label_boder_NO);ui->label_118->setText("失载"); break;
        case CurrentOver:  ui->progressBar_18->setStyleSheet(Style_progress_overload);ui->label_18->setStyleSheet(Style_label_boder_NO);ui->label_118->setStyleSheet(Style_label_boder_NO);ui->label_118->setText("电流"); break;
        case Wait:      ui->progressBar_18->setStyleSheet(Style_progress_risining);ui->label_18->setStyleSheet(Style_label_boder_MO);ui->label_118->setStyleSheet(Style_label_boder_MO);ui->label_118->setText("等待"); break;
        case Bolt:      ui->progressBar_18->setStyleSheet(Style_progress_overload);ui->label_18->setStyleSheet(Style_label_boder_NO);ui->label_118->setStyleSheet(Style_label_boder_NO);ui->label_118->setText("断线"); break;
        case Warning: ui->progressBar_18->setStyleSheet(Style_progress_warning);ui->label_18->setStyleSheet(Style_label_boder_warning);ui->label_118->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_18->setStyleSheet(Style_progress_warning);ui->label_18->setStyleSheet(Style_label_boder_warning);ui->label_118->setStyleSheet(Style_label_boder_warning);ui->label_118->setText("设置"); break;


        } ;break;
    case 18:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_19->setStyleSheet(Style_progress_risining);ui->label_19->setStyleSheet(Style_label_boder_MO);ui->label_119->setStyleSheet(Style_label_boder_MO);ui->label_119->setText("停机"); break;
        case Risining: ui->progressBar_19->setStyleSheet(Style_progress_risining);ui->label_19->setStyleSheet(Style_label_boder_move);ui->label_119->setStyleSheet(Style_label_boder_move);ui->label_119->setText("上升"); break;
        case Loose:    ui->progressBar_19->setStyleSheet(Style_progress_risining);ui->label_19->setStyleSheet(Style_label_boder_MO);ui->label_119->setStyleSheet(Style_label_boder_MO);ui->label_119->setText("松钩"); break;
        case Tight:    ui->progressBar_19->setStyleSheet(Style_progress_risining);ui->label_19->setStyleSheet(Style_label_boder_MO);ui->label_119->setStyleSheet(Style_label_boder_MO);ui->label_119->setText("紧钩"); break;
        case Falling:  ui->progressBar_19->setStyleSheet(Style_progress_risining);ui->label_19->setStyleSheet(Style_label_boder_move);ui->label_119->setStyleSheet(Style_label_boder_move);ui->label_119->setText("下降"); break;
        case Overload: ui->progressBar_19->setStyleSheet(Style_progress_overload);ui->label_19->setStyleSheet(Style_label_boder_NO);ui->label_119->setStyleSheet(Style_label_boder_NO);ui->label_119->setText("超载"); break;
        case LOSS:     ui->progressBar_19->setStyleSheet(Style_progress_overload);ui->label_19->setStyleSheet(Style_label_boder_NO);ui->label_119->setStyleSheet(Style_label_boder_NO);ui->label_119->setText("失载"); break;
        case CurrentOver:  ui->progressBar_19->setStyleSheet(Style_progress_overload);ui->label_19->setStyleSheet(Style_label_boder_NO);ui->label_119->setStyleSheet(Style_label_boder_NO);ui->label_119->setText("电流"); break;
        case Wait:      ui->progressBar_19->setStyleSheet(Style_progress_risining);ui->label_19->setStyleSheet(Style_label_boder_MO);ui->label_119->setStyleSheet(Style_label_boder_MO);ui->label_119->setText("等待"); break;
        case Bolt:      ui->progressBar_19->setStyleSheet(Style_progress_overload);ui->label_19->setStyleSheet(Style_label_boder_NO);ui->label_119->setStyleSheet(Style_label_boder_NO);ui->label_119->setText("断线"); break;
        case Warning: ui->progressBar_19->setStyleSheet(Style_progress_warning);ui->label_19->setStyleSheet(Style_label_boder_warning);ui->label_119->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_19->setStyleSheet(Style_progress_warning);ui->label_19->setStyleSheet(Style_label_boder_warning);ui->label_119->setStyleSheet(Style_label_boder_warning);ui->label_119->setText("设置"); break;


        } ;break;
    case 19:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_20->setStyleSheet(Style_progress_risining);ui->label_20->setStyleSheet(Style_label_boder_MO);ui->label_120->setStyleSheet(Style_label_boder_MO);ui->label_120->setText("停机"); break;
        case Risining: ui->progressBar_20->setStyleSheet(Style_progress_risining);ui->label_20->setStyleSheet(Style_label_boder_move);ui->label_120->setStyleSheet(Style_label_boder_move);ui->label_120->setText("上升"); break;
        case Loose:    ui->progressBar_20->setStyleSheet(Style_progress_risining);ui->label_20->setStyleSheet(Style_label_boder_MO);ui->label_120->setStyleSheet(Style_label_boder_MO);ui->label_120->setText("松钩"); break;
        case Tight:    ui->progressBar_20->setStyleSheet(Style_progress_risining);ui->label_20->setStyleSheet(Style_label_boder_MO);ui->label_120->setStyleSheet(Style_label_boder_MO);ui->label_120->setText("紧钩"); break;
        case Falling:  ui->progressBar_20->setStyleSheet(Style_progress_risining);ui->label_20->setStyleSheet(Style_label_boder_move);ui->label_120->setStyleSheet(Style_label_boder_move);ui->label_120->setText("下降"); break;
        case Overload: ui->progressBar_20->setStyleSheet(Style_progress_overload);ui->label_20->setStyleSheet(Style_label_boder_NO);ui->label_120->setStyleSheet(Style_label_boder_NO);ui->label_120->setText("超载"); break;
        case LOSS:     ui->progressBar_20->setStyleSheet(Style_progress_overload);ui->label_20->setStyleSheet(Style_label_boder_NO);ui->label_120->setStyleSheet(Style_label_boder_NO);ui->label_120->setText("失载"); break;
        case CurrentOver:  ui->progressBar_20->setStyleSheet(Style_progress_overload);ui->label_20->setStyleSheet(Style_label_boder_NO);ui->label_120->setStyleSheet(Style_label_boder_NO);ui->label_120->setText("电流"); break;
        case Wait:      ui->progressBar_20->setStyleSheet(Style_progress_risining);ui->label_20->setStyleSheet(Style_label_boder_MO);ui->label_120->setStyleSheet(Style_label_boder_MO);ui->label_120->setText("等待"); break;
        case Bolt:      ui->progressBar_20->setStyleSheet(Style_progress_overload);ui->label_20->setStyleSheet(Style_label_boder_NO);ui->label_120->setStyleSheet(Style_label_boder_NO);ui->label_120->setText("断线"); break;
        case Warning: ui->progressBar_20->setStyleSheet(Style_progress_warning);ui->label_20->setStyleSheet(Style_label_boder_warning);ui->label_120->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_20->setStyleSheet(Style_progress_warning);ui->label_20->setStyleSheet(Style_label_boder_warning);ui->label_120->setStyleSheet(Style_label_boder_warning);ui->label_120->setText("设置"); break;


        } ;break;
    case 20:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_21->setStyleSheet(Style_progress_risining);ui->label_21->setStyleSheet(Style_label_boder_MO);ui->label_121->setStyleSheet(Style_label_boder_MO);ui->label_121->setText("停机"); break;
        case Risining: ui->progressBar_21->setStyleSheet(Style_progress_risining);ui->label_21->setStyleSheet(Style_label_boder_move);ui->label_121->setStyleSheet(Style_label_boder_move);ui->label_121->setText("上升"); break;
        case Loose:    ui->progressBar_21->setStyleSheet(Style_progress_risining);ui->label_21->setStyleSheet(Style_label_boder_MO);ui->label_121->setStyleSheet(Style_label_boder_MO);ui->label_121->setText("松钩"); break;
        case Tight:    ui->progressBar_21->setStyleSheet(Style_progress_risining);ui->label_21->setStyleSheet(Style_label_boder_MO);ui->label_121->setStyleSheet(Style_label_boder_MO);ui->label_121->setText("紧钩"); break;
        case Falling:  ui->progressBar_21->setStyleSheet(Style_progress_risining);ui->label_21->setStyleSheet(Style_label_boder_move);ui->label_121->setStyleSheet(Style_label_boder_move);ui->label_121->setText("下降"); break;
        case Overload: ui->progressBar_21->setStyleSheet(Style_progress_overload);ui->label_21->setStyleSheet(Style_label_boder_NO);ui->label_121->setStyleSheet(Style_label_boder_NO);ui->label_121->setText("超载"); break;
        case LOSS:     ui->progressBar_21->setStyleSheet(Style_progress_overload);ui->label_21->setStyleSheet(Style_label_boder_NO);ui->label_121->setStyleSheet(Style_label_boder_NO);ui->label_121->setText("失载"); break;
        case CurrentOver:  ui->progressBar_21->setStyleSheet(Style_progress_overload);ui->label_21->setStyleSheet(Style_label_boder_NO);ui->label_121->setStyleSheet(Style_label_boder_NO);ui->label_121->setText("电流"); break;
        case Wait:      ui->progressBar_21->setStyleSheet(Style_progress_risining);ui->label_21->setStyleSheet(Style_label_boder_MO);ui->label_121->setStyleSheet(Style_label_boder_MO);ui->label_121->setText("等待"); break;
        case Bolt:      ui->progressBar_21->setStyleSheet(Style_progress_overload);ui->label_21->setStyleSheet(Style_label_boder_NO);ui->label_121->setStyleSheet(Style_label_boder_NO);ui->label_121->setText("断线"); break;
        case Warning: ui->progressBar_21->setStyleSheet(Style_progress_warning);ui->label_21->setStyleSheet(Style_label_boder_warning);ui->label_121->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_21->setStyleSheet(Style_progress_warning);ui->label_21->setStyleSheet(Style_label_boder_warning);ui->label_121->setStyleSheet(Style_label_boder_warning);ui->label_121->setText("设置"); break;


        } ;break;
    case 21:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_22->setStyleSheet(Style_progress_risining);ui->label_22->setStyleSheet(Style_label_boder_MO);ui->label_122->setStyleSheet(Style_label_boder_MO);ui->label_122->setText("停机"); break;
        case Risining: ui->progressBar_22->setStyleSheet(Style_progress_risining);ui->label_22->setStyleSheet(Style_label_boder_move);ui->label_122->setStyleSheet(Style_label_boder_move);ui->label_122->setText("上升"); break;
        case Loose:    ui->progressBar_22->setStyleSheet(Style_progress_risining);ui->label_22->setStyleSheet(Style_label_boder_MO);ui->label_122->setStyleSheet(Style_label_boder_MO);ui->label_122->setText("松钩"); break;
        case Tight:    ui->progressBar_22->setStyleSheet(Style_progress_risining);ui->label_22->setStyleSheet(Style_label_boder_MO);ui->label_122->setStyleSheet(Style_label_boder_MO);ui->label_122->setText("紧钩"); break;
        case Falling:  ui->progressBar_22->setStyleSheet(Style_progress_risining);ui->label_22->setStyleSheet(Style_label_boder_move);ui->label_122->setStyleSheet(Style_label_boder_move);ui->label_122->setText("下降"); break;
        case Overload: ui->progressBar_22->setStyleSheet(Style_progress_overload);ui->label_22->setStyleSheet(Style_label_boder_NO);ui->label_122->setStyleSheet(Style_label_boder_NO);ui->label_122->setText("超载"); break;
        case LOSS:     ui->progressBar_22->setStyleSheet(Style_progress_overload);ui->label_22->setStyleSheet(Style_label_boder_NO);ui->label_122->setStyleSheet(Style_label_boder_NO);ui->label_122->setText("失载"); break;
        case CurrentOver:  ui->progressBar_22->setStyleSheet(Style_progress_overload);ui->label_22->setStyleSheet(Style_label_boder_NO);ui->label_122->setStyleSheet(Style_label_boder_NO);ui->label_122->setText("电流"); break;
        case Wait:      ui->progressBar_22->setStyleSheet(Style_progress_risining);ui->label_22->setStyleSheet(Style_label_boder_MO);ui->label_122->setStyleSheet(Style_label_boder_MO);ui->label_122->setText("等待"); break;
        case Bolt:      ui->progressBar_22->setStyleSheet(Style_progress_overload);ui->label_22->setStyleSheet(Style_label_boder_NO);ui->label_122->setStyleSheet(Style_label_boder_NO);ui->label_122->setText("断线"); break;
        case Warning: ui->progressBar_22->setStyleSheet(Style_progress_warning);ui->label_22->setStyleSheet(Style_label_boder_warning);ui->label_122->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_22->setStyleSheet(Style_progress_warning);ui->label_22->setStyleSheet(Style_label_boder_warning);ui->label_122->setStyleSheet(Style_label_boder_warning);ui->label_122->setText("设置"); break;


        } ;break;
    case 22:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_23->setStyleSheet(Style_progress_risining);ui->label_23->setStyleSheet(Style_label_boder_MO);ui->label_123->setStyleSheet(Style_label_boder_MO);ui->label_123->setText("停机"); break;
        case Risining: ui->progressBar_23->setStyleSheet(Style_progress_risining);ui->label_23->setStyleSheet(Style_label_boder_move);ui->label_123->setStyleSheet(Style_label_boder_move);ui->label_123->setText("上升"); break;
        case Loose:    ui->progressBar_23->setStyleSheet(Style_progress_risining);ui->label_23->setStyleSheet(Style_label_boder_MO);ui->label_123->setStyleSheet(Style_label_boder_MO);ui->label_123->setText("松钩"); break;
        case Tight:    ui->progressBar_23->setStyleSheet(Style_progress_risining);ui->label_23->setStyleSheet(Style_label_boder_MO);ui->label_123->setStyleSheet(Style_label_boder_MO);ui->label_123->setText("紧钩"); break;
        case Falling:  ui->progressBar_23->setStyleSheet(Style_progress_risining);ui->label_23->setStyleSheet(Style_label_boder_move);ui->label_123->setStyleSheet(Style_label_boder_move);ui->label_123->setText("下降"); break;
        case Overload: ui->progressBar_23->setStyleSheet(Style_progress_overload);ui->label_23->setStyleSheet(Style_label_boder_NO);ui->label_123->setStyleSheet(Style_label_boder_NO);ui->label_123->setText("超载"); break;
        case LOSS:     ui->progressBar_23->setStyleSheet(Style_progress_overload);ui->label_23->setStyleSheet(Style_label_boder_NO);ui->label_123->setStyleSheet(Style_label_boder_NO);ui->label_123->setText("失载"); break;
        case CurrentOver:  ui->progressBar_23->setStyleSheet(Style_progress_overload);ui->label_23->setStyleSheet(Style_label_boder_NO);ui->label_123->setStyleSheet(Style_label_boder_NO);ui->label_123->setText("电流"); break;
        case Wait:      ui->progressBar_23->setStyleSheet(Style_progress_risining);ui->label_23->setStyleSheet(Style_label_boder_MO);ui->label_123->setStyleSheet(Style_label_boder_MO);ui->label_123->setText("等待"); break;
        case Bolt:      ui->progressBar_23->setStyleSheet(Style_progress_overload);ui->label_23->setStyleSheet(Style_label_boder_NO);ui->label_123->setStyleSheet(Style_label_boder_NO);ui->label_123->setText("断线"); break;
        case Warning: ui->progressBar_23->setStyleSheet(Style_progress_warning);ui->label_23->setStyleSheet(Style_label_boder_warning);ui->label_123->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_23->setStyleSheet(Style_progress_warning);ui->label_23->setStyleSheet(Style_label_boder_warning);ui->label_123->setStyleSheet(Style_label_boder_warning);ui->label_123->setText("设置"); break;

        } ;break;
    case 23:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_24->setStyleSheet(Style_progress_risining);ui->label_24->setStyleSheet(Style_label_boder_MO);ui->label_124->setStyleSheet(Style_label_boder_MO);ui->label_124->setText("停机"); break;
        case Risining: ui->progressBar_24->setStyleSheet(Style_progress_risining);ui->label_24->setStyleSheet(Style_label_boder_move);ui->label_124->setStyleSheet(Style_label_boder_move);ui->label_124->setText("上升"); break;
        case Loose:    ui->progressBar_24->setStyleSheet(Style_progress_risining);ui->label_24->setStyleSheet(Style_label_boder_MO);ui->label_124->setStyleSheet(Style_label_boder_MO);ui->label_124->setText("松钩"); break;
        case Tight:    ui->progressBar_24->setStyleSheet(Style_progress_risining);ui->label_24->setStyleSheet(Style_label_boder_MO);ui->label_124->setStyleSheet(Style_label_boder_MO);ui->label_124->setText("紧钩"); break;
        case Falling:  ui->progressBar_24->setStyleSheet(Style_progress_risining);ui->label_24->setStyleSheet(Style_label_boder_move);ui->label_124->setStyleSheet(Style_label_boder_move);ui->label_124->setText("下降"); break;
        case Overload: ui->progressBar_24->setStyleSheet(Style_progress_overload);ui->label_24->setStyleSheet(Style_label_boder_NO);ui->label_124->setStyleSheet(Style_label_boder_NO);ui->label_124->setText("超载"); break;
        case LOSS:     ui->progressBar_24->setStyleSheet(Style_progress_overload);ui->label_24->setStyleSheet(Style_label_boder_NO);ui->label_124->setStyleSheet(Style_label_boder_NO);ui->label_124->setText("失载"); break;
        case CurrentOver:  ui->progressBar_24->setStyleSheet(Style_progress_overload);ui->label_24->setStyleSheet(Style_label_boder_NO);ui->label_124->setStyleSheet(Style_label_boder_NO);ui->label_124->setText("电流"); break;
        case Wait:      ui->progressBar_24->setStyleSheet(Style_progress_risining);ui->label_24->setStyleSheet(Style_label_boder_MO);ui->label_124->setStyleSheet(Style_label_boder_MO);ui->label_124->setText("等待"); break;
        case Bolt:      ui->progressBar_24->setStyleSheet(Style_progress_overload);ui->label_24->setStyleSheet(Style_label_boder_NO);ui->label_124->setStyleSheet(Style_label_boder_NO);ui->label_124->setText("断线"); break;
        case Warning: ui->progressBar_24->setStyleSheet(Style_progress_warning);ui->label_24->setStyleSheet(Style_label_boder_warning);ui->label_124->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_24->setStyleSheet(Style_progress_warning);ui->label_24->setStyleSheet(Style_label_boder_warning);ui->label_124->setStyleSheet(Style_label_boder_warning);ui->label_124->setText("设置"); break;


        } ;break;
    case 24:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_25->setStyleSheet(Style_progress_risining);ui->label_25->setStyleSheet(Style_label_boder_MO);ui->label_125->setStyleSheet(Style_label_boder_MO);ui->label_125->setText("停机"); break;
        case Risining: ui->progressBar_25->setStyleSheet(Style_progress_risining);ui->label_25->setStyleSheet(Style_label_boder_move);ui->label_125->setStyleSheet(Style_label_boder_move);ui->label_125->setText("上升"); break;
        case Loose:    ui->progressBar_25->setStyleSheet(Style_progress_risining);ui->label_25->setStyleSheet(Style_label_boder_MO);ui->label_125->setStyleSheet(Style_label_boder_MO);ui->label_125->setText("松钩"); break;
        case Tight:    ui->progressBar_25->setStyleSheet(Style_progress_risining);ui->label_25->setStyleSheet(Style_label_boder_MO);ui->label_125->setStyleSheet(Style_label_boder_MO);ui->label_125->setText("紧钩"); break;
        case Falling:  ui->progressBar_25->setStyleSheet(Style_progress_risining);ui->label_25->setStyleSheet(Style_label_boder_move);ui->label_125->setStyleSheet(Style_label_boder_move);ui->label_125->setText("下降"); break;
        case Overload: ui->progressBar_25->setStyleSheet(Style_progress_overload);ui->label_25->setStyleSheet(Style_label_boder_NO);ui->label_125->setStyleSheet(Style_label_boder_NO);ui->label_125->setText("超载"); break;
        case LOSS:     ui->progressBar_25->setStyleSheet(Style_progress_overload);ui->label_25->setStyleSheet(Style_label_boder_NO);ui->label_125->setStyleSheet(Style_label_boder_NO);ui->label_125->setText("失载"); break;
        case CurrentOver:  ui->progressBar_25->setStyleSheet(Style_progress_overload);ui->label_25->setStyleSheet(Style_label_boder_NO);ui->label_125->setStyleSheet(Style_label_boder_NO);ui->label_125->setText("电流"); break;
        case Wait:      ui->progressBar_25->setStyleSheet(Style_progress_risining);ui->label_25->setStyleSheet(Style_label_boder_MO);ui->label_125->setStyleSheet(Style_label_boder_MO);ui->label_125->setText("等待"); break;
        case Bolt:      ui->progressBar_25->setStyleSheet(Style_progress_overload);ui->label_25->setStyleSheet(Style_label_boder_NO);ui->label_125->setStyleSheet(Style_label_boder_NO);ui->label_125->setText("断线"); break;
        case Warning: ui->progressBar_25->setStyleSheet(Style_progress_warning);ui->label_25->setStyleSheet(Style_label_boder_warning);ui->label_125->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_25->setStyleSheet(Style_progress_warning);ui->label_25->setStyleSheet(Style_label_boder_warning);ui->label_125->setStyleSheet(Style_label_boder_warning);ui->label_125->setText("设置"); break;

        } ;break;
    case 25:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_26->setStyleSheet(Style_progress_risining);ui->label_26->setStyleSheet(Style_label_boder_MO);ui->label_126->setStyleSheet(Style_label_boder_MO);ui->label_126->setText("停机"); break;
        case Risining: ui->progressBar_26->setStyleSheet(Style_progress_risining);ui->label_26->setStyleSheet(Style_label_boder_move);ui->label_126->setStyleSheet(Style_label_boder_move);ui->label_126->setText("上升"); break;
        case Loose:    ui->progressBar_26->setStyleSheet(Style_progress_risining);ui->label_26->setStyleSheet(Style_label_boder_MO);ui->label_126->setStyleSheet(Style_label_boder_MO);ui->label_126->setText("松钩"); break;
        case Tight:    ui->progressBar_26->setStyleSheet(Style_progress_risining);ui->label_26->setStyleSheet(Style_label_boder_MO);ui->label_126->setStyleSheet(Style_label_boder_MO);ui->label_126->setText("紧钩"); break;
        case Falling:  ui->progressBar_26->setStyleSheet(Style_progress_risining);ui->label_26->setStyleSheet(Style_label_boder_move);ui->label_126->setStyleSheet(Style_label_boder_move);ui->label_126->setText("下降"); break;
        case Overload: ui->progressBar_26->setStyleSheet(Style_progress_overload);ui->label_26->setStyleSheet(Style_label_boder_NO);ui->label_126->setStyleSheet(Style_label_boder_NO);ui->label_126->setText("超载"); break;
        case LOSS:     ui->progressBar_26->setStyleSheet(Style_progress_overload);ui->label_26->setStyleSheet(Style_label_boder_NO);ui->label_126->setStyleSheet(Style_label_boder_NO);ui->label_126->setText("失载"); break;
        case CurrentOver:  ui->progressBar_26->setStyleSheet(Style_progress_overload);ui->label_26->setStyleSheet(Style_label_boder_NO);ui->label_126->setStyleSheet(Style_label_boder_NO);ui->label_126->setText("电流"); break;
        case Wait:      ui->progressBar_26->setStyleSheet(Style_progress_risining);ui->label_26->setStyleSheet(Style_label_boder_MO);ui->label_126->setStyleSheet(Style_label_boder_MO);ui->label_126->setText("等待"); break;
        case Bolt:      ui->progressBar_26->setStyleSheet(Style_progress_overload);ui->label_26->setStyleSheet(Style_label_boder_NO);ui->label_126->setStyleSheet(Style_label_boder_NO);ui->label_126->setText("断线"); break;
        case Warning: ui->progressBar_26->setStyleSheet(Style_progress_warning);ui->label_26->setStyleSheet(Style_label_boder_warning);ui->label_126->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_26->setStyleSheet(Style_progress_warning);ui->label_26->setStyleSheet(Style_label_boder_warning);ui->label_126->setStyleSheet(Style_label_boder_warning);ui->label_126->setText("设置"); break;

        } ;break;
    case 26:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_27->setStyleSheet(Style_progress_risining);ui->label_27->setStyleSheet(Style_label_boder_MO);ui->label_127->setStyleSheet(Style_label_boder_MO);ui->label_127->setText("停机"); break;
        case Risining: ui->progressBar_27->setStyleSheet(Style_progress_risining);ui->label_27->setStyleSheet(Style_label_boder_move);ui->label_127->setStyleSheet(Style_label_boder_move);ui->label_127->setText("上升"); break;
        case Loose:    ui->progressBar_27->setStyleSheet(Style_progress_risining);ui->label_27->setStyleSheet(Style_label_boder_MO);ui->label_127->setStyleSheet(Style_label_boder_MO);ui->label_127->setText("松钩"); break;
        case Tight:    ui->progressBar_27->setStyleSheet(Style_progress_risining);ui->label_27->setStyleSheet(Style_label_boder_MO);ui->label_127->setStyleSheet(Style_label_boder_MO);ui->label_127->setText("紧钩"); break;
        case Falling:  ui->progressBar_27->setStyleSheet(Style_progress_risining);ui->label_27->setStyleSheet(Style_label_boder_move);ui->label_127->setStyleSheet(Style_label_boder_move);ui->label_127->setText("下降"); break;
        case Overload: ui->progressBar_27->setStyleSheet(Style_progress_overload);ui->label_27->setStyleSheet(Style_label_boder_NO);ui->label_127->setStyleSheet(Style_label_boder_NO);ui->label_127->setText("超载"); break;
        case LOSS:     ui->progressBar_27->setStyleSheet(Style_progress_overload);ui->label_27->setStyleSheet(Style_label_boder_NO);ui->label_127->setStyleSheet(Style_label_boder_NO);ui->label_127->setText("失载"); break;
        case CurrentOver:  ui->progressBar_27->setStyleSheet(Style_progress_overload);ui->label_27->setStyleSheet(Style_label_boder_NO);ui->label_127->setStyleSheet(Style_label_boder_NO);ui->label_127->setText("电流"); break;
        case Wait:      ui->progressBar_27->setStyleSheet(Style_progress_risining);ui->label_27->setStyleSheet(Style_label_boder_MO);ui->label_127->setStyleSheet(Style_label_boder_MO);ui->label_127->setText("等待"); break;
        case Bolt:      ui->progressBar_27->setStyleSheet(Style_progress_overload);ui->label_27->setStyleSheet(Style_label_boder_NO);ui->label_127->setStyleSheet(Style_label_boder_NO);ui->label_127->setText("断线"); break;
        case Warning: ui->progressBar_27->setStyleSheet(Style_progress_warning);ui->label_27->setStyleSheet(Style_label_boder_warning);ui->label_127->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_27->setStyleSheet(Style_progress_warning);ui->label_27->setStyleSheet(Style_label_boder_warning);ui->label_127->setStyleSheet(Style_label_boder_warning);ui->label_127->setText("设置"); break;

        } ;break;

    case 27:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_28->setStyleSheet(Style_progress_risining);ui->label_28->setStyleSheet(Style_label_boder_MO);ui->label_128->setStyleSheet(Style_label_boder_MO);ui->label_128->setText("停机"); break;
        case Risining: ui->progressBar_28->setStyleSheet(Style_progress_risining);ui->label_28->setStyleSheet(Style_label_boder_move);ui->label_128->setStyleSheet(Style_label_boder_move);ui->label_128->setText("上升"); break;
        case Loose:    ui->progressBar_28->setStyleSheet(Style_progress_risining);ui->label_28->setStyleSheet(Style_label_boder_MO);ui->label_128->setStyleSheet(Style_label_boder_MO);ui->label_128->setText("松钩"); break;
        case Tight:    ui->progressBar_28->setStyleSheet(Style_progress_risining);ui->label_28->setStyleSheet(Style_label_boder_MO);ui->label_128->setStyleSheet(Style_label_boder_MO);ui->label_128->setText("紧钩"); break;
        case Falling:  ui->progressBar_28->setStyleSheet(Style_progress_risining);ui->label_28->setStyleSheet(Style_label_boder_move);ui->label_128->setStyleSheet(Style_label_boder_move);ui->label_128->setText("下降"); break;
        case Overload: ui->progressBar_28->setStyleSheet(Style_progress_overload);ui->label_28->setStyleSheet(Style_label_boder_NO);ui->label_128->setStyleSheet(Style_label_boder_NO);ui->label_128->setText("超载"); break;
        case LOSS:     ui->progressBar_28->setStyleSheet(Style_progress_overload);ui->label_28->setStyleSheet(Style_label_boder_NO);ui->label_128->setStyleSheet(Style_label_boder_NO);ui->label_128->setText("失载"); break;
        case CurrentOver:  ui->progressBar_28->setStyleSheet(Style_progress_overload);ui->label_28->setStyleSheet(Style_label_boder_NO);ui->label_128->setStyleSheet(Style_label_boder_NO);ui->label_128->setText("电流"); break;
        case Wait:      ui->progressBar_28->setStyleSheet(Style_progress_risining);ui->label_28->setStyleSheet(Style_label_boder_MO);ui->label_128->setStyleSheet(Style_label_boder_MO);ui->label_128->setText("等待"); break;
        case Bolt:      ui->progressBar_28->setStyleSheet(Style_progress_overload);ui->label_28->setStyleSheet(Style_label_boder_NO);ui->label_128->setStyleSheet(Style_label_boder_NO);ui->label_128->setText("断线"); break;
        case Warning: ui->progressBar_28->setStyleSheet(Style_progress_warning);ui->label_28->setStyleSheet(Style_label_boder_warning);ui->label_128->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_28->setStyleSheet(Style_progress_warning);ui->label_28->setStyleSheet(Style_label_boder_warning);ui->label_128->setStyleSheet(Style_label_boder_warning);ui->label_128->setText("设置"); break;

        } ;break;
    case 28:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_29->setStyleSheet(Style_progress_risining);ui->label_29->setStyleSheet(Style_label_boder_MO);ui->label_129->setStyleSheet(Style_label_boder_MO);ui->label_129->setText("停机"); break;
        case Risining: ui->progressBar_29->setStyleSheet(Style_progress_risining);ui->label_29->setStyleSheet(Style_label_boder_move);ui->label_129->setStyleSheet(Style_label_boder_move);ui->label_129->setText("上升"); break;
        case Loose:    ui->progressBar_29->setStyleSheet(Style_progress_risining);ui->label_29->setStyleSheet(Style_label_boder_MO);ui->label_129->setStyleSheet(Style_label_boder_MO);ui->label_129->setText("松钩"); break;
        case Tight:    ui->progressBar_29->setStyleSheet(Style_progress_risining);ui->label_29->setStyleSheet(Style_label_boder_MO);ui->label_129->setStyleSheet(Style_label_boder_MO);ui->label_129->setText("紧钩"); break;
        case Falling:  ui->progressBar_29->setStyleSheet(Style_progress_risining);ui->label_29->setStyleSheet(Style_label_boder_move);ui->label_129->setStyleSheet(Style_label_boder_move);ui->label_129->setText("下降"); break;
        case Overload: ui->progressBar_29->setStyleSheet(Style_progress_overload);ui->label_29->setStyleSheet(Style_label_boder_NO);ui->label_129->setStyleSheet(Style_label_boder_NO);ui->label_129->setText("超载"); break;
        case LOSS:     ui->progressBar_29->setStyleSheet(Style_progress_overload);ui->label_29->setStyleSheet(Style_label_boder_NO);ui->label_129->setStyleSheet(Style_label_boder_NO);ui->label_129->setText("失载"); break;
        case CurrentOver:  ui->progressBar_29->setStyleSheet(Style_progress_overload);ui->label_29->setStyleSheet(Style_label_boder_NO);ui->label_129->setStyleSheet(Style_label_boder_NO);ui->label_129->setText("电流"); break;
        case Wait:      ui->progressBar_29->setStyleSheet(Style_progress_risining);ui->label_29->setStyleSheet(Style_label_boder_MO);ui->label_129->setStyleSheet(Style_label_boder_MO);ui->label_129->setText("等待"); break;
        case Bolt:      ui->progressBar_29->setStyleSheet(Style_progress_overload);ui->label_29->setStyleSheet(Style_label_boder_NO);ui->label_129->setStyleSheet(Style_label_boder_NO);ui->label_129->setText("断线"); break;
        case Warning: ui->progressBar_29->setStyleSheet(Style_progress_warning);ui->label_29->setStyleSheet(Style_label_boder_warning);ui->label_129->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_29->setStyleSheet(Style_progress_warning);ui->label_29->setStyleSheet(Style_label_boder_warning);ui->label_129->setStyleSheet(Style_label_boder_warning); ui->label_129->setText("设置");break;

        } ;break;
    case 29:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_30->setStyleSheet(Style_progress_risining);ui->label_30->setStyleSheet(Style_label_boder_MO);ui->label_130->setStyleSheet(Style_label_boder_MO);ui->label_130->setText("停机"); break;
        case Risining: ui->progressBar_30->setStyleSheet(Style_progress_risining);ui->label_30->setStyleSheet(Style_label_boder_move);ui->label_130->setStyleSheet(Style_label_boder_move);ui->label_130->setText("上升"); break;
        case Loose:    ui->progressBar_30->setStyleSheet(Style_progress_risining);ui->label_30->setStyleSheet(Style_label_boder_MO);ui->label_130->setStyleSheet(Style_label_boder_MO);ui->label_130->setText("松钩"); break;
        case Tight:    ui->progressBar_30->setStyleSheet(Style_progress_risining);ui->label_30->setStyleSheet(Style_label_boder_MO);ui->label_130->setStyleSheet(Style_label_boder_MO);ui->label_130->setText("紧钩"); break;
        case Falling:  ui->progressBar_30->setStyleSheet(Style_progress_risining);ui->label_30->setStyleSheet(Style_label_boder_move);ui->label_130->setStyleSheet(Style_label_boder_move);ui->label_130->setText("下降"); break;
        case Overload: ui->progressBar_30->setStyleSheet(Style_progress_overload);ui->label_30->setStyleSheet(Style_label_boder_NO);ui->label_130->setStyleSheet(Style_label_boder_NO);ui->label_130->setText("超载"); break;
        case LOSS:     ui->progressBar_30->setStyleSheet(Style_progress_overload);ui->label_30->setStyleSheet(Style_label_boder_NO);ui->label_130->setStyleSheet(Style_label_boder_NO);ui->label_130->setText("失载"); break;
        case CurrentOver:  ui->progressBar_30->setStyleSheet(Style_progress_overload);ui->label_30->setStyleSheet(Style_label_boder_NO);ui->label_130->setStyleSheet(Style_label_boder_NO);ui->label_130->setText("电流"); break;
        case Wait:      ui->progressBar_30->setStyleSheet(Style_progress_risining);ui->label_30->setStyleSheet(Style_label_boder_MO);ui->label_130->setStyleSheet(Style_label_boder_MO);ui->label_130->setText("等待"); break;
        case Bolt:      ui->progressBar_30->setStyleSheet(Style_progress_overload);ui->label_30->setStyleSheet(Style_label_boder_NO);ui->label_130->setStyleSheet(Style_label_boder_NO);ui->label_130->setText("断线"); break;
        case Warning: ui->progressBar_30->setStyleSheet(Style_progress_warning);ui->label_30->setStyleSheet(Style_label_boder_warning);ui->label_130->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_30->setStyleSheet(Style_progress_warning);ui->label_30->setStyleSheet(Style_label_boder_warning);ui->label_130->setStyleSheet(Style_label_boder_warning);ui->label_130->setText("设置"); break;

        } ;break;

    case 30:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_31->setStyleSheet(Style_progress_risining);ui->label_31->setStyleSheet(Style_label_boder_MO);ui->label_131->setStyleSheet(Style_label_boder_MO);ui->label_131->setText("停机"); break;
        case Risining: ui->progressBar_31->setStyleSheet(Style_progress_risining);ui->label_31->setStyleSheet(Style_label_boder_move);ui->label_131->setStyleSheet(Style_label_boder_move);ui->label_131->setText("上升"); break;
        case Loose:    ui->progressBar_31->setStyleSheet(Style_progress_risining);ui->label_31->setStyleSheet(Style_label_boder_MO);ui->label_131->setStyleSheet(Style_label_boder_MO);ui->label_131->setText("松钩"); break;
        case Tight:    ui->progressBar_31->setStyleSheet(Style_progress_risining);ui->label_31->setStyleSheet(Style_label_boder_MO);ui->label_131->setStyleSheet(Style_label_boder_MO);ui->label_131->setText("紧钩"); break;
        case Falling:  ui->progressBar_31->setStyleSheet(Style_progress_risining);ui->label_31->setStyleSheet(Style_label_boder_move);ui->label_131->setStyleSheet(Style_label_boder_move);ui->label_131->setText("下降"); break;
        case Overload: ui->progressBar_31->setStyleSheet(Style_progress_overload);ui->label_31->setStyleSheet(Style_label_boder_NO);ui->label_131->setStyleSheet(Style_label_boder_NO);ui->label_131->setText("超载"); break;
        case LOSS:     ui->progressBar_31->setStyleSheet(Style_progress_overload);ui->label_31->setStyleSheet(Style_label_boder_NO);ui->label_131->setStyleSheet(Style_label_boder_NO);ui->label_131->setText("失载"); break;
        case CurrentOver:  ui->progressBar_31->setStyleSheet(Style_progress_overload);ui->label_31->setStyleSheet(Style_label_boder_NO);ui->label_131->setStyleSheet(Style_label_boder_NO);ui->label_131->setText("电流"); break;
        case Wait:      ui->progressBar_31->setStyleSheet(Style_progress_risining);ui->label_31->setStyleSheet(Style_label_boder_MO);ui->label_131->setStyleSheet(Style_label_boder_MO);ui->label_131->setText("等待"); break;
        case Bolt:      ui->progressBar_31->setStyleSheet(Style_progress_overload);ui->label_31->setStyleSheet(Style_label_boder_NO);ui->label_131->setStyleSheet(Style_label_boder_NO);ui->label_131->setText("断线"); break;
        case Warning: ui->progressBar_31->setStyleSheet(Style_progress_warning);ui->label_31->setStyleSheet(Style_label_boder_warning);ui->label_131->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_31->setStyleSheet(Style_progress_warning);ui->label_31->setStyleSheet(Style_label_boder_warning);ui->label_131->setStyleSheet(Style_label_boder_warning);ui->label_131->setText("设置"); break;


        } ;break;
    case 31:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_32->setStyleSheet(Style_progress_risining);ui->label_32->setStyleSheet(Style_label_boder_MO);ui->label_132->setStyleSheet(Style_label_boder_MO);ui->label_132->setText("停机"); break;
        case Risining: ui->progressBar_32->setStyleSheet(Style_progress_risining);ui->label_32->setStyleSheet(Style_label_boder_move);ui->label_132->setStyleSheet(Style_label_boder_move);ui->label_132->setText("上升"); break;
        case Loose:    ui->progressBar_32->setStyleSheet(Style_progress_risining);ui->label_32->setStyleSheet(Style_label_boder_MO);ui->label_132->setStyleSheet(Style_label_boder_MO);ui->label_132->setText("松钩"); break;
        case Tight:    ui->progressBar_32->setStyleSheet(Style_progress_risining);ui->label_32->setStyleSheet(Style_label_boder_MO);ui->label_132->setStyleSheet(Style_label_boder_MO);ui->label_132->setText("紧钩"); break;
        case Falling:  ui->progressBar_32->setStyleSheet(Style_progress_risining);ui->label_32->setStyleSheet(Style_label_boder_move);ui->label_132->setStyleSheet(Style_label_boder_move);ui->label_132->setText("下降"); break;
        case Overload: ui->progressBar_32->setStyleSheet(Style_progress_overload);ui->label_32->setStyleSheet(Style_label_boder_NO);ui->label_132->setStyleSheet(Style_label_boder_NO);ui->label_132->setText("超载"); break;
        case LOSS:     ui->progressBar_32->setStyleSheet(Style_progress_overload);ui->label_32->setStyleSheet(Style_label_boder_NO);ui->label_132->setStyleSheet(Style_label_boder_NO);ui->label_132->setText("失载"); break;
        case CurrentOver:  ui->progressBar_32->setStyleSheet(Style_progress_overload);ui->label_32->setStyleSheet(Style_label_boder_NO);ui->label_132->setStyleSheet(Style_label_boder_NO);ui->label_132->setText("电流"); break;
        case Wait:      ui->progressBar_32->setStyleSheet(Style_progress_risining);ui->label_32->setStyleSheet(Style_label_boder_MO);ui->label_132->setStyleSheet(Style_label_boder_MO);ui->label_132->setText("等待"); break;
        case Bolt:      ui->progressBar_32->setStyleSheet(Style_progress_overload);ui->label_32->setStyleSheet(Style_label_boder_NO);ui->label_132->setStyleSheet(Style_label_boder_NO);ui->label_132->setText("断线"); break;
        case Warning: ui->progressBar_32->setStyleSheet(Style_progress_warning);ui->label_32->setStyleSheet(Style_label_boder_warning);ui->label_132->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_32->setStyleSheet(Style_progress_warning);ui->label_32->setStyleSheet(Style_label_boder_warning);ui->label_132->setStyleSheet(Style_label_boder_warning);ui->label_132->setText("设置"); break;


        } ;break;

    case 32:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_33->setStyleSheet(Style_progress_risining);ui->label_33->setStyleSheet(Style_label_boder_MO);ui->label_133->setStyleSheet(Style_label_boder_MO);ui->label_133->setText("停机"); break;
        case Risining: ui->progressBar_33->setStyleSheet(Style_progress_risining);ui->label_33->setStyleSheet(Style_label_boder_move);ui->label_133->setStyleSheet(Style_label_boder_move);ui->label_133->setText("上升"); break;
        case Loose:    ui->progressBar_33->setStyleSheet(Style_progress_risining);ui->label_33->setStyleSheet(Style_label_boder_MO);ui->label_133->setStyleSheet(Style_label_boder_MO);ui->label_133->setText("松钩"); break;
        case Tight:    ui->progressBar_33->setStyleSheet(Style_progress_risining);ui->label_33->setStyleSheet(Style_label_boder_MO);ui->label_133->setStyleSheet(Style_label_boder_MO);ui->label_133->setText("紧钩"); break;
        case Falling:  ui->progressBar_33->setStyleSheet(Style_progress_risining);ui->label_33->setStyleSheet(Style_label_boder_move);ui->label_133->setStyleSheet(Style_label_boder_move);ui->label_133->setText("下降"); break;
        case Overload: ui->progressBar_33->setStyleSheet(Style_progress_overload);ui->label_33->setStyleSheet(Style_label_boder_NO);ui->label_133->setStyleSheet(Style_label_boder_NO);ui->label_133->setText("超载"); break;
        case LOSS:     ui->progressBar_33->setStyleSheet(Style_progress_overload);ui->label_33->setStyleSheet(Style_label_boder_NO);ui->label_133->setStyleSheet(Style_label_boder_NO);ui->label_133->setText("失载"); break;
        case CurrentOver:  ui->progressBar_33->setStyleSheet(Style_progress_overload);ui->label_33->setStyleSheet(Style_label_boder_NO);ui->label_133->setStyleSheet(Style_label_boder_NO);ui->label_133->setText("电流"); break;
        case Wait:      ui->progressBar_33->setStyleSheet(Style_progress_risining);ui->label_33->setStyleSheet(Style_label_boder_MO);ui->label_133->setStyleSheet(Style_label_boder_MO);ui->label_133->setText("等待"); break;
        case Bolt:      ui->progressBar_33->setStyleSheet(Style_progress_overload);ui->label_33->setStyleSheet(Style_label_boder_NO);ui->label_133->setStyleSheet(Style_label_boder_NO);ui->label_133->setText("断线"); break;
        case Warning: ui->progressBar_33->setStyleSheet(Style_progress_warning);ui->label_33->setStyleSheet(Style_label_boder_warning);ui->label_133->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_33->setStyleSheet(Style_progress_warning);ui->label_33->setStyleSheet(Style_label_boder_warning);ui->label_133->setStyleSheet(Style_label_boder_warning);ui->label_133->setText("设置"); break;

        } ;break;
    case 33:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_34->setStyleSheet(Style_progress_risining);ui->label_34->setStyleSheet(Style_label_boder_MO);ui->label_134->setStyleSheet(Style_label_boder_MO);ui->label_134->setText("停机"); break;
        case Risining: ui->progressBar_34->setStyleSheet(Style_progress_risining);ui->label_34->setStyleSheet(Style_label_boder_move);ui->label_134->setStyleSheet(Style_label_boder_move);ui->label_134->setText("上升"); break;
        case Loose:    ui->progressBar_34->setStyleSheet(Style_progress_risining);ui->label_34->setStyleSheet(Style_label_boder_MO);ui->label_134->setStyleSheet(Style_label_boder_MO);ui->label_134->setText("松钩"); break;
        case Tight:    ui->progressBar_34->setStyleSheet(Style_progress_risining);ui->label_34->setStyleSheet(Style_label_boder_MO);ui->label_134->setStyleSheet(Style_label_boder_MO);ui->label_134->setText("紧钩"); break;
        case Falling:  ui->progressBar_34->setStyleSheet(Style_progress_risining);ui->label_34->setStyleSheet(Style_label_boder_move);ui->label_134->setStyleSheet(Style_label_boder_move);ui->label_134->setText("下降"); break;
        case Overload: ui->progressBar_34->setStyleSheet(Style_progress_overload);ui->label_34->setStyleSheet(Style_label_boder_NO);ui->label_134->setStyleSheet(Style_label_boder_NO);ui->label_134->setText("超载"); break;
        case LOSS:     ui->progressBar_34->setStyleSheet(Style_progress_overload);ui->label_34->setStyleSheet(Style_label_boder_NO);ui->label_134->setStyleSheet(Style_label_boder_NO);ui->label_134->setText("失载"); break;
        case CurrentOver:  ui->progressBar_34->setStyleSheet(Style_progress_overload);ui->label_34->setStyleSheet(Style_label_boder_NO);ui->label_134->setStyleSheet(Style_label_boder_NO);ui->label_134->setText("电流"); break;
        case Wait:      ui->progressBar_34->setStyleSheet(Style_progress_risining);ui->label_34->setStyleSheet(Style_label_boder_MO);ui->label_134->setStyleSheet(Style_label_boder_MO);ui->label_134->setText("等待"); break;
        case Bolt:      ui->progressBar_34->setStyleSheet(Style_progress_overload);ui->label_34->setStyleSheet(Style_label_boder_NO);ui->label_134->setStyleSheet(Style_label_boder_NO);ui->label_134->setText("断线"); break;
        case Warning: ui->progressBar_34->setStyleSheet(Style_progress_warning);ui->label_34->setStyleSheet(Style_label_boder_warning);ui->label_134->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_34->setStyleSheet(Style_progress_warning);ui->label_34->setStyleSheet(Style_label_boder_warning);ui->label_134->setStyleSheet(Style_label_boder_warning);ui->label_134->setText("设置"); break;


        } ;break;
    case 34:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_35->setStyleSheet(Style_progress_risining);ui->label_35->setStyleSheet(Style_label_boder_MO);ui->label_135->setStyleSheet(Style_label_boder_MO);ui->label_135->setText("停机"); break;
        case Risining: ui->progressBar_35->setStyleSheet(Style_progress_risining);ui->label_35->setStyleSheet(Style_label_boder_move);ui->label_135->setStyleSheet(Style_label_boder_move);ui->label_135->setText("上升"); break;
        case Loose:    ui->progressBar_35->setStyleSheet(Style_progress_risining);ui->label_35->setStyleSheet(Style_label_boder_MO);ui->label_135->setStyleSheet(Style_label_boder_MO);ui->label_135->setText("松钩"); break;
        case Tight:    ui->progressBar_35->setStyleSheet(Style_progress_risining);ui->label_35->setStyleSheet(Style_label_boder_MO);ui->label_135->setStyleSheet(Style_label_boder_MO);ui->label_135->setText("紧钩"); break;
        case Falling:  ui->progressBar_35->setStyleSheet(Style_progress_risining);ui->label_35->setStyleSheet(Style_label_boder_move);ui->label_135->setStyleSheet(Style_label_boder_move);ui->label_135->setText("下降"); break;
        case Overload: ui->progressBar_35->setStyleSheet(Style_progress_overload);ui->label_35->setStyleSheet(Style_label_boder_NO);ui->label_135->setStyleSheet(Style_label_boder_NO);ui->label_135->setText("超载"); break;
        case LOSS:     ui->progressBar_35->setStyleSheet(Style_progress_overload);ui->label_35->setStyleSheet(Style_label_boder_NO);ui->label_135->setStyleSheet(Style_label_boder_NO);ui->label_135->setText("失载"); break;
        case CurrentOver:  ui->progressBar_35->setStyleSheet(Style_progress_overload);ui->label_35->setStyleSheet(Style_label_boder_NO);ui->label_135->setStyleSheet(Style_label_boder_NO);ui->label_135->setText("电流"); break;
        case Wait:      ui->progressBar_35->setStyleSheet(Style_progress_risining);ui->label_35->setStyleSheet(Style_label_boder_MO);ui->label_135->setStyleSheet(Style_label_boder_MO);ui->label_135->setText("等待"); break;
        case Bolt:      ui->progressBar_35->setStyleSheet(Style_progress_overload);ui->label_35->setStyleSheet(Style_label_boder_NO);ui->label_135->setStyleSheet(Style_label_boder_NO);ui->label_135->setText("断线"); break;
        case Warning: ui->progressBar_35->setStyleSheet(Style_progress_warning);ui->label_35->setStyleSheet(Style_label_boder_warning);ui->label_135->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_35->setStyleSheet(Style_progress_warning);ui->label_35->setStyleSheet(Style_label_boder_warning);ui->label_135->setStyleSheet(Style_label_boder_warning);ui->label_135->setText("设置"); break;


        } ;break;
    case 35:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_36->setStyleSheet(Style_progress_risining);ui->label_36->setStyleSheet(Style_label_boder_MO);ui->label_136->setStyleSheet(Style_label_boder_MO);ui->label_136->setText("停机"); break;
        case Risining: ui->progressBar_36->setStyleSheet(Style_progress_risining);ui->label_36->setStyleSheet(Style_label_boder_move);ui->label_136->setStyleSheet(Style_label_boder_move);ui->label_136->setText("上升"); break;
        case Loose:    ui->progressBar_36->setStyleSheet(Style_progress_risining);ui->label_36->setStyleSheet(Style_label_boder_MO);ui->label_136->setStyleSheet(Style_label_boder_MO);ui->label_136->setText("松钩"); break;
        case Tight:    ui->progressBar_36->setStyleSheet(Style_progress_risining);ui->label_36->setStyleSheet(Style_label_boder_MO);ui->label_136->setStyleSheet(Style_label_boder_MO);ui->label_136->setText("紧钩"); break;
        case Falling:  ui->progressBar_36->setStyleSheet(Style_progress_risining);ui->label_36->setStyleSheet(Style_label_boder_move);ui->label_136->setStyleSheet(Style_label_boder_move);ui->label_136->setText("下降"); break;
        case Overload: ui->progressBar_36->setStyleSheet(Style_progress_overload);ui->label_36->setStyleSheet(Style_label_boder_NO);ui->label_136->setStyleSheet(Style_label_boder_NO);ui->label_136->setText("超载"); break;
        case LOSS:     ui->progressBar_36->setStyleSheet(Style_progress_overload);ui->label_36->setStyleSheet(Style_label_boder_NO);ui->label_136->setStyleSheet(Style_label_boder_NO);ui->label_136->setText("失载"); break;
        case CurrentOver:  ui->progressBar_36->setStyleSheet(Style_progress_overload);ui->label_36->setStyleSheet(Style_label_boder_NO);ui->label_136->setStyleSheet(Style_label_boder_NO);ui->label_136->setText("电流"); break;
        case Wait:      ui->progressBar_36->setStyleSheet(Style_progress_risining);ui->label_36->setStyleSheet(Style_label_boder_MO);ui->label_136->setStyleSheet(Style_label_boder_MO);ui->label_136->setText("等待"); break;
        case Bolt:      ui->progressBar_36->setStyleSheet(Style_progress_overload);ui->label_36->setStyleSheet(Style_label_boder_NO);ui->label_136->setStyleSheet(Style_label_boder_NO);ui->label_136->setText("断线"); break;
        case Warning: ui->progressBar_36->setStyleSheet(Style_progress_warning);ui->label_36->setStyleSheet(Style_label_boder_warning);ui->label_136->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_36->setStyleSheet(Style_progress_warning);ui->label_36->setStyleSheet(Style_label_boder_warning);ui->label_136->setStyleSheet(Style_label_boder_warning);ui->label_136->setText("设置"); break;


        } ;break;
    case 36:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_37->setStyleSheet(Style_progress_risining);ui->label_37->setStyleSheet(Style_label_boder_MO);ui->label_137->setStyleSheet(Style_label_boder_MO);ui->label_137->setText("停机"); break;
        case Risining: ui->progressBar_37->setStyleSheet(Style_progress_risining);ui->label_37->setStyleSheet(Style_label_boder_move);ui->label_137->setStyleSheet(Style_label_boder_move);ui->label_137->setText("上升"); break;
        case Loose:    ui->progressBar_37->setStyleSheet(Style_progress_risining);ui->label_37->setStyleSheet(Style_label_boder_MO);ui->label_137->setStyleSheet(Style_label_boder_MO);ui->label_137->setText("松钩"); break;
        case Tight:    ui->progressBar_37->setStyleSheet(Style_progress_risining);ui->label_37->setStyleSheet(Style_label_boder_MO);ui->label_137->setStyleSheet(Style_label_boder_MO);ui->label_137->setText("紧钩"); break;
        case Falling:  ui->progressBar_37->setStyleSheet(Style_progress_risining);ui->label_37->setStyleSheet(Style_label_boder_move);ui->label_137->setStyleSheet(Style_label_boder_move);ui->label_137->setText("下降"); break;
        case Overload: ui->progressBar_37->setStyleSheet(Style_progress_overload);ui->label_37->setStyleSheet(Style_label_boder_NO);ui->label_137->setStyleSheet(Style_label_boder_NO);ui->label_137->setText("超载"); break;
        case LOSS:     ui->progressBar_37->setStyleSheet(Style_progress_overload);ui->label_37->setStyleSheet(Style_label_boder_NO);ui->label_137->setStyleSheet(Style_label_boder_NO);ui->label_137->setText("失载"); break;
        case CurrentOver:  ui->progressBar_37->setStyleSheet(Style_progress_overload);ui->label_37->setStyleSheet(Style_label_boder_NO);ui->label_137->setStyleSheet(Style_label_boder_NO);ui->label_137->setText("电流"); break;
        case Wait:      ui->progressBar_37->setStyleSheet(Style_progress_risining);ui->label_37->setStyleSheet(Style_label_boder_MO);ui->label_137->setStyleSheet(Style_label_boder_MO);ui->label_137->setText("等待"); break;
        case Bolt:      ui->progressBar_37->setStyleSheet(Style_progress_overload);ui->label_37->setStyleSheet(Style_label_boder_NO);ui->label_137->setStyleSheet(Style_label_boder_NO);ui->label_137->setText("断线"); break;
        case Warning: ui->progressBar_37->setStyleSheet(Style_progress_warning);ui->label_37->setStyleSheet(Style_label_boder_warning);ui->label_137->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_37->setStyleSheet(Style_progress_warning);ui->label_37->setStyleSheet(Style_label_boder_warning);ui->label_137->setStyleSheet(Style_label_boder_warning);ui->label_137->setText("设置"); break;

        } ;break;
    case 37:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_38->setStyleSheet(Style_progress_risining);ui->label_38->setStyleSheet(Style_label_boder_MO);ui->label_138->setStyleSheet(Style_label_boder_MO);ui->label_138->setText("停机"); break;
        case Risining: ui->progressBar_38->setStyleSheet(Style_progress_risining);ui->label_38->setStyleSheet(Style_label_boder_move);ui->label_138->setStyleSheet(Style_label_boder_move);ui->label_138->setText("上升"); break;
        case Loose:    ui->progressBar_38->setStyleSheet(Style_progress_risining);ui->label_38->setStyleSheet(Style_label_boder_MO);ui->label_138->setStyleSheet(Style_label_boder_MO);ui->label_138->setText("松钩"); break;
        case Tight:    ui->progressBar_38->setStyleSheet(Style_progress_risining);ui->label_38->setStyleSheet(Style_label_boder_MO);ui->label_138->setStyleSheet(Style_label_boder_MO);ui->label_138->setText("紧钩"); break;
        case Falling:  ui->progressBar_38->setStyleSheet(Style_progress_risining);ui->label_38->setStyleSheet(Style_label_boder_move);ui->label_138->setStyleSheet(Style_label_boder_move);ui->label_138->setText("下降"); break;
        case Overload: ui->progressBar_38->setStyleSheet(Style_progress_overload);ui->label_38->setStyleSheet(Style_label_boder_NO);ui->label_138->setStyleSheet(Style_label_boder_NO);ui->label_138->setText("超载"); break;
        case LOSS:     ui->progressBar_38->setStyleSheet(Style_progress_overload);ui->label_38->setStyleSheet(Style_label_boder_NO);ui->label_138->setStyleSheet(Style_label_boder_NO);ui->label_138->setText("失载"); break;
        case CurrentOver:  ui->progressBar_38->setStyleSheet(Style_progress_overload);ui->label_38->setStyleSheet(Style_label_boder_NO);ui->label_138->setStyleSheet(Style_label_boder_NO);ui->label_138->setText("电流"); break;
        case Wait:      ui->progressBar_38->setStyleSheet(Style_progress_risining);ui->label_38->setStyleSheet(Style_label_boder_MO);ui->label_138->setStyleSheet(Style_label_boder_MO);ui->label_138->setText("等待"); break;
        case Bolt:      ui->progressBar_38->setStyleSheet(Style_progress_overload);ui->label_38->setStyleSheet(Style_label_boder_NO);ui->label_138->setStyleSheet(Style_label_boder_NO);ui->label_138->setText("断线"); break;
        case Warning: ui->progressBar_38->setStyleSheet(Style_progress_warning);ui->label_38->setStyleSheet(Style_label_boder_warning);ui->label_138->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_38->setStyleSheet(Style_progress_warning);ui->label_38->setStyleSheet(Style_label_boder_warning);ui->label_138->setStyleSheet(Style_label_boder_warning); ui->label_138->setText("设置");break;


        } ;break;
    case 38:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_39->setStyleSheet(Style_progress_risining);ui->label_39->setStyleSheet(Style_label_boder_MO);ui->label_139->setStyleSheet(Style_label_boder_MO);ui->label_139->setText("停机"); break;
        case Risining: ui->progressBar_39->setStyleSheet(Style_progress_risining);ui->label_39->setStyleSheet(Style_label_boder_move);ui->label_139->setStyleSheet(Style_label_boder_move);ui->label_139->setText("上升"); break;
        case Loose:    ui->progressBar_39->setStyleSheet(Style_progress_risining);ui->label_39->setStyleSheet(Style_label_boder_MO);ui->label_139->setStyleSheet(Style_label_boder_MO);ui->label_139->setText("松钩"); break;
        case Tight:    ui->progressBar_39->setStyleSheet(Style_progress_risining);ui->label_39->setStyleSheet(Style_label_boder_MO);ui->label_139->setStyleSheet(Style_label_boder_MO);ui->label_139->setText("紧钩"); break;
        case Falling:  ui->progressBar_39->setStyleSheet(Style_progress_risining);ui->label_39->setStyleSheet(Style_label_boder_move);ui->label_139->setStyleSheet(Style_label_boder_move);ui->label_139->setText("下降"); break;
        case Overload: ui->progressBar_39->setStyleSheet(Style_progress_overload);ui->label_39->setStyleSheet(Style_label_boder_NO);ui->label_139->setStyleSheet(Style_label_boder_NO);ui->label_139->setText("超载"); break;
        case LOSS:     ui->progressBar_39->setStyleSheet(Style_progress_overload);ui->label_39->setStyleSheet(Style_label_boder_NO);ui->label_139->setStyleSheet(Style_label_boder_NO);ui->label_139->setText("失载"); break;
        case CurrentOver:  ui->progressBar_39->setStyleSheet(Style_progress_overload);ui->label_39->setStyleSheet(Style_label_boder_NO);ui->label_139->setStyleSheet(Style_label_boder_NO);ui->label_139->setText("电流"); break;
        case Wait:      ui->progressBar_39->setStyleSheet(Style_progress_risining);ui->label_39->setStyleSheet(Style_label_boder_MO);ui->label_139->setStyleSheet(Style_label_boder_MO);ui->label_139->setText("等待"); break;
        case Bolt:      ui->progressBar_39->setStyleSheet(Style_progress_overload);ui->label_39->setStyleSheet(Style_label_boder_NO);ui->label_139->setStyleSheet(Style_label_boder_NO);ui->label_139->setText("断线"); break;
        case Warning: ui->progressBar_39->setStyleSheet(Style_progress_warning);ui->label_39->setStyleSheet(Style_label_boder_warning);ui->label_139->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_39->setStyleSheet(Style_progress_warning);ui->label_39->setStyleSheet(Style_label_boder_warning);ui->label_139->setStyleSheet(Style_label_boder_warning);ui->label_139->setText("设置"); break;


        } ;break;
    case 39:switch (State)   //color: rgb(57, 172, 0);border-color: rgb(57, 172, 0);
        {
        case Stop:     ui->progressBar_40->setStyleSheet(Style_progress_risining);ui->label_40->setStyleSheet(Style_label_boder_MO);ui->label_140->setStyleSheet(Style_label_boder_MO);ui->label_140->setText("停机"); break;
        case Risining: ui->progressBar_40->setStyleSheet(Style_progress_risining);ui->label_40->setStyleSheet(Style_label_boder_move);ui->label_140->setStyleSheet(Style_label_boder_move);ui->label_140->setText("上升"); break;
        case Loose:    ui->progressBar_40->setStyleSheet(Style_progress_risining);ui->label_40->setStyleSheet(Style_label_boder_MO);ui->label_140->setStyleSheet(Style_label_boder_MO);ui->label_140->setText("松钩"); break;
        case Tight:    ui->progressBar_40->setStyleSheet(Style_progress_risining);ui->label_40->setStyleSheet(Style_label_boder_MO);ui->label_140->setStyleSheet(Style_label_boder_MO);ui->label_140->setText("紧钩"); break;
        case Falling:  ui->progressBar_40->setStyleSheet(Style_progress_risining);ui->label_40->setStyleSheet(Style_label_boder_move);ui->label_140->setStyleSheet(Style_label_boder_move);ui->label_140->setText("下降"); break;
        case Overload: ui->progressBar_40->setStyleSheet(Style_progress_overload);ui->label_40->setStyleSheet(Style_label_boder_NO);ui->label_140->setStyleSheet(Style_label_boder_NO);ui->label_140->setText("超载"); break;
        case LOSS:     ui->progressBar_40->setStyleSheet(Style_progress_overload);ui->label_40->setStyleSheet(Style_label_boder_NO);ui->label_140->setStyleSheet(Style_label_boder_NO);ui->label_140->setText("失载"); break;
        case CurrentOver:  ui->progressBar_40->setStyleSheet(Style_progress_overload);ui->label_40->setStyleSheet(Style_label_boder_NO);ui->label_140->setStyleSheet(Style_label_boder_NO);ui->label_140->setText("电流"); break;
        case Wait:      ui->progressBar_40->setStyleSheet(Style_progress_risining);ui->label_40->setStyleSheet(Style_label_boder_MO);ui->label_140->setStyleSheet(Style_label_boder_MO);ui->label_140->setText("等待"); break;
        case Bolt:      ui->progressBar_40->setStyleSheet(Style_progress_overload);ui->label_40->setStyleSheet(Style_label_boder_NO);ui->label_140->setStyleSheet(Style_label_boder_NO);ui->label_140->setText("断线"); break;
        case Warning: ui->progressBar_40->setStyleSheet(Style_progress_warning);ui->label_40->setStyleSheet(Style_label_boder_warning);ui->label_140->setStyleSheet(Style_label_boder_warning); break;
        case Set: ui->progressBar_40->setStyleSheet(Style_progress_warning);ui->label_40->setStyleSheet(Style_label_boder_warning);ui->label_140->setStyleSheet(Style_label_boder_warning); ui->label_140->setText("设置");break;


        } ;break;

    default: break;


    }

}

void oneWindow::Check_Error(int id)
{
    //错误检查弹窗 并赋值

    //    Risining ,      //上升
    //    Loose  ,         //松钩
    //    Tight  ,         //紧钩
    //    Falling ,       //下降
    //    Overload ,       //超载
    //    LOSS ,           //失载
    //    CurrentOver ,   //电流过大
    //    Wait,           //等待切换响应
    //    Bolt,            //断线
    if(Motor1.Group == 1)       //  1组
    {
        switch (Motor1.branch_oneGroup[id].branch.state)        //判断错误码
        {
        case Risining:ui->label_WarningReason->setText("上升中");ui->label_WarningValue->setText("请先停机");break;
        case Loose:ui->label_WarningReason->setText("松钩中");ui->label_WarningValue->setText("请先停机");break;
        case Tight:ui->label_WarningReason->setText("紧钩中");ui->label_WarningValue->setText("请先停机");break;
        case Falling:ui->label_WarningReason->setText("下降中");ui->label_WarningValue->setText("请先停机");break;
        case Overload: ui->label_WarningReason->setText("超载");ui->label_WarningValue->setText(QString("%1.%2T").arg(Motor1.branch_oneGroup[id].branch.weight/10).arg(Motor1.branch_oneGroup[id].branch.weight%10));break;
        case LOSS:ui->label_WarningReason->setText("失载");ui->label_WarningValue->setText(QString("%1.%2T").arg(Motor1.branch_oneGroup[id].branch.weight/10).arg(Motor1.branch_oneGroup[id].branch.weight%10));break;
        case CurrentOver:ui->label_WarningReason->setText("电流异常");ui->label_WarningValue->setText(QString("%1.%2A").arg(Motor1.branch_oneGroup[id].branch.current/10).arg(Motor1.branch_oneGroup[id].branch.current%10));break;
        case Bolt:ui->label_WarningReason->setText("断线");ui->label_WarningValue->setText("--");break;
        }
        ui->label_WarningID->setText(QString("%1").arg(Motor1.branch_oneGroup[id].branch.id));
        if(Waining_wigFale == false)        //未显示
        {
            Waining_wigFale = true;
            ui->widget_WarningOne->show();
        }



    }else if(Motor1.Group == 2)     //2组
    {
        switch (Motor1.branch_twoGroup[id].branch.state)        //判断错误码
        {
        case Risining:ui->label_WarningReason->setText("上升中");ui->label_WarningValue->setText("请先停机");break;
        case Loose:ui->label_WarningReason->setText("松钩中");ui->label_WarningValue->setText("请先停机");break;
        case Tight:ui->label_WarningReason->setText("紧钩中");ui->label_WarningValue->setText("请先停机");break;
        case Falling:ui->label_WarningReason->setText("下降中");ui->label_WarningValue->setText("请先停机");break;
        case Overload: ui->label_WarningReason->setText("超载");ui->label_WarningValue->setText(QString("%1.%2T").arg(Motor1.branch_twoGroup[id].branch.weight/10).arg(Motor1.branch_twoGroup[id].branch.weight%10));break;
        case LOSS:ui->label_WarningReason->setText("失载");ui->label_WarningValue->setText(QString("%1.%2T").arg(Motor1.branch_twoGroup[id].branch.weight/10).arg(Motor1.branch_twoGroup[id].branch.weight%10));break;
        case CurrentOver:ui->label_WarningReason->setText("电流异常");ui->label_WarningValue->setText(QString("%1.%2A").arg(Motor1.branch_twoGroup[id].branch.current/10).arg(Motor1.branch_twoGroup[id].branch.current%10));break;
        case Bolt:ui->label_WarningReason->setText("断线");ui->label_WarningValue->setText("--");break;
        }
        ui->label_WarningID->setText(QString("%1").arg(Motor1.branch_twoGroup[id].branch.id));
        if(Waining_wigFale == false)        //未显示
        {
            Waining_wigFale = true;
            ui->widget_WarningOne->show();
        }


    }

}

void oneWindow::Set_Yes_StyleSheet()
{
    //设置按钮和进度条选中和未选中状态

    for (int i = 0 ; i < Motor1.page1_num ;i++ )
    {
        Motor1.branch_oneGroup[i].branch.choose = YES;

    }
    for (int i = 0 ; i < Motor1.page2_num ;i++ )
    {
        Motor1.branch_twoGroup[i].branch.choose = YES;

    }
//    ui->progressBar_01->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_01->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_02->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_02->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_03->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_03->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_04->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_04->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_05->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_05->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_06->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_06->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_07->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_07->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_08->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_08->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_09->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_09->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_10->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_10->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_11->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_11->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_12->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_12->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_13->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_13->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_14->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_14->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_15->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_15->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_16->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_16->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_17->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_17->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_18->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_18->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_19->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_19->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_20->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_20->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_21->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_21->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_22->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_22->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_23->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_23->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_24->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_24->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_25->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_25->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_26->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_26->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_27->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_27->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_28->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_28->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_29->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_29->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_30->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_30->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_31->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_31->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_32->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_32->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_33->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_33->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_34->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_34->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_35->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_35->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_36->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_36->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_37->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_37->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_38->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_38->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_39->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_39->setStyleSheet(pushButton_StyleSheet_YES);
//    ui->progressBar_40->setStyleSheet(progressBar_StyleSheet_YES);
    ui->pushButton_40->setStyleSheet(pushButton_StyleSheet_YES);




}

void oneWindow::Set_NO_StyleSheet()
{
      //设置按钮和进度条选中和未选中状态  未选中
    for (int i = 0 ; i < Motor1.page1_num ;i++ )
    {
        Motor1.branch_oneGroup[i].branch.choose = NO;

    }
    for (int i = 0 ; i < Motor1.page2_num ;i++ )
    {
        Motor1.branch_twoGroup[i].branch.choose = NO;

    }


//    ui->progressBar_01->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_01->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_02->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_02->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_03->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_03->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_04->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_04->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_05->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_05->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_06->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_06->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_07->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_07->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_08->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_08->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_09->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_09->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_10->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_10->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_11->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_11->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_12->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_12->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_13->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_13->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_14->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_14->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_15->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_15->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_16->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_16->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_17->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_17->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_18->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_18->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_19->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_19->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_20->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_20->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_21->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_21->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_22->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_22->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_23->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_23->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_24->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_24->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_25->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_25->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_26->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_26->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_27->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_27->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_28->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_28->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_29->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_29->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_30->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_30->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_31->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_31->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_32->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_32->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_33->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_33->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_34->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_34->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_35->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_35->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_36->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_36->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_37->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_37->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_38->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_38->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_39->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_39->setStyleSheet(pushButton_StyleSheet_NO);
//    ui->progressBar_40->setStyleSheet(progressBar_StyleSheet_NO);
    ui->pushButton_40->setStyleSheet(pushButton_StyleSheet_NO);

}

void oneWindow::on_pushButton_main_clicked()
{
    //开关电源
    if(Motor1.power == POWER_OFF)
    {
        Motor1.power = POWER_ON;
        ui->pushButton_main->setText("关电源");
        //操作日记
        QFileInfo file_op(DIARY_RUNNING);

        if(!file_op.isFile())       //判断文件如果不存在 ,就创建默认值文件
        {
             QSettings settings(DIARY_RUNNING,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
             settings.beginGroup("number");
             settings.setValue("all",0);
             settings.setValue("now",0);
             settings.endGroup();

        }
        QSettings settings(DIARY_RUNNING,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
        int all = settings.value("number/all").toInt() +1;
        settings.beginGroup(QString::number(all));
        settings.setValue("equipment","主控系统");
        settings.setValue("action","开电源");
        settings.setValue("group","all");
        settings.setValue("number",Motor1.all_num);
        settings.setValue("numberID","all");
        settings.setValue("role",User_information.User);
        settings.setValue("time",now_data);
        settings.endGroup();

        settings.beginGroup("number");
        settings.setValue("all",all);
        settings.endGroup();


   //     ui->pushButton_main->setStyleSheet("QPushButton{background-color: rgb(225, 152, 43);}");
    }else if ((Motor1.power == POWER_ON ) && (Motor1.Motor_state == Stop))   //
    {
        Motor1.power = POWER_OFF;
        ui->pushButton_main->setText("开电源");
        //操作日记
        QFileInfo file_op(DIARY_RUNNING);

        if(!file_op.isFile())       //判断文件如果不存在 ,就创建默认值文件
        {
             QSettings settings(DIARY_RUNNING,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
             settings.beginGroup("number");
             settings.setValue("all",0);
             settings.setValue("now",0);
             settings.endGroup();

        }
        QSettings settings(DIARY_RUNNING,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
        int all = settings.value("number/all").toInt() +1;
        settings.beginGroup(QString::number(all));
        settings.setValue("equipment","主控系统");
        settings.setValue("action","关电源");
        settings.setValue("group","all");
        settings.setValue("number",Motor1.all_num);
        settings.setValue("numberID","all");
        settings.setValue("role",User_information.User);
        settings.setValue("time",now_data);

        settings.endGroup();



        settings.beginGroup("number");
        settings.setValue("all",all);
        settings.endGroup();


    //    ui->pushButton_main->setStyleSheet("QPushButton{background-color: rgb(255, 255, 255);}");

     }else
    {
        QMessageBox::warning(NULL,"警告","请先停机再操作",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
      //  qDebug() << "请先停机";

     }
}

void oneWindow::on_oneTeamButton_clicked()
{
    //一组按钮点击
    if(Motor1.power == POWER_OFF)
    {
            if(Motor1.Group != 1 )
            {
            Motor1.Group = 1 ;
            progerss_location =0;
            ID_location = 0;
            Set_Yes_StyleSheet();
            Icon_show();
            Icon_hide(Motor1.page1_num);
            ID_IconSet(Motor1);
                // background-color: rgb(9, 82, 120);深色   background-color: rgb(183, 222, 242);浅色
            ui->oneTeamButton->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/843.png); background-color: rgb(183, 222, 242);}");
            ui->twoTeamButton->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/844.png);background-color: rgb(9, 82, 120);}");
            }

    }else if (Motor1.Group == 2)
    {
        //提示先关闭电源
        QMessageBox::warning(NULL,"警告","请先关闭电源再操作",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
      //  qDebug()<< "请先关闭电源";

    }

}


void oneWindow::on_twoTeamButton_clicked()
{
    //二组按钮点击

    if(Motor1.power == POWER_OFF)
    {

            if(  Motor1.Group == 1)
            {
                Motor1.Group = 2 ;
                Set_Yes_StyleSheet();
                progerss_location =0;
                 ID_location = 0;
                Icon_show();
                Icon_hide(Motor1.page2_num);
                 ID_IconSet(Motor1);
                ui->oneTeamButton->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/844.png);background-color: rgb(9, 82, 120);}");
                ui->twoTeamButton->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/843.png); background-color: rgb(183, 222, 242);}");
            }

    }else if (Motor1.Group == 1)
    {
        //提示先关闭电源
        QMessageBox::warning(NULL,"警告","请先关闭电源再操作",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
       // qDebug()<< "请先关闭电源";

    }

}


void oneWindow::on_pushButton_tight_clicked()
{
    //紧钩
    if(Motor1.power == POWER_ON)
    {
        if(Motor1.Motor_state != Tight)
        {
            if(Motor1.Motor_state == Stop )
            {
                //检查分控选中个数
             int CheckBox_num = 0;
             CheckBox_num = Check_ChooseNum();        //检查选中个数
             if(CheckBox_num > 0)          //选中个数大于0
             {
                //检查每个分控状态
              int key =   Check_StateOK(Motor1.Group);
              if(key < 0)
              {
                  //没有错，报错框隐藏
                  ui->widget_WarningOne->hide();
                  User_information.Action = "紧钩";
                  User_information.Howto = "主控系统";
                  Motor1.Motor_state = Tight;   //改变状态
                  Motor_OldState = Motor1.Motor_state;       //
                  //改变停机按钮
                  RounBar_set();
                  timer3->start(1200);      //定时判断紧钩是否完成

                  ID_location = 0;              //下标标志位复位
                  ui->pushButton_rising->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/113.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                                  "QPushButton:hover{background-image: url(:/back/Image/button/113.png); color: black;}"
                                                                  "QPushButton:pressed{background-image: url(:/back/Image/button/113.png);border-style: inset;}");

                  ui->pushButton_tight->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/845.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                                  "QPushButton:hover{background-image: url(:/back/Image/button/845.png); color: black;}"
                                                                  "QPushButton:pressed{background-image: url(:/back/Image/button/845.png);border-style: inset;}");

                  ui->pushButton_Loose  ->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/112.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                                  "QPushButton:hover{background-image: url(:/back/Image/button/112.png); color: black;}"
                                                                  "QPushButton:pressed{background-image: url(:/back/Image/button/112.png);border-style: inset;}");
                  qDebug() << "紧钩";

                 QString ID_num =  Check_IdNumber(Motor1);
                  //操作日记
                  QFileInfo file_op(DIARY_RUNNING);

                  if(!file_op.isFile())       //判断文件如果不存在 ,就创建默认值文件
                  {
                       QSettings settings(DIARY_RUNNING,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
                       settings.beginGroup("number");
                       settings.setValue("all",0);
                       settings.setValue("now",0);
                       settings.endGroup();

                  }
                  QSettings settings(DIARY_RUNNING,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
                  int all = settings.value("number/all").toInt() +1;
                  settings.beginGroup(QString::number(all));
                  settings.setValue("equipment","主控系统");
                  settings.setValue("action","紧钩");
                  settings.setValue("group",Motor1.Group);
                  settings.setValue("number",Motor1.all_num);
                  settings.setValue("numberID",ID_num);
                  settings.setValue("role",User_information.User);
                  settings.setValue("time",now_data);
                  settings.endGroup();

                  settings.beginGroup("number");
                  settings.setValue("all",all);
                  settings.endGroup();

              }else         //分控未处于停机
              {
                  Check_Error(key);
               }
            }else               //没有选中的分控
             {
                 QMessageBox::warning(NULL,"警告","请先选中分控",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
               //  qDebug() << "请先选中分控";

            }
            }else
            {
                QMessageBox::warning(NULL,"警告","请先停机再操作",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
              //  qDebug() << "请先停机";

            }

        }


    }else
    {
        QMessageBox::warning(NULL,"警告","请先关闭/打开电源再操作",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
      //  qDebug() << "请先开电源";

     }



}


void oneWindow::on_pushButton_Loose_clicked()
{
    //松钩
    if(Motor1.power == POWER_ON)
    {
        if(Motor1.Motor_state != Loose)
        {
        if(Motor1.Motor_state == Stop)
        {
            //检查分控选中个数
         int CheckBox_num = 0;
         CheckBox_num = Check_ChooseNum();        //检查选中个数
         if(CheckBox_num > 0)          //选中个数大于0
         {

            //检查每个分控状态
          int key =   Check_StateOK(Motor1.Group);
          if(key < 0)
          {
              //没有错，报错框隐藏
              ui->widget_WarningOne->hide();
              User_information.Action = "松钩";
              User_information.Howto = "主控系统";
              Motor1.Motor_state = Loose;     //改变状态为松钩
              Motor_OldState = Motor1.Motor_state;       //
              //改变停机按钮
              RounBar_set();
              timer3->start(1200);      //定时判断松钩是否完成

              ID_location = 0;              //下标标志位复位
            ui->pushButton_rising->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/113.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                            "QPushButton:hover{background-image: url(:/back/Image/button/113.png); color: black;}"
                                                            "QPushButton:pressed{background-image: url(:/back/Image/button/113.png);border-style: inset;}");

            ui->pushButton_tight->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/111.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                            "QPushButton:hover{background-image: url(:/back/Image/button/111.png); color: black;}"
                                                            "QPushButton:pressed{background-image: url(:/back/Image/button/111.png);border-style: inset;}");

            ui->pushButton_Loose  ->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/847.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                            "QPushButton:hover{background-image: url(:/back/Image/button/847.png); color: black;}"
                                                            "QPushButton:pressed{background-image: url(:/back/Image/button/847.png);border-style: inset;}");
             qDebug() <<"松钩";
             QString ID_num =  Check_IdNumber(Motor1);
              //操作日记
              QFileInfo file_op(DIARY_RUNNING);

              if(!file_op.isFile())       //判断文件如果不存在 ,就创建默认值文件
              {
                   QSettings settings(DIARY_RUNNING,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
                   settings.beginGroup("number");
                   settings.setValue("all",0);
                   settings.setValue("now",0);
                   settings.endGroup();

              }
              QSettings settings(DIARY_RUNNING,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
              int all = settings.value("number/all").toInt() +1;
              settings.beginGroup(QString::number(all));
              settings.setValue("equipment","主控系统");
              settings.setValue("action","松钩");
              settings.setValue("group",Motor1.Group);
              settings.setValue("number",Motor1.all_num);
              settings.setValue("numberID",ID_num);
              settings.setValue("role",User_information.User);
              settings.setValue("time",now_data);
              settings.endGroup();

              settings.beginGroup("number");
              settings.setValue("all",all);
              settings.endGroup();
          }else         //分控未处于停机
          {
              Check_Error(key);
           }

        }else
         {
               QMessageBox::warning(NULL,"警告","请先选中分控",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
            // qDebug() << "请先选中分控";
         }
        }else
        {
          //  QMessageBox::StandardButton result = QMessageBox::warning(NULL,"警告","请先停机再操作",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
            QMessageBox::warning(NULL,"警告","请先停机再操作",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
         //   qDebug() << "请先停机";

        }

    }

    }else
    {
         QMessageBox::warning(NULL,"警告","请先关闭/打开电源再操作",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
       // qDebug() << "请先开电源";

     }




}
void oneWindow::on_pushButton_rising_clicked()
{
    //上升
    if(Motor1.power == POWER_ON)
    {
        if(Motor1.Motor_state != Risining)
        {
            if(Motor1.Motor_state == Stop)
            {
                //检查分控选中个数
             int CheckBox_num = 0;
             CheckBox_num = Check_ChooseNum();        //检查选中个数
             if(CheckBox_num > 0)          //选中个数大于0
             {

               //检查每个分控状态
              int key =   Check_StateOK(Motor1.Group);
              if(key < 0)           //检查分控未报错
              {
                  //没有错，报错框隐藏
                  ui->widget_WarningOne->hide();


                  time_mark = true;    //上升开始计时
                  Motor1.Motor_state = Risining;   //改变状态为上升
                  Motor_OldState = Motor1.Motor_state;       //
                  //改变停机按钮
                  RounBar_set();
                  User_information.Action = "上升";
                  User_information.Howto = "主控系统";
                  ID_location = 0;                //下标标志位复位
                ui->pushButton_rising->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/846.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                                "QPushButton:hover{background-image: url(:/back/Image/button/846.png); color: black;}"
                                                                "QPushButton:pressed{background-image: url(:/back/Image/button/846.png);border-style: inset;}");

                ui->pushButton_tight->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/111.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                                "QPushButton:hover{background-image: url(:/back/Image/button/111.png); color: black;}"
                                                                "QPushButton:pressed{background-image: url(:/back/Image/button/111.png);border-style: inset;}");

                ui->pushButton_Loose  ->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/112.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                                "QPushButton:hover{background-image: url(:/back/Image/button/112.png); color: black;}"
                                                                "QPushButton:pressed{background-image: url(:/back/Image/button/112.png);border-style: inset;}");
                 qDebug() <<"上升";
                 QString ID_num =  Check_IdNumber(Motor1);
                  //操作日记
                  QFileInfo file_op(DIARY_RUNNING);

                  if(!file_op.isFile())       //判断文件如果不存在 ,就创建默认值文件
                  {
                       QSettings settings(DIARY_RUNNING,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
                       settings.beginGroup("number");
                       settings.setValue("all",0);
                       settings.setValue("now",0);
                       settings.endGroup();

                  }
                  QSettings settings(DIARY_RUNNING,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
                  int all = settings.value("number/all").toInt() +1;
                  settings.beginGroup(QString::number(all));
                  settings.setValue("equipment","主控系统");
                  settings.setValue("action","上升");
                  settings.setValue("group",Motor1.Group);
                  settings.setValue("number",Motor1.all_num);
                  settings.setValue("numberID",ID_num);
                  settings.setValue("role",User_information.User);
                  settings.setValue("time",now_data);
                  settings.endGroup();

                  settings.beginGroup("number");
                  settings.setValue("all",all);
                  settings.endGroup();
              }else         //分控未处于停机
              {
                  Check_Error(key);
               }

            }else           //选中个数为0
             {
                   QMessageBox::warning(NULL,"警告","请先选中分控",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
                 // qDebug() << "请先选中分控";


             }
            }else
            {
                QMessageBox::warning(NULL,"警告","请先停机再操作",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
             //   qDebug() << "请先停机";

            }

        }


    }else
    {
        QMessageBox::warning(NULL,"警告","请先关闭/打开电源再操作",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
       // qDebug() << "请先开电源";

     }






//    if(i == 0)
//    {
//        ui->pushButton_rising->setStyleSheet("background-color: rgb(170, 255, 255);");

//    }else if (i == 1)
//    {
//       // 设置底色 background-color: rgb(170, 255, 255);  圆形
//        ui->pushButton_rising->setStyleSheet("QPushButton{background-color: rgb(170, 225, 225);border:2px groove gray;border-radius:25px;padding:2px 4px;border-style: outset;}"
//                                               "QPushButton:hover{background-color:rgb(170, 241, 251); color: black;}"
//                                               "QPushButton:pressed{background-color:rgb(170, 228, 247);border-style: inset;}");
//        ui->progressBar_03->setStyleSheet("QProgressBar{border-color: rgb(240, 240, 240);background-color: rgb(240, 240, 240);} QProgressBar::chunk{background:red}");


//    }else if(i == 2)
//    {
//        // 设置按钮样式及悬浮、按下时的状态 圆角
//        ui->pushButton_rising->setStyleSheet("QPushButton{background-color: rgb(225, 225, 225);border:2px groove gray;border-radius:10px;padding:2px 4px;border-style: outset;}"
//                                                  "QPushButton:hover{background-color:rgb(229, 241, 251); color: black;}"
//                                                  "QPushButton:pressed{background-color:rgb(204, 228, 247);border-style: inset;}");
//    }
//    i++;
//    if(i == 3)
//    {
//        i = 0;
//    }

}
void oneWindow::Update_data()
{


    //控件状态更新

    //分组
    if(Motor1.Group == 1)       //  一组
    {
        if(progerss_location >= Motor1.page1_num)

           { progerss_location = 0;}

        //ID为0，表示未选中
        if(Motor1.branch_oneGroup[progerss_location].branch.choose == NO)
        {
            progerss_location++;
            return;
        }

         //判断是否报错  //未报错
        if(Motor1.branch_oneGroup[progerss_location].branch.mark == Right)
        {

            //设置状态   设置数值  是否设置中间预警状态
            Set_ProgressBarState(progerss_location,Motor1.branch_oneGroup[progerss_location].branch.state);
            Set_ProgressBarData(progerss_location,Motor1.branch_oneGroup[progerss_location].branch.weight);
            //判断预警值
            if(Motor1.branch_oneGroup[progerss_location].branch.weight > Motor1.branch_oneGroup[progerss_location].parameter.overload_value*0.8 && Motor1.branch_oneGroup[progerss_location].branch.weight < Motor1.branch_oneGroup[progerss_location].parameter.overload_value)
            {
                //预警范围
                Set_ProgressBarState(progerss_location,Warning);  //预警
            }
            progerss_location++;


        }else if(Motor1.branch_oneGroup[progerss_location].branch.mark == Wrong)  //  报错
        {
            //弹框报错

                if(Motor_OldState != Motor1.Motor_state && Motor1.Motor_state != Wait)        //由于状态改变导致停机
                {
                     qDebug() <<  Motor_OldState  << Motor1.Motor_state;
                    time_mark = false;    //上升停止计时

                    Motor1.Motor_state = Stop;       //  改变状态为停机
                    Motor_OldState = Motor1.Motor_state;       //
                     ID_location = 0;              //下标标志位复位
                    ui->pushButton_rising->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/113.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                                    "QPushButton:hover{background-image: url(:/back/Image/button/113.png); color: black;}"
                                                                    "QPushButton:pressed{background-image: url(:/back/Image/button/846.png);border-style: inset;}");

                    ui->pushButton_tight->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/111.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                                    "QPushButton:hover{background-image: url(:/back/Image/button/111.png); color: black;}"
                                                                    "QPushButton:pressed{background-image: url(:/back/Image/button/845.png);border-style: inset;}");

                    ui->pushButton_Loose  ->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/112.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                                    "QPushButton:hover{background-image: url(:/back/Image/button/112.png); color: black;}"
                                                                    "QPushButton:pressed{background-image: url(:/back/Image/button/847.png);border-style: inset;}");
                    qDebug() << "异常停机";

                }

            //弹框报错  未标记
            if(Motor1.branch_oneGroup[progerss_location].branch.mark_fale == false)
            {
                //弹窗
                Check_Error(progerss_location);
                Motor1.branch_oneGroup[progerss_location].branch.mark_fale = true;      //改变标记
                //保存到错误日记
                QFileInfo file_op(DIARY_FAULT);
                if(!file_op.isFile())       //判断文件如果不存在 ,就创建默认值文件
                {
                     QSettings settings(DIARY_FAULT,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
                     settings.beginGroup("number");
                     settings.setValue("all",0);
                     settings.setValue("now",0);
                     settings.endGroup();
                }

                QSettings settings(DIARY_FAULT,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
                int all = settings.value("number/all").toInt() +1;
                settings.beginGroup(QString::number(all));
                settings.setValue("ID", Motor1.branch_oneGroup[progerss_location].branch.id);
                QString state = Check_State(Motor1.branch_oneGroup[progerss_location].branch.state);
                settings.setValue("Fault",state);
                if(Motor1.branch_oneGroup[progerss_location].branch.state == CurrentOver)       //电流异常
               {
                 settings.setValue("Data",QString("%1.%2A").arg( Motor1.branch_oneGroup[progerss_location].branch.current/10).arg( Motor1.branch_oneGroup[progerss_location].branch.current%10));

                }else
                {
                   settings.setValue("Data",QString("%1.%2T").arg( Motor1.branch_oneGroup[progerss_location].branch.weight/10).arg( Motor1.branch_oneGroup[progerss_location].branch.weight%10));

                }
                settings.setValue("Action",User_information.Action);
                settings.setValue("Howto",User_information.Howto);
                settings.setValue("User",User_information.User);
                settings.setValue("time",now_data);
                settings.endGroup();
                settings.beginGroup("number");
                settings.setValue("all",all);
                settings.endGroup();

            }
            //设置状态   设置数值
            Set_ProgressBarState(progerss_location,Motor1.branch_oneGroup[progerss_location].branch.state);
            Set_ProgressBarData(progerss_location,Motor1.branch_oneGroup[progerss_location].branch.weight);
            progerss_location++;

        }

    }else if (Motor1.Group == 2)    //二组
    {
        if(progerss_location >= Motor1.page2_num)

           { progerss_location = 0;}

        //ID为0，表示未选中
        if(Motor1.branch_twoGroup[progerss_location].branch.choose == NO)
        {
            progerss_location++;
            return;
        }

         //判断是否报错  //未报错
        if(Motor1.branch_twoGroup[progerss_location].branch.mark == Right)
        {

            //设置状态   设置数值
            Set_ProgressBarState(progerss_location,Motor1.branch_twoGroup[progerss_location].branch.state);
            Set_ProgressBarData(progerss_location,Motor1.branch_twoGroup[progerss_location].branch.weight);
            //判断预警值
            if(Motor1.branch_twoGroup[progerss_location].branch.weight > Motor1.branch_twoGroup[progerss_location].parameter.overload_value*0.8 && Motor1.branch_twoGroup[progerss_location].branch.weight < Motor1.branch_twoGroup[progerss_location].parameter.overload_value)
            {
                //预警范围
                 Set_ProgressBarState(progerss_location,Warning);  //预警

            }
            progerss_location++;

        }else if(Motor1.branch_twoGroup[progerss_location].branch.mark == Wrong)      //  报错
        {

                if(Motor_OldState != Motor1.Motor_state && Motor1.Motor_state != Wait)        //由于状态改变导致停机
                {
                     qDebug() <<  Motor_OldState  << Motor1.Motor_state;
                    time_mark = false;    //上升停止计时

                    Motor1.Motor_state = Stop;       //  改变状态为停机
                    Motor_OldState = Motor1.Motor_state;       //
                     ID_location = 0;              //下标标志位复位
                    ui->pushButton_rising->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/113.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                                    "QPushButton:hover{background-image: url(:/back/Image/button/113.png); color: black;}"
                                                                    "QPushButton:pressed{background-image: url(:/back/Image/button/846.png);border-style: inset;}");

                    ui->pushButton_tight->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/111.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                                    "QPushButton:hover{background-image: url(:/back/Image/button/111.png); color: black;}"
                                                                    "QPushButton:pressed{background-image: url(:/back/Image/button/845.png);border-style: inset;}");

                    ui->pushButton_Loose  ->setStyleSheet("QPushButton{background-image: url(:/back/Image/button/112.png);border:0px groove gray;border-radius:20px;padding:2px 4px;border-style: outset;}"
                                                                    "QPushButton:hover{background-image: url(:/back/Image/button/112.png); color: black;}"
                                                                    "QPushButton:pressed{background-image: url(:/back/Image/button/847.png);border-style: inset;}");
                    qDebug() << "异常停机";

                }

            //弹框报错  未标记

            if(Motor1.branch_twoGroup[progerss_location].branch.mark_fale == false)
            {
                //弹窗
                Check_Error(progerss_location);
                Motor1.branch_twoGroup[progerss_location].branch.mark_fale = true;      //改变标记

                //保存到日记
                QFileInfo file_op(DIARY_FAULT);
                if(!file_op.isFile())       //判断文件如果不存在 ,就创建默认值文件
                {
                     QSettings settings(DIARY_FAULT,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
                     settings.beginGroup("number");
                     settings.setValue("all",0);
                     settings.setValue("now",0);
                     settings.endGroup();
                }

                QSettings settings(DIARY_FAULT,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
                int all = settings.value("number/all").toInt() +1;
                settings.beginGroup(QString::number(all));
                settings.setValue("ID", Motor1.branch_twoGroup[progerss_location].branch.id);
                QString state = Check_State(Motor1.branch_twoGroup[progerss_location].branch.state);
                settings.setValue("Fault",state);
                if(Motor1.branch_twoGroup[progerss_location].branch.state == CurrentOver)       //电流异常
               {
                 settings.setValue("Data",QString("%1.%2A").arg( Motor1.branch_twoGroup[progerss_location].branch.current/10).arg( Motor1.branch_twoGroup[progerss_location].branch.current%10));

                }else
                {
                   settings.setValue("Data",QString("%1.%2T").arg( Motor1.branch_twoGroup[progerss_location].branch.weight/10).arg( Motor1.branch_twoGroup[progerss_location].branch.weight%10));

                }
                settings.setValue("Action",User_information.Action);
                settings.setValue("Howto",User_information.Howto);
                settings.setValue("User",User_information.User);
                settings.setValue("time",now_data);
                settings.endGroup();
                settings.beginGroup("number");
                settings.setValue("all",all);
                settings.endGroup();


            }


            //设置状态   设置数值
            Set_ProgressBarState(progerss_location,Motor1.branch_twoGroup[progerss_location].branch.state);
            Set_ProgressBarData(progerss_location,Motor1.branch_twoGroup[progerss_location].branch.weight);
            progerss_location++;
        }

    }



}

int oneWindow::Check_StateOK(int Group)
{
    //查询分控状态
    if(Group == 1)
    {
        for (int i = 0 ;i < Motor1.page1_num ;i++ )
        {

            if(Motor1.branch_oneGroup[i].branch.choose == YES)
            {
                if(Motor1.branch_oneGroup[i].branch.state != Stop)
                {
                  //  qDebug() << QString("单控%1异常状态处于：%2").arg(Motor1.branch_oneGroup[i].branch.id).arg(Motor1.branch_oneGroup[i].branch.state);
                    return  i;
                }

            }
        }

    }else if(Group == 2)
    {
        for (int i = 0 ;i < Motor1.page2_num ;i++ )
        {

            if(Motor1.branch_twoGroup[i].branch.choose == YES)
            {
                if(Motor1.branch_twoGroup[i].branch.state != Stop)
                {
                 //   qDebug() << QString("单控%1异常状态处于：%2").arg(Motor1.branch_twoGroup[i].branch.id).arg(Motor1.branch_twoGroup[i].branch.state);
                    return  i;
                }

            }
        }

    }

    return  -1;


}

int oneWindow::Check_ChooseNum()
{
    //检查选中个数
    int number = 0;
    if(Motor1.Group == 1)       //1组
    {
        for(int i  = 0; i < Motor1.page1_num;i++)
        {
            if( Motor1.branch_oneGroup[i].branch.choose == YES)
            {
                number++;
            }

        }



    }else if (Motor1.Group == 2)            //2组
    {
        for (int i = 0 ; i < Motor1.page2_num ;i++ )
        {
            if(Motor1.branch_twoGroup->branch.choose == YES)
            {
                number++;
            }

        }


    }

    return number;

}

QString oneWindow::Check_State(uint8_t state)
{
//    Stop = 0x01,        //停机
//    Risining = 2,     //上升
//    Loose = 3  ,      //松钩
//    Tight = 4 ,       //紧钩
//    Falling=5 ,       //下降
//    Scan =6,          //扫描
//    Set = 7,          //设置参数
//    Wait =8,           //等待切换响应
//    Overload= 9 ,       //超载
//    LOSS =10,           //失载
//    CurrentOver =11 ,   //电流过大
//    Parameter,          //参数功能配置
//    Bolt ,              //断线
//    Warning,            //预警状态

    QString data;

    //将状态转成中文
    switch (state)
    {
    case Stop:data = "停机";break;
    case Risining:data = "上升";break;
    case Loose:data = "松钩";break;
    case Tight:data = "紧钩";break;
    case Falling:data = "下降";break;
    case Scan:data = "扫描";break;
    case Set:data = "设置";break;
    case Wait:data = "等待";break;
    case Overload:data = "超载";break;
    case LOSS:data = "失载";break;
    case CurrentOver:data = "电流异常";break;
    case Parameter:data = "参数设置";break;
    case Bolt:data = "断线";break;
    case Warning:data = "预警";break;
    }

    return data;

}

QString oneWindow::Check_IdNumber(Motor_dev Motor_unm)
{
    //选中ID数量，转换成字符
    QString ID;
    if(Motor_unm.Group == 1)
    {
        for (int i =0 ; i < Motor_unm.page1_num ;i++ )
        {
            if(Motor_unm.branch_oneGroup[i].branch.choose == YES)
            {
                ID += QString("%1").arg(Motor_unm.branch_oneGroup[i].branch.id);
            }

        }


    }else if (Motor_unm.Group ==2)
    {
        for (int i =0 ; i < Motor_unm.page2_num ;i++ )
        {
            if(Motor_unm.branch_twoGroup[i].branch.choose == YES)
            {
                ID += QString("%1").arg(Motor_unm.branch_twoGroup[i].branch.id);
            }

        }



    }else           //未选组
    {


    }

    return  ID;


}



void oneWindow::on_pushButton_control_clicked()
{
    //分控操作


}


void oneWindow::on_pushButton_oneConfig_clicked()
{
    //分控设置
    if(Motor1.power == POWER_OFF)
    {
   // timer2->stop();   //由于换页面还在运行，影响
   this->close();
    configWindow *Config = new configWindow;
    Config->show();


    }else
    {
        QMessageBox::warning(NULL,"警告","请先关闭电源再操作",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
       // qDebug() << "请先关闭电源";

     }
}


void oneWindow::on_pushButton_remote_clicked()
{
    //遥控
    if(Motor1.power == POWER_OFF)
    {
          this->close();
        remoteSet *remote = new remoteSet;
        remote->show();

    }else
    {
        QMessageBox::warning(NULL,"警告","请先关闭电源再操作",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
       // qDebug() << "请先关闭电源";

     }


}


void oneWindow::on_pushButton_project_clicked()
{
    //项目信息
    if(Motor1.power == POWER_OFF)
    {
         this->close();
        Project_information  *project = new Project_information;
        project->show();

    }else
    {
        QMessageBox::warning(NULL,"警告","请先关闭电源再操作",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
       // qDebug() << "请先关闭电源";

     }

}


void oneWindow::on_pushButton_system_clicked()
{
    //系统信息
    if(Motor1.power == POWER_OFF)
    {
          this->close();
        Syetem_information *system = new Syetem_information;
        system->show();

    }else
    {
        QMessageBox::warning(NULL,"警告","请先关闭电源再操作",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
      //  qDebug() << "请先关闭电源";

     }

}


void oneWindow::on_pushButton_dairy_clicked()
{
    //日志信息
    if(Motor1.power == POWER_OFF)
    {
        this->close();
        DiaryWindow  *diary = new DiaryWindow;
        diary->show();

    }else
    {
        QMessageBox::warning(NULL,"警告","请先关闭电源再操作",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
       // qDebug() << "请先关闭电源";

     }


}



void oneWindow::on_YesOrNOButton_clicked()
{
    //全选反选

    if(Motor1.Motor_state == Stop)
    {
        if(YesOrNOButton_mark == YES)
        {


            Set_NO_StyleSheet();
            YesOrNOButton_mark = NO;


        }else           //
        {
            Set_Yes_StyleSheet();
            YesOrNOButton_mark = YES;


        }

    }else
    {
        QMessageBox::warning(NULL,"警告","请先停机再操作",QMessageBox::Yes | QMessageBox::No , QMessageBox::Yes);
      //  qDebug() << "请先停机再操作";

  }



 }


void oneWindow::on_pushButton_01_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {      
        if(Motor1.branch_oneGroup[0].branch.choose == YES)
        {
            Motor1.branch_oneGroup[0].branch.choose = NO;
            //改变柱子颜色
         //   ui->progressBar_01->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_01->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[0].branch.choose = YES;
         //   ui->progressBar_01->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_01->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[0].branch.choose == YES)
        {
            Motor1.branch_twoGroup[0].branch.choose = NO;
            //改变柱子颜色
         //   ui->progressBar_01->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_01->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[0].branch.choose = YES;
          //  ui->progressBar_01->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_01->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }


}






void oneWindow::on_pushButton_02_clicked()
{
  if(Motor1.Motor_state == Stop)
  {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[1].branch.choose == YES)
        {
            Motor1.branch_oneGroup[1].branch.choose = NO;
            //改变柱子颜色
        //    ui->progressBar_02->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_02->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[1].branch.choose = YES;
       //     ui->progressBar_02->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_02->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[1].branch.choose == YES)
        {
            Motor1.branch_twoGroup[1].branch.choose = NO;
            //改变柱子颜色
       //     ui->progressBar_02->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_02->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[1].branch.choose = YES;
       //     ui->progressBar_02->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_02->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
  }
}


void oneWindow::on_pushButton_03_clicked()
{
  if(Motor1.Motor_state == Stop)
  {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[2].branch.choose == YES)
        {
            Motor1.branch_oneGroup[2].branch.choose = NO;
            //改变柱子颜色
       //     ui->progressBar_03->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_03->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[2].branch.choose = YES;
       //     ui->progressBar_03->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_03->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[2].branch.choose == YES)
        {
            Motor1.branch_twoGroup[2].branch.choose = NO;
            //改变柱子颜色
        //    ui->progressBar_03->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_03->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[2].branch.choose = YES;
       //     ui->progressBar_03->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_03->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
  }


}


void oneWindow::on_pushButton_04_clicked()
{
  if(Motor1.Motor_state == Stop)
  {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[3].branch.choose == YES)
        {
            Motor1.branch_oneGroup[3].branch.choose = NO;
            //改变柱子颜色
      //      ui->progressBar_04->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_04->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[3].branch.choose = YES;
        //    ui->progressBar_04->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_04->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[3].branch.choose == YES)
        {
            Motor1.branch_twoGroup[3].branch.choose = NO;
            //改变柱子颜色
       //     ui->progressBar_04->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_04->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[3].branch.choose = YES;
       //     ui->progressBar_04->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_04->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
  }
}


void oneWindow::on_pushButton_05_clicked()
{
  if(Motor1.Motor_state == Stop)
  {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[4].branch.choose == YES)
        {
            Motor1.branch_oneGroup[4].branch.choose = NO;
            //改变柱子颜色
       //     ui->progressBar_05->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_05->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[4].branch.choose = YES;
        //   ui->progressBar_05->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_05->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[4].branch.choose == YES)
        {
            Motor1.branch_twoGroup[4].branch.choose = NO;
            //改变柱子颜色
        //    ui->progressBar_05->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_05->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[4].branch.choose = YES;
       //     ui->progressBar_05->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_05->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
  }

}


void oneWindow::on_pushButton_06_clicked()
{
  if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[5].branch.choose == YES)
        {
            Motor1.branch_oneGroup[5].branch.choose = NO;
            //改变柱子颜色
       //     ui->progressBar_06->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_06->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[5].branch.choose = YES;
       //     ui->progressBar_06->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_06->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[5].branch.choose == YES)
        {
            Motor1.branch_twoGroup[5].branch.choose = NO;
            //改变柱子颜色
         //   ui->progressBar_06->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_06->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[5].branch.choose = YES;
       //     ui->progressBar_06->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_06->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
  }

}


void oneWindow::on_pushButton_07_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[6].branch.choose == YES)
        {
            Motor1.branch_oneGroup[6].branch.choose = NO;
            //改变柱子颜色
      //      ui->progressBar_07->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_07->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[6].branch.choose = YES;
       //     ui->progressBar_07->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_07->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[6].branch.choose == YES)
        {
            Motor1.branch_twoGroup[6].branch.choose = NO;
            //改变柱子颜色
      //      ui->progressBar_07->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_07->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[6].branch.choose = YES;
      //      ui->progressBar_07->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_07->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }

    }
}


void oneWindow::on_pushButton_08_clicked()
{
   if(Motor1.Motor_state == Stop)
   {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[7].branch.choose == YES)
        {
            Motor1.branch_oneGroup[7].branch.choose = NO;
            //改变柱子颜色
     //       ui->progressBar_08->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_08->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[7].branch.choose = YES;
       //     ui->progressBar_08->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_08->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[7].branch.choose == YES)
        {
            Motor1.branch_twoGroup[7].branch.choose = NO;
            //改变柱子颜色
      //      ui->progressBar_08->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_08->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[7].branch.choose = YES;
      //      ui->progressBar_08->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_08->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
   }

}


void oneWindow::on_pushButton_09_clicked()
{
  if(Motor1.Motor_state == Stop)
   {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[8].branch.choose == YES)
        {
            Motor1.branch_oneGroup[8].branch.choose = NO;
            //改变柱子颜色
     //       ui->progressBar_09->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_09->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[8].branch.choose = YES;
      //      ui->progressBar_09->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_09->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[8].branch.choose == YES)
        {
            Motor1.branch_twoGroup[8].branch.choose = NO;
            //改变柱子颜色
      //      ui->progressBar_09->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_09->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[8].branch.choose = YES;
       //     ui->progressBar_09->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_09->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
  }
}


void oneWindow::on_pushButton_10_clicked()
{
   if(Motor1.Motor_state == Stop)
     {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[9].branch.choose == YES)
        {
            Motor1.branch_oneGroup[9].branch.choose = NO;
            //改变柱子颜色
     //       ui->progressBar_10->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_10->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[9].branch.choose = YES;
      //      ui->progressBar_10->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_10->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[9].branch.choose == YES)
        {
            Motor1.branch_twoGroup[9].branch.choose = NO;
            //改变柱子颜色
     //       ui->progressBar_10->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_10->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[9].branch.choose = YES;
      //      ui->progressBar_10->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_10->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
   }

}


void oneWindow::on_pushButton_11_clicked()
{
 if(Motor1.Motor_state == Stop)
 {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[10].branch.choose == YES)
        {
            Motor1.branch_oneGroup[10].branch.choose = NO;
            //改变柱子颜色
    //        ui->progressBar_11->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_11->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[10].branch.choose = YES;
     //       ui->progressBar_11->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_11->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[10].branch.choose == YES)
        {
            Motor1.branch_twoGroup[10].branch.choose = NO;
            //改变柱子颜色
       //     ui->progressBar_11->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_11->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[10].branch.choose = YES;
      //      ui->progressBar_11->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_11->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
 }

}


void oneWindow::on_pushButton_12_clicked()
{
  if(Motor1.Motor_state == Stop)
  {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[11].branch.choose == YES)
        {
            Motor1.branch_oneGroup[11].branch.choose = NO;
            //改变柱子颜色
    //        ui->progressBar_12->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_12->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[11].branch.choose = YES;
     //       ui->progressBar_12->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_12->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[11].branch.choose == YES)
        {
            Motor1.branch_twoGroup[11].branch.choose = NO;
            //改变柱子颜色
     //       ui->progressBar_12->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_12->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[11].branch.choose = YES;
      //      ui->progressBar_12->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_12->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
  }

}


void oneWindow::on_pushButton_13_clicked()
{
  if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[12].branch.choose == YES)
        {
            Motor1.branch_oneGroup[12].branch.choose = NO;
            //改变柱子颜色
     //       ui->progressBar_13->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_13->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[12].branch.choose = YES;
     //       ui->progressBar_13->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_13->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[12].branch.choose == YES)
        {
            Motor1.branch_twoGroup[12].branch.choose = NO;
            //改变柱子颜色
      //      ui->progressBar_13->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_13->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[12].branch.choose = YES;
      //      ui->progressBar_13->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_13->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
  }
}


void oneWindow::on_pushButton_14_clicked()
{
 if(Motor1.Motor_state == Stop)
 {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[13].branch.choose == YES)
        {
            Motor1.branch_oneGroup[13].branch.choose = NO;
            //改变柱子颜色
    //        ui->progressBar_14->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_14->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[13].branch.choose = YES;
     //       ui->progressBar_14->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_14->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[13].branch.choose == YES)
        {
            Motor1.branch_twoGroup[13].branch.choose = NO;
            //改变柱子颜色
      //      ui->progressBar_14->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_14->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[13].branch.choose = YES;
      //      ui->progressBar_14->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_14->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
 }
}


void oneWindow::on_pushButton_15_clicked()
{
  if(Motor1.Motor_state == Stop)
  {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[14].branch.choose == YES)
        {
            Motor1.branch_oneGroup[14].branch.choose = NO;
            //改变柱子颜色
     //       ui->progressBar_15->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_15->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[14].branch.choose = YES;
      //      ui->progressBar_15->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_15->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[14].branch.choose == YES)
        {
            Motor1.branch_twoGroup[14].branch.choose = NO;
            //改变柱子颜色
     //       ui->progressBar_15->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_15->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[14].branch.choose = YES;
       //     ui->progressBar_15->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_15->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
  }
}


void oneWindow::on_pushButton_16_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[15].branch.choose == YES)
        {
            Motor1.branch_oneGroup[15].branch.choose = NO;
            //改变柱子颜色
    //        ui->progressBar_16->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_16->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[15].branch.choose = YES;
    //        ui->progressBar_16->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_16->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[15].branch.choose == YES)
        {
            Motor1.branch_twoGroup[15].branch.choose = NO;
            //改变柱子颜色
       //     ui->progressBar_16->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_16->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[15].branch.choose = YES;
      //      ui->progressBar_16->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_16->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }
}


void oneWindow::on_pushButton_17_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[16].branch.choose == YES)
        {
            Motor1.branch_oneGroup[16].branch.choose = NO;
            //改变柱子颜色
  //          ui->progressBar_17->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_17->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[16].branch.choose = YES;
     //       ui->progressBar_17->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_17->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[16].branch.choose == YES)
        {
            Motor1.branch_twoGroup[16].branch.choose = NO;
            //改变柱子颜色
      //      ui->progressBar_17->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_17->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[16].branch.choose = YES;
     //       ui->progressBar_17->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_17->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }
}


void oneWindow::on_pushButton_18_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[17].branch.choose == YES)
        {
            Motor1.branch_oneGroup[17].branch.choose = NO;
            //改变柱子颜色
     //       ui->progressBar_18->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_18->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[17].branch.choose = YES;
      //      ui->progressBar_18->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_18->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[17].branch.choose == YES)
        {
            Motor1.branch_twoGroup[17].branch.choose = NO;
            //改变柱子颜色
     //       ui->progressBar_18->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_18->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[17].branch.choose = YES;
       //     ui->progressBar_18->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_18->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }
}


void oneWindow::on_pushButton_19_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[18].branch.choose == YES)
        {
            Motor1.branch_oneGroup[18].branch.choose = NO;
            //改变柱子颜色
    //        ui->progressBar_19->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_19->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[18].branch.choose = YES;
      //      ui->progressBar_19->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_19->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[18].branch.choose == YES)
        {
            Motor1.branch_twoGroup[18].branch.choose = NO;
            //改变柱子颜色
      //      ui->progressBar_19->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_19->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[18].branch.choose = YES;
    //        ui->progressBar_19->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_19->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }

}


void oneWindow::on_pushButton_20_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[19].branch.choose == YES)
        {
            Motor1.branch_oneGroup[19].branch.choose = NO;
            //改变柱子颜色
       //     ui->progressBar_20->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_20->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[19].branch.choose = YES;
       //     ui->progressBar_20->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_20->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[19].branch.choose == YES)
        {
            Motor1.branch_twoGroup[19].branch.choose = NO;
            //改变柱子颜色
     //       ui->progressBar_20->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_20->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[19].branch.choose = YES;
      //      ui->progressBar_20->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_20->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }

}


void oneWindow::on_pushButton_21_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[20].branch.choose == YES)
        {
            Motor1.branch_oneGroup[20].branch.choose = NO;
            //改变柱子颜色
    //        ui->progressBar_21->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_21->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[20].branch.choose = YES;
     //       ui->progressBar_21->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_21->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[20].branch.choose == YES)
        {
            Motor1.branch_twoGroup[20].branch.choose = NO;
            //改变柱子颜色
     //       ui->progressBar_21->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_21->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[20].branch.choose = YES;
    //        ui->progressBar_21->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_21->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }

}


void oneWindow::on_pushButton_22_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[21].branch.choose == YES)
        {
            Motor1.branch_oneGroup[21].branch.choose = NO;
            //改变柱子颜色
     //       ui->progressBar_22->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_22->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[21].branch.choose = YES;
     //       ui->progressBar_22->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_22->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[21].branch.choose == YES)
        {
            Motor1.branch_twoGroup[21].branch.choose = NO;
            //改变柱子颜色
     //       ui->progressBar_22->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_22->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[21].branch.choose = YES;
     //       ui->progressBar_22->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_22->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }

}


void oneWindow::on_pushButton_23_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[22].branch.choose == YES)
        {
            Motor1.branch_oneGroup[22].branch.choose = NO;
            //改变柱子颜色
   //         ui->progressBar_23->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_23->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[22].branch.choose = YES;
   //         ui->progressBar_23->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_23->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[22].branch.choose == YES)
        {
            Motor1.branch_twoGroup[22].branch.choose = NO;
            //改变柱子颜色
   //         ui->progressBar_23->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_23->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[22].branch.choose = YES;
    //        ui->progressBar_23->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_23->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }

}


void oneWindow::on_pushButton_24_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[23].branch.choose == YES)
        {
            Motor1.branch_oneGroup[23].branch.choose = NO;
            //改变柱子颜色
     //       ui->progressBar_24->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_24->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[23].branch.choose = YES;
    //        ui->progressBar_24->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_24->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[23].branch.choose == YES)
        {
            Motor1.branch_twoGroup[23].branch.choose = NO;
            //改变柱子颜色
     //       ui->progressBar_24->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_24->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[23].branch.choose = YES;
      //      ui->progressBar_24->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_24->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }

}


void oneWindow::on_pushButton_25_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[24].branch.choose == YES)
        {
            Motor1.branch_oneGroup[24].branch.choose = NO;
            //改变柱子颜色
     //       ui->progressBar_25->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_25->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[24].branch.choose = YES;
     //       ui->progressBar_25->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_25->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[24].branch.choose == YES)
        {
            Motor1.branch_twoGroup[24].branch.choose = NO;
            //改变柱子颜色
       //     ui->progressBar_25->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_25->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[24].branch.choose = YES;
      //      ui->progressBar_25->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_25->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }

}


void oneWindow::on_pushButton_26_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[25].branch.choose == YES)
        {
            Motor1.branch_oneGroup[25].branch.choose = NO;
            //改变柱子颜色
      //      ui->progressBar_26->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_26->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[25].branch.choose = YES;
      //      ui->progressBar_26->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_26->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[25].branch.choose == YES)
        {
            Motor1.branch_twoGroup[25].branch.choose = NO;
            //改变柱子颜色
       //     ui->progressBar_26->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_26->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[25].branch.choose = YES;
       //     ui->progressBar_26->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_26->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }

}


void oneWindow::on_pushButton_27_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[26].branch.choose == YES)
        {
            Motor1.branch_oneGroup[26].branch.choose = NO;
            //改变柱子颜色
    //        ui->progressBar_27->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_27->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[26].branch.choose = YES;
   //         ui->progressBar_27->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_27->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[26].branch.choose == YES)
        {
            Motor1.branch_twoGroup[26].branch.choose = NO;
            //改变柱子颜色
    //        ui->progressBar_27->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_27->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[26].branch.choose = YES;
     //       ui->progressBar_27->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_27->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }

}


void oneWindow::on_pushButton_28_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[27].branch.choose == YES)
        {
            Motor1.branch_oneGroup[27].branch.choose = NO;
            //改变柱子颜色
    //        ui->progressBar_28->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_28->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[27].branch.choose = YES;
     //       ui->progressBar_28->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_28->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[27].branch.choose == YES)
        {
            Motor1.branch_twoGroup[27].branch.choose = NO;
            //改变柱子颜色
     //       ui->progressBar_28->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_28->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[27].branch.choose = YES;
      //      ui->progressBar_28->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_28->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }

}


void oneWindow::on_pushButton_29_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[28].branch.choose == YES)
        {
            Motor1.branch_oneGroup[28].branch.choose = NO;
            //改变柱子颜色
    //        ui->progressBar_29->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_29->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[28].branch.choose = YES;
    //        ui->progressBar_29->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_29->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[28].branch.choose == YES)
        {
            Motor1.branch_twoGroup[28].branch.choose = NO;
            //改变柱子颜色
     //       ui->progressBar_29->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_29->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[28].branch.choose = YES;
    //        ui->progressBar_29->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_29->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }

}


void oneWindow::on_pushButton_30_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[29].branch.choose == YES)
        {
            Motor1.branch_oneGroup[29].branch.choose = NO;
            //改变柱子颜色
    //        ui->progressBar_30->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_30->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[29].branch.choose = YES;
   //         ui->progressBar_30->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_30->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[29].branch.choose == YES)
        {
            Motor1.branch_twoGroup[29].branch.choose = NO;
            //改变柱子颜色
   //         ui->progressBar_30->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_30->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[29].branch.choose = YES;
    //        ui->progressBar_30->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_30->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }

}


void oneWindow::on_pushButton_31_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[30].branch.choose == YES)
        {
            Motor1.branch_oneGroup[30].branch.choose = NO;
            //改变柱子颜色
  //          ui->progressBar_31->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_31->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[30].branch.choose = YES;
   //         ui->progressBar_31->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_31->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[30].branch.choose == YES)
        {
            Motor1.branch_twoGroup[30].branch.choose = NO;
            //改变柱子颜色
   //         ui->progressBar_31->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_31->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[30].branch.choose = YES;
   //         ui->progressBar_31->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_31->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }

}


void oneWindow::on_pushButton_32_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[31].branch.choose == YES)
        {
            Motor1.branch_oneGroup[31].branch.choose = NO;
            //改变柱子颜色
   //         ui->progressBar_32->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_32->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[31].branch.choose = YES;
   //         ui->progressBar_32->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_32->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[31].branch.choose == YES)
        {
            Motor1.branch_twoGroup[31].branch.choose = NO;
            //改变柱子颜色
    //        ui->progressBar_32->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_32->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[31].branch.choose = YES;
      //      ui->progressBar_32->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_32->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }

}


void oneWindow::on_pushButton_33_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[32].branch.choose == YES)
        {
            Motor1.branch_oneGroup[32].branch.choose = NO;
            //改变柱子颜色
    //        ui->progressBar_33->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_33->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[32].branch.choose = YES;
     //       ui->progressBar_33->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_33->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[32].branch.choose == YES)
        {
            Motor1.branch_twoGroup[32].branch.choose = NO;
            //改变柱子颜色
    //        ui->progressBar_33->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_33->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[32].branch.choose = YES;
    //        ui->progressBar_33->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_33->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }

}


void oneWindow::on_pushButton_34_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[33].branch.choose == YES)
        {
            Motor1.branch_oneGroup[33].branch.choose = NO;
            //改变柱子颜色
  //          ui->progressBar_34->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_34->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[33].branch.choose = YES;
 //           ui->progressBar_34->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_34->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[33].branch.choose == YES)
        {
            Motor1.branch_twoGroup[33].branch.choose = NO;
            //改变柱子颜色
    //        ui->progressBar_34->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_34->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[33].branch.choose = YES;
   //         ui->progressBar_34->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_34->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }

}


void oneWindow::on_pushButton_35_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[34].branch.choose == YES)
        {
            Motor1.branch_oneGroup[34].branch.choose = NO;
            //改变柱子颜色
    //        ui->progressBar_35->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_35->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[34].branch.choose = YES;
     //       ui->progressBar_35->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_35->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[34].branch.choose == YES)
        {
            Motor1.branch_twoGroup[34].branch.choose = NO;
            //改变柱子颜色
    //        ui->progressBar_35->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_35->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[34].branch.choose = YES;
    //        ui->progressBar_35->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_35->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }

}


void oneWindow::on_pushButton_36_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[35].branch.choose == YES)
        {
            Motor1.branch_oneGroup[35].branch.choose = NO;
            //改变柱子颜色
   //         ui->progressBar_36->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_36->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[35].branch.choose = YES;
   //         ui->progressBar_36->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_36->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[35].branch.choose == YES)
        {
            Motor1.branch_twoGroup[35].branch.choose = NO;
     //       //改变柱子颜色
     //       ui->progressBar_36->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_36->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[35].branch.choose = YES;
      //      ui->progressBar_36->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_36->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }

}


void oneWindow::on_pushButton_37_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[36].branch.choose == YES)
        {
            Motor1.branch_oneGroup[36].branch.choose = NO;
            //改变柱子颜色
    //        ui->progressBar_37->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_37->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[36].branch.choose = YES;
   //         ui->progressBar_37->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_37->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[36].branch.choose == YES)
        {
            Motor1.branch_twoGroup[36].branch.choose = NO;
            //改变柱子颜色
      //      ui->progressBar_37->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_37->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[36].branch.choose = YES;
     //       ui->progressBar_37->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_37->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }

}


void oneWindow::on_pushButton_38_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[37].branch.choose == YES)
        {
            Motor1.branch_oneGroup[37].branch.choose = NO;
            //改变柱子颜色
    //        ui->progressBar_38->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_38->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[37].branch.choose = YES;
   //         ui->progressBar_38->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_38->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[37].branch.choose == YES)
        {
            Motor1.branch_twoGroup[37].branch.choose = NO;
            //改变柱子颜色
  //          ui->progressBar_38->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_38->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[37].branch.choose = YES;
    //        ui->progressBar_38->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_38->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }

}


void oneWindow::on_pushButton_39_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[38].branch.choose == YES)
        {
            Motor1.branch_oneGroup[38].branch.choose = NO;
            //改变柱子颜色
    //        ui->progressBar_39->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_39->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[38].branch.choose = YES;
    //        ui->progressBar_39->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_39->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[38].branch.choose == YES)
        {
            Motor1.branch_twoGroup[38].branch.choose = NO;
            //改变柱子颜色
      //      ui->progressBar_39->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_39->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[38].branch.choose = YES;
       //     ui->progressBar_39->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_39->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }

}


void oneWindow::on_pushButton_40_clicked()
{
    if(Motor1.Motor_state == Stop)
    {
    if(Motor1.Group == 1)
    {
        if(Motor1.branch_oneGroup[39].branch.choose == YES)
        {
            Motor1.branch_oneGroup[39].branch.choose = NO;
            //改变柱子颜色
       //     ui->progressBar_40->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_40->setStyleSheet(pushButton_StyleSheet_NO);
        }else
        {
            Motor1.branch_oneGroup[39].branch.choose = YES;
        //    ui->progressBar_40->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_40->setStyleSheet(pushButton_StyleSheet_YES);
        }

    }else if(Motor1.Group == 2)
    {
        if(Motor1.branch_twoGroup[39].branch.choose == YES)
        {
            Motor1.branch_twoGroup[39].branch.choose = NO;
            //改变柱子颜色
      //      ui->progressBar_40->setStyleSheet(progressBar_StyleSheet_NO);
            ui->pushButton_40->setStyleSheet(pushButton_StyleSheet_NO);


        }else
        {
            Motor1.branch_twoGroup[39].branch.choose = YES;
    //        ui->progressBar_40->setStyleSheet(progressBar_StyleSheet_YES);
            ui->pushButton_40->setStyleSheet(pushButton_StyleSheet_YES);

        }

    }
    }

}


void oneWindow::on_pushButton_Repair_clicked()
{
    //故障已经排查按钮
    ui->widget_WarningOne->hide();
     Waining_wigFale = false;

}

