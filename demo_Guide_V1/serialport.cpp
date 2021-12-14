#include "serialport.h"
#include "ui_serialport.h"

//QSerialPort *m_serial;  //全局串口对象
//QSerialPort *m_serial1;  //全局串口对象
//QByteArray DataBuf;
QString now_data;    //当前时间
int ID_location = 0;                                        //地址
Motor_dev Motor1;                                           //结构体
Motor_dev Motor1_new;                                       //扫描结构体
Motor_dev Motor_clear;                                      //结构体数据清零
Parameter_SET  paramemter_config;                           //功能配置结构体
User_data User_information;
QString  CONFIG_oneGroupNAME = "Setting_oneData.ini";
QString  CONFIG_twoGroupNAME = "Setting_twoData.ini";
QString  CONFIG_NAME = "CONGIF.ini";
QString  Config_parameter = "Parameter_config.ini";         //功能配置文件名称
QString  DIARY_OPERRATION  = "Operration.ini";    //操作日志
QString  DIARY_FAULT  = "Fault.ini";              //错误日志
QString  DIARY_RUNNING  = "Running.ini";          //运行日志


Serialport::Serialport(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Serialport)
{
    ui->setupUi(this);
    Initialize_Data();

    //串口
    m_serial = new QSerialPort("COM3");  //实例化串口对象
    //串口2初始化
    m_serial2 = new QSerialPort(Serial_port2);  //实例化串口对象
    Serialport1();
    Serialport2();
   // connect(&demo,&demo_guide::opendSerial,this,&Serialport::Serialport1);
    //定时器
    timer1 = new QTimer(this);   //定时更新
    connect(timer1,SIGNAL(timeout()),this,SLOT(TimeOut1()));
    timer1->setTimerType(Qt::PreciseTimer);     //保持毫秒精度
    timer1->start(10);  //20ms秒中断一次

    timer2 = new QTimer(this);   //定时更新
    connect(timer2,SIGNAL(timeout()),this,SLOT(TimeOut2()));
    timer2->start(1000);  //一秒中断一次

    //获取当前时间
    QDateTime current_date_time = QDateTime::currentDateTime();
    now_data = current_date_time.toString("yyyy/MM/dd.hh:mm:ss  ddd");

}

Serialport::~Serialport()
{
    delete timer1;
    delete timer2;
    delete m_serial;
    delete m_serial2;
    delete ui;
}


void Serialport::Serialport1()
{
    //初始化串口

    // QSerialPort("/dev/ttyUSB0")
    // m_serial->setPort("COM3");

    if (m_serial->isOpen())    //已经打开先关闭
    {
        m_serial->close();
        qDebug() << "serial had open";
        return;

    }


    if(m_serial->open(QIODevice::ReadWrite))  //用读写模式打开串口
    {
        qDebug() <<"serial open";
        //打开成功
        serial1_fale = true ;//成功连接串口
        m_serial->setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections); //设置波特率和读写方向
        m_serial->setDataBits(QSerialPort::Data8);    //数据位8
        m_serial->setFlowControl(QSerialPort::NoFlowControl); //无流控制
        m_serial->setParity(QSerialPort::NoParity);   //无校验
        m_serial->setStopBits(QSerialPort::OneStop);   //一个停止位
        connect(m_serial,SIGNAL(readyRead()),this,SLOT(ReceiveInfo1())); //定义槽函数，接收数据
        QObject::connect(m_serial, static_cast<void (QSerialPort::*)
                    (QSerialPort::SerialPortError)>(&QSerialPort::error),
                    this, &Serialport::handleSerialError1);


    }else
    {
       // qDebug() <<"serial open flase";
        return;
    }
}

void Serialport::Serialport2()
{

    // QSerialPort("/dev/ttyUSB0")
    // m_serial->setPort("COM3");

    if (m_serial2->isOpen())    //已经打开先关闭
    {
        m_serial2->close();
        qDebug() << "serial had open";
        return;

    }


    if(m_serial2->open(QIODevice::ReadWrite))  //用读写模式打开串口
    {
        qDebug() <<"serial2 open";
        //打开成功
        serial2_fale = true;          //成功连接串口2
        m_serial2->setBaudRate(QSerialPort::Baud115200,QSerialPort::AllDirections); //设置波特率和读写方向
        m_serial2->setDataBits(QSerialPort::Data8);    //数据位8
        m_serial2->setFlowControl(QSerialPort::NoFlowControl); //无流控制
        m_serial2->setParity(QSerialPort::NoParity);   //无校验
        m_serial2->setStopBits(QSerialPort::OneStop);   //一个停止位
       connect(m_serial2,SIGNAL(readyRead()),this,SLOT(ReceiveInfo2())); //定义槽函数，接收数据

       QObject::connect(m_serial2, static_cast<void (QSerialPort::*)
                         (QSerialPort::SerialPortError)>(&QSerialPort::error),
                         this, &Serialport::handleSerialError2);

    }else
    {
       // qDebug() <<"serial2 open flase";
        return;
    }

}

void Serialport::Initialize_Data()
{
    //结构体数据初始化
    Motor1.login = MASTER_SIGN_OFF;     //未登录
    Motor1.Remote_lock = LOCK;          //初始化锁定遥控
    Motor1.power = POWER_OFF;
    Motor1.Motor_state = Stop;
    Motor1.Group = 1;

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
   settings.setValue("com1","/dev/ttyUSB0");
   settings.setValue("com2","/dev/ttyUSB1");
   settings.endGroup();
   }

   //读取配置
   QSettings settingsread(CONFIG_NAME,QSettings::IniFormat);
   Serial_port1 = settingsread.value("Seting/com1").toString();
   Serial_port2 = settingsread.value("Seting/com2").toString();

   //读取配置
   QFileInfo file_one(CONFIG_oneGroupNAME);
    if(file_one.isFile())       //判断文件如果存在 ,
    {
        QSettings settingsread(CONFIG_oneGroupNAME,QSettings::IniFormat);

        Motor1.page1_num = settingsread.value("number/page1_num").toInt();          //获取数量
        if(Motor1.page1_num >PAGE_Motor_max)
        {
            Motor1.page1_num = PAGE_Motor_max;
        }

        for (int i = 0 ;i < Motor1.page1_num ;i++ )
        {
            Motor1.branch_oneGroup[i].branch.id = settingsread.value(QString("%1/id").arg(i)).toInt();
            Motor1.branch_oneGroup[i].parameter.Only_ID  = settingsread.value(QString("%1/only_id").arg(i)).toString();
            Motor1.branch_oneGroup[i].parameter.overload_value  = settingsread.value(QString("%1/load").arg(i)).toInt();
            Motor1.branch_oneGroup[i].parameter.loss_value  = settingsread.value(QString("%1/loss").arg(i)).toInt();
            Motor1.branch_oneGroup[i].parameter.tighten_value  = settingsread.value(QString("%1/tight").arg(i)).toInt();
            Motor1.branch_oneGroup[i].parameter.loose_value  = settingsread.value(QString("%1/loose").arg(i)).toInt();
            Motor1.branch_oneGroup[i].parameter.current_value  = settingsread.value(QString("%1/current").arg(i)).toInt();
            //初始化状态
            Motor1.branch_oneGroup[i].branch.state = Stop;
            Motor1.branch_oneGroup[i].branch.choose = YES;
            Motor1.page1_id[i] =  Motor1.branch_oneGroup[i].branch.id;


        }

//        Motor1.page1_num = 40;
//        for (int i = 1;i <Motor1.page1_num ;i++ )
//        {
//            Motor1.branch_oneGroup[i].branch.state = Stop;
//            Motor1.branch_oneGroup[i].branch.choose = YES;
//            Motor1.page1_id[i] =  i;
//           Motor1.branch_oneGroup[i].branch.id = i;

//        }



    }

    QFileInfo file_two(CONFIG_twoGroupNAME);
    if(file_two.isFile())       //判断文件如果存在 ,
    {
        QSettings settingsread(CONFIG_twoGroupNAME,QSettings::IniFormat);

         Motor1.page2_num = settingsread.value("number/page2_num").toInt();          //获取数量
         if(Motor1.page2_num > PAGE_Motor_max)
         {
             Motor1.page2_num = PAGE_Motor_max;
         }

        for (int i = 0 ;i < Motor1.page2_num ;i++ )
        {
            Motor1.branch_twoGroup[i].branch.id = settingsread.value(QString("%1/id").arg(i)).toInt();
            Motor1.branch_twoGroup[i].parameter.Only_ID  = settingsread.value(QString("%1/only_id").arg(i)).toString();
            Motor1.branch_twoGroup[i].parameter.overload_value  = settingsread.value(QString("%1/load").arg(i)).toInt();
            Motor1.branch_twoGroup[i].parameter.loss_value  = settingsread.value(QString("%1/loss").arg(i)).toInt();
            Motor1.branch_twoGroup[i].parameter.tighten_value  = settingsread.value(QString("%1/tight").arg(i)).toInt();
            Motor1.branch_twoGroup[i].parameter.loose_value  = settingsread.value(QString("%1/loose").arg(i)).toInt();
            Motor1.branch_twoGroup[i].parameter.current_value  = settingsread.value(QString("%1/current").arg(i)).toInt();

            //初始化状态
            Motor1.branch_twoGroup[i].branch.state = Stop;
            Motor1.branch_twoGroup[i].branch.choose = YES;
            Motor1.page2_id[i] = Motor1.branch_twoGroup[i].branch.id;

        }

        Motor1.all_num = Motor1.page1_num +Motor1.page2_num;   //总数量

    }


   //写操作日记
   QFileInfo file_opr(DIARY_OPERRATION);

   if(!file_opr.isFile())       //判断文件如果不存在 ,就创建默认值文件
   {
   QSettings settings(DIARY_OPERRATION,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
   settings.beginGroup("number");
   settings.setValue("all",0);
   settings.setValue("now",0);
   settings.endGroup();

   }

   //写错误日记
   QFileInfo file_fault(DIARY_FAULT);

   if(!file_fault.isFile())       //判断文件如果不存在 ,就创建默认值文件
   {
   QSettings settings(DIARY_FAULT,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
   settings.beginGroup("number");
   settings.setValue("all",0);
   settings.setValue("now",0);
   settings.endGroup();

   }

   //运行日记
   QFileInfo file_running(DIARY_RUNNING);

   if(!file_running.isFile())       //判断文件如果不存在 ,就创建默认值文件
   {
   QSettings settings(DIARY_RUNNING,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
   settings.beginGroup("number");
   settings.setValue("all",0);
   settings.setValue("now",0);
   settings.endGroup();

   }

   //读取参数配置
   QFileInfo file_parameter(Config_parameter);

   if(!file_parameter.isFile())       //判断文件如果不存在 ,就创建默认值文件
   {
     QSettings settings(Config_parameter,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
    settings.beginGroup("Setting");
    settings.setValue("led",0);              //0是报错led闪烁，1是常亮
    settings.setValue("loose",5);           //失载计算时间单位S，
    settings.setValue("checkCurrent",1);   //是否启用电流检测，0不启用，1启用
    settings.setValue("bolt",2);           //多久时间不通讯，判定为断线单位S
    settings.setValue("startcurrent",2);   //电机状态切换电流延迟多少毫秒检测，滤波，0表示200ms ~ 3表示500ms
    settings.setValue("curentMAX",2);     //电流检测峰值，0表示30 ~ 4表示50
    settings.setValue("runCurrent",2);    //电机状态切换电流延迟多少毫秒检测，滤波，0表示200ms ~ 3表示500ms
    settings.setValue("baud",2);          //是否启用电流检测，0波特率9600，1 57600 , 2 115200
    settings.endGroup();

    //初始化结构体
    paramemter_config.led = 0 ;
    paramemter_config.loose = 5;
    paramemter_config.checkcurrent = 1;
    paramemter_config.bolt = 4;
    paramemter_config.startcurrent = 2;
    paramemter_config.curentMAX   = 2;
    paramemter_config.runCurrent = 2;
    paramemter_config.baud = 2;
   }else               //文件存在
   {
        QSettings readsettings(Config_parameter,QSettings::IniFormat);            //当前路径直接写名字，绝对路径需要绝对路径
        paramemter_config.led =  readsettings.value("Setting/led").toInt();
        paramemter_config.loose = readsettings.value("Setting/loose").toInt();
        paramemter_config.checkcurrent = readsettings.value("Setting/checkCurrent").toInt();
        paramemter_config.bolt = readsettings.value("Setting/bolt").toInt();
        paramemter_config.startcurrent = readsettings.value("Setting/startcurrent").toInt();
        paramemter_config.curentMAX   = readsettings.value("Setting/curentMAX").toInt();
        paramemter_config.runCurrent = readsettings.value("Setting/runCurrent").toInt();
        paramemter_config.baud = readsettings.value("Setting/baud").toInt();

   }
}

int Serialport::Find_ID(int ID, int length, uchar *data)
{
    //查找数组的ID位置
        //查找ID

        for (int i = 0;i <length ;i++ )
        {
            if(data[i] == ID)
            {
                return  i;
            }

        }

        return -1;

}

void Serialport:: ReceiveInfo1()
{
    //接收数据
  //  QByteArray oneData;
    QByteArray ID_data;
    ID_data.resize(10);
    QString ONLY_id;
    int length = 0;
    oneData.append( m_serial->readAll());
//    qDebug() << "DataBuf:" << oneData.toHex();    // 打印接收数据
    char *data = oneData.data();
    length = oneData.length();
    uchar *udata = (uchar*)data;
    for (int i = 0; i< length ;i++ )
    {
        if(udata[i] == 0xFF)
        {
            uint16_t CRC_data = CRC16(&udata[i+4],udata[i+3]);
            //  qDebug() << udata[0] << udata[1]  << udata[2] << udata[3] << udata[4] <<  udata[5] << udata[6]  << udata[7] << udata[8] << udata[9] <<  udata[10] << udata[11]  << udata[12] << udata[13] << udata[14] <<  udata[15] << udata[16]  << udata[17] << udata[18] << udata[19] <<  udata[20] << udata[21]  << udata[22] << udata[23];    // 打印接收数据
            uint8_t low = CRC_data  & 0x00FF;  //低位
            uint8_t high =  CRC_data >> 8 & 0x00FF;  //高位

            //   qDebug() << udata[0] << udata[1]  << udata[2] << udata[3] << udata[4] <<  udata[5] << udata[6]  << udata[7] << udata[8] << udata[9] <<  udata[10] << udata[11]  << udata[12] << udata[13] << udata[14] <<  udata[15] << udata[16]  << udata[17] << udata[18] << udata[19] <<  udata[20] << udata[21]  << udata[22] << udata[23];    // 打印接收数据
            if(high == udata[i+4+udata[i+3]] && low == udata[i+4+udata[i+3]+1]) //检验数据 CRC校验正确
            {

                //  qDebug() << udata[0] << udata[1]  << udata[2] << udata[3] << udata[4];    // 打印接收数据
                switch (udata[i+4])          //功能码
                {

                case Scan:
                    //数据保存
                    if(Motor1_new.page1_num < PAGE_Motor_max)      //单个数量
                    {

                        // 将唯一id转换成字符
                        ID_data[0] = udata[i+5];
                        ID_data[1] = udata[i+6];
                        ID_data[2] = udata[i+7];
                        ID_data[3] = udata[i+8];
                        ID_data[4] = udata[i+9];
                        ID_data[5] = udata[i+10];
                        ID_data[6] = udata[i+11];
                        ID_data[7] = udata[i+12];
                        ID_data[8] = udata[i+13];
                        ID_data[9] = udata[i+14];

                        ONLY_id = ID_data.toHex();

                        Motor1_new.page1_id[Motor1_new.page1_num] =  udata[i+1];      //ID
                        Motor1_new.branch_oneGroup[Motor1_new.page1_num].branch.id = udata[i+1];
                        Motor1_new.all_id[Motor1_new.page1_num] =  udata[i+1];
                        Motor1_new.branch_oneGroup[Motor1_new.page1_num].parameter.Only_ID  = ONLY_id;   //唯一ID
                        Motor1_new.branch_oneGroup[Motor1_new.page1_num].parameter.reset_value = udata[i+15];
                        Motor1_new.branch_oneGroup[Motor1_new.page1_num].parameter.overload_value  = udata[i+16];
                        Motor1_new.branch_oneGroup[Motor1_new.page1_num].parameter.loss_value  = udata[i+17];
                        Motor1_new.branch_oneGroup[Motor1_new.page1_num].parameter.tighten_value  = udata[i+18];
                        Motor1_new.branch_oneGroup[Motor1_new.page1_num].parameter.loose_value  = udata[i+19];
                        Motor1_new.branch_oneGroup[Motor1_new.page1_num].parameter.current_value  = udata[i+20];
                        Motor1_new.page1_num +=1;
                        Motor1_new.all_num   +=1;

                        Scan_time[ ID_location].scan_ok = true;          //have ID

                    }
                    break;  //扫描
                case Set:break;
                    //                 case 10:;break;  //遥控绑定
                default:
                {
                    int id =Find_ID(udata[1],Motor1.page1_num,Motor1.page1_id);
                    if(id != -1)  //未找到
                    {
                        if(udata[i+8] == Wrong)          //报错 停机
                        {
                            Motor1.Motor_state = Stop;
                            //   qDebug() << "报错";
                        }else            //未报错
                        {
                            Motor1.branch_oneGroup[id].branch.mark_fale = false;  //错误标志改成

                        }
                        //数据保存
                        Motor1.branch_oneGroup[id].branch.bolt = 0;      //接收到数据，清除标志
                        Motor1.branch_oneGroup[id].branch.weight  = udata[i+5];
                        Motor1.branch_oneGroup[id].branch.current = udata[i+6];
                        Motor1.branch_oneGroup[id].branch.state   = udata[i+7];
                        Motor1.branch_oneGroup[id].branch.mark   = udata[i+8];
                        //   qDebug() <<  Motor1.branch_oneGroup[id].branch.state ;
                    }
                }break;

                }

            }

        }
    }
    oneData.clear();  //clear data
}

void Serialport::ReceiveInfo2()
{
    //串口2接收数据函数
    //接收数据
  //  QByteArray oneData;
    QByteArray ID_data;
    ID_data.resize(10);
    QString ONLY_id;
    int length = 0;
    oneData.append( m_serial2->readAll());
   // qDebug() << "DataBuf:" << oneData.toHex();    // 打印接收数据
    char *data = oneData.data();
    length = oneData.length();
    uchar *udata = (uchar*)data;
    for (int i = 0; i< length ;i++ )
    {
        if(udata[i] == 0xFF)
        {
            uint16_t CRC_data = CRC16(&udata[i+4],udata[i+3]);
            //  qDebug() << udata[0] << udata[1]  << udata[2] << udata[3] << udata[4] <<  udata[5] << udata[6]  << udata[7] << udata[8] << udata[9] <<  udata[10] << udata[11]  << udata[12] << udata[13] << udata[14] <<  udata[15] << udata[16]  << udata[17] << udata[18] << udata[19] <<  udata[20] << udata[21]  << udata[22] << udata[23];    // 打印接收数据
            uint8_t low = CRC_data  & 0x00FF;  //低位
            uint8_t high =  CRC_data >> 8 & 0x00FF;  //高位

            //   qDebug() << udata[0] << udata[1]  << udata[2] << udata[3] << udata[4] <<  udata[5] << udata[6]  << udata[7] << udata[8] << udata[9] <<  udata[10] << udata[11]  << udata[12] << udata[13] << udata[14] <<  udata[15] << udata[16]  << udata[17] << udata[18] << udata[19] <<  udata[20] << udata[21]  << udata[22] << udata[23];    // 打印接收数据
            if(high == udata[i+4+udata[i+3]] && low == udata[i+4+udata[i+3]+1]) //检验数据 CRC校验正确
            {

                //  qDebug() << udata[0] << udata[1]  << udata[2] << udata[3] << udata[4];    // 打印接收数据
                switch (udata[i+4])          //功能码
                {

                case Scan:
                    //数据保存
                    if(Motor1_new.page2_num < PAGE_Motor_max)      //单个数量
                    {

                        // 将唯一id转换成字符
                        ID_data[0] = udata[i+5];
                        ID_data[1] = udata[i+6];
                        ID_data[2] = udata[i+7];
                        ID_data[3] = udata[i+8];
                        ID_data[4] = udata[i+9];
                        ID_data[5] = udata[i+10];
                        ID_data[6] = udata[i+11];
                        ID_data[7] = udata[i+12];
                        ID_data[8] = udata[i+13];
                        ID_data[9] = udata[i+14];

                        ONLY_id = ID_data.toHex();

                        Motor1_new.page2_id[Motor1_new.page2_num] =  udata[i+1];      //ID
                        Motor1_new.branch_twoGroup[Motor1_new.page2_num].branch.id = udata[i+1];
                        Motor1_new.all_id[Motor1_new.page2_num] =  udata[i+1];
                        Motor1_new.branch_twoGroup[Motor1_new.page2_num].parameter.Only_ID  = ONLY_id;   //唯一ID
                        Motor1_new.branch_twoGroup[Motor1_new.page2_num].parameter.reset_value = udata[i+15];
                        Motor1_new.branch_twoGroup[Motor1_new.page2_num].parameter.overload_value  = udata[i+16];
                        Motor1_new.branch_twoGroup[Motor1_new.page2_num].parameter.loss_value  = udata[i+17];
                        Motor1_new.branch_twoGroup[Motor1_new.page2_num].parameter.tighten_value  = udata[i+18];
                        Motor1_new.branch_twoGroup[Motor1_new.page2_num].parameter.loose_value  = udata[i+19];
                        Motor1_new.branch_twoGroup[Motor1_new.page2_num].parameter.current_value  = udata[i+20];
                        Motor1_new.page2_num +=1;
                        Motor1_new.all_num   +=1;

                        Scan_time[ ID_location].scan_ok = true;          //have ID

                    }
                    break;  //扫描
                case Set:break;
                    //                 case 10:;break;  //遥控绑定
                default:
                {
                    int id =Find_ID(udata[1],Motor1.page2_num,Motor1.page2_id);
                    if(id != -1)  //未找到
                    {
                        if(udata[i+8] == Wrong)          //报错 停机
                        {
                            Motor1.Motor_state = Stop;
                            //   qDebug() << "报错";
                        }else            //未报错
                        {
                            Motor1.branch_twoGroup[id].branch.mark_fale = false;  //错误标志改成

                        }
                        //数据保存
                        Motor1.branch_twoGroup[id].branch.bolt = 0;      //接收到数据，清除标志
                        Motor1.branch_twoGroup[id].branch.weight  = udata[i+5];
                        Motor1.branch_twoGroup[id].branch.current = udata[i+6];
                        Motor1.branch_twoGroup[id].branch.state   = udata[i+7];
                        Motor1.branch_twoGroup[id].branch.mark   = udata[i+8];
                        //   qDebug() <<  Motor1.branch_oneGroup[id].branch.state ;
                    }
                }break;

                }

            }

        }
    }
    oneData.clear();  //clear data

}

void Serialport::handleSerialError1(QSerialPort::SerialPortError error)
{
    //串口重连
    if (error == QSerialPort::ResourceError||error== QSerialPort::PermissionError)
    {//包括串口被拔出来的错误

            qDebug() <<  "串口1连接中断，请检查是否正确连接!";
            //serial->clear();
            m_serial->close();
            //serial->deleteLater();//串口出错，释放串口内容
            serial1_fale = false;

    }

}

void Serialport::handleSerialError2(QSerialPort::SerialPortError error)
{
    //串口重连
    if (error == QSerialPort::ResourceError||error== QSerialPort::PermissionError)
    {//包括串口被拔出来的错误

            qDebug() <<  "串口2连接中断，请检查是否正确连接!";
            //serial->clear();
            m_serial2->close();
            //serial->deleteLater();//串口出错，释放串口内容
            serial2_fale = false;


    }

}

void Serialport::TimeOut1()
{
    //定时器1中断回调 定时发送数据

    uint8_t byte[512];

    if(Motor1.Group == 1)       //1组
    {
        if(Motor1.Motor_state == Wait)
            return;

        if(Motor1.page1_num > 0)
          {
            if(ID_location >= Motor1.page1_num)
            {
                ID_location = 0;
                if(Motor1.Motor_state == Set)       //参数设置
                {
                    Motor1.Motor_state = Stop;     //
                    return;
                }
            }


           if(Motor1.branch_oneGroup[ID_location].branch.choose == YES)  //选中
            {
                if(Motor1.Motor_state == Set)       //参数设置
                {

                    byte[0] = 0xFF;                             // 起始位
                    byte[1] = Motor1.page1_id[ID_location];     //机号位
                    byte[2] = Motor1.login;                     //登录标志
                    byte[3] = 0x07;                             //数据长度
                    byte[4] = Motor1.Motor_state;               //功能能码
                    byte[5] = Motor1.branch_oneGroup[ID_location].parameter.reset_value;        //清零值
                    byte[6] = Motor1.branch_oneGroup[ID_location].parameter.overload_value;    //负载值
                    byte[7] = Motor1.branch_oneGroup[ID_location].parameter.loss_value;        //失载值
                    byte[8] = Motor1.branch_oneGroup[ID_location].parameter.tighten_value;      //紧钩值
                    byte[9] = Motor1.branch_oneGroup[ID_location].parameter.loose_value;        //松钩值
                    byte[10] = Motor1.branch_oneGroup[ID_location].parameter.current_value;      //电流值
                    byte[11] = 0x00;                              //CRC16
                    byte[12] = 0x00;                              //CRC16
                    byte[13] = 0xfe;                             //结束码

                    uint16_t CRC= CRC16(&byte[4],byte[3]);
                    byte[11] =    CRC >> 8 & 0x00FF;  //高位
                    byte[12] = CRC  & 0x00FF;  //低位
              //     uint16_t data1 =   CRC16(byte,)
                     uchar *data1 = byte;
                     char *data  = (char*)data1;

                     if(m_serial->isOpen())  //串口打开
                     {
                         m_serial->write(data,14);
                     }


                }else   //普通数据下发
                {
//                     qDebug() << QDateTime::currentDateTime();

                    byte[0] = 0xFF;                             //  起始位
                    byte[1] = Motor1.page1_id[ID_location];     //机号位
                    byte[2] = Motor1.login;                     //登录标志
                    byte[3] = 0x05;                             //数据长度
                    byte[4] = Motor1.Motor_state;               //功能能码
                    byte[5] = 0x00;                             //负载值，电流值，电葫芦标志，电葫芦异常值
                    byte[6] = 0x00;
                    byte[7] = 0x00;
                    byte[8] = 0x00;
                    byte[9] = 0x00;                             //CRC16
                    byte[10] = 0x00;
                    byte[11] = 0xfe;                             //结束码

                   uint16_t CRC= CRC16(&byte[4],5);
                   byte[9] =    CRC >> 8 & 0x00FF;  //高位
                   byte[10] = CRC  & 0x00FF;  //低位

             //     uint16_t data1 =   CRC16(byte,)
                    uchar *data1 = byte;
                    char *data  = (char*)data1;

                    if(m_serial->isOpen())  //串口打开
                    {
                        m_serial->write(data,12);

                    }
                    Motor1.branch_oneGroup[ID_location].branch.bolt++;
                    if(Motor1.branch_oneGroup[ID_location].branch.bolt > paramemter_config.bolt) //5次发送未接收数据，判定为断线bolt_time
                    {
                        Motor1.branch_oneGroup[ID_location].branch.bolt =  paramemter_config.bolt;
                        Motor1.branch_oneGroup[ID_location].branch.state = Bolt;  //设备断线
                        Motor1.branch_oneGroup[ID_location].branch.mark = Wrong;  //标志位报错
                        Motor1.branch_oneGroup[ID_location].branch.weight = 0;
                        Motor1.Motor_state = Stop;
                    }


                }

            }else           //  未选中
            {

            }
             ID_location++;

        }


    }else if(Motor1.Group == 2)     //  2组
    {
        if(Motor1.Motor_state == Wait)
            return;

        if(Motor1.page2_num >0)
        {
            if(ID_location >= Motor1.page2_num)
            {
                ID_location = 0;
                if(Motor1.Motor_state == Set)       //参数设置
                {
                    Motor1.Motor_state =Stop;
                }

            }

            if(Motor1.branch_twoGroup[ID_location].branch.choose == YES)        //选中
            {
                if(Motor1.Motor_state == Set)      //设置参数
                {
                    byte[0] = 0xFF;                             //  起始位
                    byte[1] = Motor1.page2_id[ID_location];     //机号位
                    byte[2] = Motor1.login;                     //登录标志
                    byte[3] = 0x07;                             //数据长度
                    byte[4] = Motor1.Motor_state;               //功能能码
                    byte[5] = Motor1.branch_twoGroup[ID_location].parameter.reset_value;        //清零值
                    byte[6] = Motor1.branch_twoGroup[ID_location].parameter.overload_value;    //负载值
                    byte[7] = Motor1.branch_twoGroup[ID_location].parameter.loss_value;        //失载值
                    byte[8] = Motor1.branch_twoGroup[ID_location].parameter.tighten_value;      //紧钩值
                    byte[9] = Motor1.branch_twoGroup[ID_location].parameter.loose_value;        //松钩值
                    byte[10] = Motor1.branch_twoGroup[ID_location].parameter.current_value;      //电流值
                    byte[11] = 0x00;                              //CRC16
                    byte[12] = 0x00;                              //CRC16
                    byte[13] = 0xfe;                             //结束码

                    uint16_t CRC= CRC16(&byte[4],byte[3]);
                    byte[11] =    CRC >> 8 & 0x00FF;  //高位
                    byte[12] = CRC  & 0x00FF;  //低位

              //     uint16_t data1 =   CRC16(byte,)
                     uchar *data1 = byte;
                     char *data  = (char*)data1;

                     if(m_serial2->isOpen())  //串口打开
                     {
                         m_serial2->write(data,14);
                     }


                }else
                {
                    byte[0] = 0xFF;                             //  起始位
                    byte[1] = Motor1.page2_id[ID_location];     //机号位
                    byte[2] = Motor1.login;                     //登录标志
                    byte[3] = 0x05;                             //数据长度
                    byte[4] = Motor1.Motor_state;               //功能能码
                    byte[5] = 0x00;                             //负载值，电流值，电葫芦标志，电葫芦异常值
                    byte[6] = 0x00;
                    byte[7] = 0x00;
                    byte[8] = 0x00;
                    byte[9] = 0x00;                             //CRC16
                    byte[10] = 0x00;
                    byte[11] = 0xfe;                             //结束码

                   uint16_t CRC= CRC16(&byte[4],5);
                   byte[9] =    CRC >> 8 & 0x00FF;  //高位
                   byte[10] =  CRC  & 0x00FF;  //低位

                    uchar *data1 = byte;
                    char *data  = (char*)data1;
                    if(m_serial2->isOpen())
                    {
                        m_serial2->write(data,12);
                    }

                    Motor1.branch_twoGroup[ID_location].branch.bolt++;
                    if(Motor1.branch_twoGroup[ID_location].branch.bolt >  paramemter_config.bolt) //5次发送未接收数据，判定为断线
                    {
                        Motor1.branch_twoGroup[ID_location].branch.bolt =  paramemter_config.bolt;
                        Motor1.branch_twoGroup[ID_location].branch.state = Bolt;  //设备断线
                        Motor1.branch_twoGroup[ID_location].branch.mark = Wrong;  //标志位报错
                        Motor1.branch_twoGroup[ID_location].branch.weight = 0;
                        Motor1.Motor_state = Stop;

                    }

                }


            }else       //未选中
            {

            }
            ID_location++;
        }


    }else       //未分组 可能是未扫描
    {

        if(Motor1.Motor_state == Scan)  //状态是扫描
        {
            if(ID_location > Last_ID)  //到达最大扫描个数
            {
                Motor1.Motor_state = Stop;  //停止
                ID_location = 0;
                for (int i = 0 ; i < 151 ; i++ )    //clear
                {
                     Scan_time[i] = Scan_clear;
                }

                return;
            }

            if(Scan_time[ID_location].scan_ok == true || Scan_time[ID_location].sacn_time > 2)
            {
                 ID_location++;
            }


            byte[0] = 0xFF;                             //  起始位
            byte[1] = ID_location;     //机号位
            byte[2] = Motor1.login;                     //登录标志
            byte[3] = 0x05;                             //数据长度
            byte[4] = Motor1.Motor_state;               //功能能码
            byte[5] = 0x00;                             //负载值，电流值，电葫芦标志，电葫芦异常值
            byte[6] = 0x00;
            byte[7] = 0x00;
            byte[8] = 0x00;
            byte[9] = 0x00;                             //CRC16
            byte[10] = 0x00;
            byte[11] = 0xfe;                             //结束码

           uint16_t CRC= CRC16(&byte[4],5);
           byte[9] =    CRC >> 8 & 0x00FF;  //高位
           byte[10] = CRC  & 0x00FF;  //低位
            uchar *data1 = byte;
            char *data  = (char*)data1;
            if(m_serial->isOpen())              //1组串口
            {
                m_serial->write(data,12);
            }
            if(m_serial2->isOpen())             //2组串口
            {
                m_serial2->write(data,12);
            }

            Scan_time[ID_location].sacn_time++;


        }else if (Motor1.Motor_state == Parameter)          //状态是功能配置
        {
            if(ID_location > Motor1.all_num)  //到达最大设置个数
            {
                Motor1.Motor_state = Stop;  //停止
                ID_location = 0;
                return;
            }

            if(ID_location < Motor1.page1_num)          //1组设置功能
            {
                byte[0] = 0xFF;                             //起始位
                byte[1] = Motor1.page1_id[ID_location];     //机号位
                byte[2] = Motor1.login;                     //登录标志
                byte[3] = 17;                             //数据长度
                byte[4] = Motor1.Motor_state;               //功能能码
                byte[5] = 0;                                //LE灯标识
                byte[6] = paramemter_config.led;            //LED
                byte[7] =  1;                               //失载计算时间标识
                byte[8] = paramemter_config.loose;          //失载
                byte[9] =  2;                                //是否电流检测标识
                byte[10] = paramemter_config.checkcurrent;   //电流检测
                byte[11] = 3;                                //与主控断线时间标识
                byte[12] = paramemter_config.bolt;           //断线时间
                byte[13] = 4;                                //电流运行
                byte[14] = paramemter_config.runCurrent;    //电流检测忽略标识
                byte[15] = 5;                                //电流峰值
                byte[16] = paramemter_config.curentMAX;    //
                byte[17] = 6;                                //电流启动
                byte[18] = paramemter_config.startcurrent;    //
                byte[19] = 7;                                //波特率
                byte[20] = paramemter_config.baud;    //

                byte[21] = 0x00;                              //CRC16 高位
                byte[22] = 0x00;                              //CRC16 低位
                byte[23] = 0xfe;                             //结束码

                uint16_t CRC= CRC16(&byte[4],byte[3]);
                byte[21] =    CRC >> 8 & 0x00FF;  //高位
                byte[22] = CRC  & 0x00FF;  //低位
          //     uint16_t data1 =   CRC16(byte,)
                 uchar *data1 = byte;
                 char *data  = (char*)data1;

                 if(m_serial->isOpen())  //串口打开
                 {
                     m_serial->write(data,24);
                 }


            }else if (ID_location >=Motor1.page1_num && ID_location < Motor1.all_num )      //2组设置功能
            {
                byte[0] = 0xFF;                             //起始位
                byte[1] = Motor1.page2_id[ID_location - Motor1.page1_num];     //机号位
                byte[2] = Motor1.login;                     //登录标志
                byte[3] = 17;                             //数据长度
                byte[4] = Motor1.Motor_state;               //功能能码
                byte[5] = 0;                                //LE灯标识
                byte[6] = paramemter_config.led;            //LED
                byte[7] =  1;                               //失载计算时间标识
                byte[8] = paramemter_config.loose;          //失载
                byte[9] =  2;                                //是否电流检测标识
                byte[10] = paramemter_config.checkcurrent;   //电流检测
                byte[11] = 3;                                //与主控断线时间标识
                byte[12] = paramemter_config.bolt;           //断线时间
                byte[13] = 4;                                //电流运行
                byte[14] = paramemter_config.runCurrent;    //电流检测忽略标识
                byte[15] = 5;                                //电流峰值
                byte[16] = paramemter_config.curentMAX;    //
                byte[17] = 6;                                //电流启动
                byte[18] = paramemter_config.startcurrent;    //
                byte[19] = 7;                                //波特率
                byte[20] = paramemter_config.baud;    //

                byte[21] = 0x00;                              //CRC16 高位
                byte[22] = 0x00;                              //CRC16 低位
                byte[23] = 0xfe;                             //结束码

                uint16_t CRC= CRC16(&byte[4],byte[3]);
                byte[21] =    CRC >> 8 & 0x00FF;  //高位
                byte[22] = CRC  & 0x00FF;  //低位
          //     uint16_t data1 =   CRC16(byte,)
                 uchar *data1 = byte;
                 char *data  = (char*)data1;

                 if(m_serial2->isOpen())  //串口打开
                 {
                     m_serial2->write(data,24);
                 }


            }
              ID_location++;



        }else if(Motor1.Motor_state == Set)  //状态是设置参数
        {
            if(ID_location > 150)  //到达最大扫描个数
            {
                Motor1.Motor_state = Stop;  //停止
                ID_location = 0;
                return;
            }
            if(ID_location < Motor1.page1_num)          //1组设置参数
            {
                byte[0] = 0xFF;                             //  起始位
                byte[1] = Motor1.page1_id[ID_location];     //机号位
                byte[2] = Motor1.login;                     //登录标志
                byte[3] = 0x07;                             //数据长度
                byte[4] = Motor1.Motor_state;               //功能能码
                byte[5] = Motor1.branch_oneGroup[ID_location].parameter.reset_value;        //清零值
                byte[6] = Motor1.branch_oneGroup[ID_location].parameter.overload_value;    //负载值
                byte[7] = Motor1.branch_oneGroup[ID_location].parameter.loss_value;        //失载值
                byte[8] = Motor1.branch_oneGroup[ID_location].parameter.tighten_value;      //紧钩值
                byte[9] = Motor1.branch_oneGroup[ID_location].parameter.loose_value;        //松钩值
                byte[10] = Motor1.branch_oneGroup[ID_location].parameter.current_value;      //电流值
                byte[11] = 0x00;                              //CRC16 高位
                byte[12] = 0x00;                              //CRC16 低位
                byte[13] = 0xfe;                             //结束码

                uint16_t CRC= CRC16(&byte[4],byte[3]);
                byte[11] =    CRC >> 8 & 0x00FF;  //高位
                byte[12] = CRC  & 0x00FF;  //低位
          //     uint16_t data1 =   CRC16(byte,)
                 uchar *data1 = byte;
                 char *data  = (char*)data1;

                 if(m_serial->isOpen())  //串口打开
                 {
                     m_serial->write(data,14);
                 }


            }else if (ID_location >=Motor1.page1_num && ID_location < Motor1.all_num )
            {
                if(Motor1.branch_twoGroup[ID_location].branch.choose == YES)        //选中
                {
                    byte[0] = 0xFF;                             //起始位
                    byte[1] = Motor1.page2_id[ID_location - Motor1.page1_num];     //机号位
                    byte[2] = Motor1.login;                     //登录标志
                    byte[3] = 0x07;                             //数据长度
                    byte[4] = Motor1.Motor_state;               //功能能码
                    byte[5] = Motor1.branch_twoGroup[ID_location].parameter.reset_value;        //清零值
                    byte[6] = Motor1.branch_twoGroup[ID_location].parameter.overload_value;    //负载值
                    byte[7] = Motor1.branch_twoGroup[ID_location].parameter.loss_value;        //失载值
                    byte[8] = Motor1.branch_twoGroup[ID_location].parameter.tighten_value;      //紧钩值
                    byte[9] = Motor1.branch_twoGroup[ID_location].parameter.loose_value;        //松钩值
                    byte[10] = Motor1.branch_twoGroup[ID_location].parameter.current_value;      //电流值
                    byte[11] = 0x00;                              //CRC16
                    byte[12] = 0x00;                              //CRC16
                    byte[13] = 0xfe;                             //结束码

                    uint16_t CRC= CRC16(&byte[4],byte[3]);
                    byte[11] =    CRC >> 8 & 0x00FF;  //高位
                    byte[12] = CRC  & 0x00FF;  //低位

              //     uint16_t data1 =   CRC16(byte,)
                     uchar *data1 = byte;
                     char *data  = (char*)data1;

                     if(m_serial2->isOpen())  //串口打开
                     {
                         m_serial2->write(data,14);
                     }

                }

            }
             ID_location++;

        }



    }

}

void Serialport::TimeOut2()
{
    //定时器2回调
    //    ui->dateTimeEdit->setDateTime( QDateTime::currentDateTime() );
        QDateTime current_date_time = QDateTime::currentDateTime();
        now_data = current_date_time.toString("yyyy/MM/dd.hh:mm:ss  ddd");
      //  "2021.11.11.13:53:16.202 周四"


        if(serial2_fale == false)
        {
            Serialport2();      //重连2串口
        }

        if(serial1_fale == false)
        {
            Serialport1();           //重连1串口
        }



}


uint16_t Serialport::CRC16(uint8_t *Pushdata, uint8_t length)
{
    /* 查表法CRC16    高位在前 低位在后*/
    /* 查表法CRC16 */
        uint8_t uchCRCHi = 0xFF;
        uint8_t uchCRCLo = 0xFF;
        uint8_t uIndex;
        while (length--)
        {
            uIndex = uchCRCLo^*Pushdata++;
            uchCRCLo = uchCRCHi^auchCRCLo[uIndex];
            uchCRCHi = auchCRCHi[uIndex];
         }
        return (uchCRCHi << 8 | uchCRCLo);



}

