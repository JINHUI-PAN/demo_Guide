#include "diarywindow.h"
#include "ui_diarywindow.h"

DiaryWindow::DiaryWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::DiaryWindow)
{
    ui->setupUi(this);
    this->setStyleSheet("#DiaryWindow{background-image: url(:/back/Image/button/config.png);}");   //设置底图

    timer2 = new QTimer(this);   //定时更新
    connect(timer2,SIGNAL(timeout()),this,SLOT(TimeOut2()));
    timer2->start(1000);      //一秒中断一次

    //初始化
     tablewigetSet_Run(page_running);
     breakbook = Run;
     ui->pushButton_operration->setStyleSheet("QPushButton{color: rgb(255, 255, 255); background-color: rgb(9, 82, 120);}");
     ui->pushButton_Running->setStyleSheet("QPushButton{color: rgb(0, 0, 0);background-color: rgb(240, 240, 240);}");
     ui->pushButton_fault->setStyleSheet("QPushButton{color: rgb(0, 0, 0); background-color: rgb(240, 240, 240);}");
}

DiaryWindow::~DiaryWindow()
{
    delete timer2;
    delete ui;
}

void DiaryWindow:: tablewigetSet_Run(int page)
{


    //表格初始化
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setRowCount(setRowCount_num);
   // ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    //列宽自适应

    //设置列宽
    ui->tableWidget->setColumnWidth(0,100);
    ui->tableWidget->setColumnWidth(1,100);
    ui->tableWidget->setColumnWidth(2,100);
    ui->tableWidget->setColumnWidth(3,100);
    ui->tableWidget->setColumnWidth(4,250);
    ui->tableWidget->setColumnWidth(5,100);
    ui->tableWidget->setColumnWidth(6,205);


    ui->tableWidget->setWindowTitle("DEMO");
    QStringList header;
    header<<"操作设备"<<"操作动作"<< "组名称" << "分控数量" << "机位号"<< "角色" << "操作时间";
    ui->tableWidget->setHorizontalHeaderLabels(header);    //设置头标题
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    QString data;
    //写配置
  //  QFileInfo file(DIARY_RUNNING);
    QSettings settingsread(DIARY_RUNNING,QSettings::IniFormat);
    //     int Num = settingsread.value("number/now").toInt();

    for (int i = setRowCount_num*(page - 1) ; i < page*setRowCount_num ;i++ )
    {
        QTableWidgetItem *item1 = new QTableWidgetItem(settingsread.value(QString("%1/equipment").arg(i+1)).toString());        //操作设备

        QTableWidgetItem *item2 = new QTableWidgetItem(settingsread.value(QString("%1/action").arg(i+1)).toString());        //  操作动作

        QTableWidgetItem *item3 = new QTableWidgetItem(settingsread.value(QString("%1/group").arg(i+1)).toString());        //组名称


        QTableWidgetItem *item4 = new QTableWidgetItem(settingsread.value(QString("%1/number").arg(i+1)).toString());         //分控数量


        QTableWidgetItem *item5 = new QTableWidgetItem(settingsread.value(QString("%1/numberID").arg(i+1)).toString());           //机位号


        QTableWidgetItem *item6 = new QTableWidgetItem(settingsread.value(QString("%1/role").arg(i+1)).toString());              //角色


        QTableWidgetItem *item7 = new QTableWidgetItem(settingsread.value(QString("%1/time").arg(i+1)).toString());            //操作时间

        item1->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - setRowCount_num * (page-1),0,item1);

        item2->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - setRowCount_num * (page-1),1,item2);

        item3->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - setRowCount_num * (page-1),2,item3);

        item4->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - setRowCount_num * (page-1),3,item4);

        item5->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - setRowCount_num * (page-1),4,item5);

        item6->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - setRowCount_num * (page-1),5,item6);

        item7->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - setRowCount_num * (page-1),6,item7);
    }

}

void DiaryWindow::tablewigetSet_Operation(int page)
{
    //操作日志
    //表格初始化
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setRowCount(setRowCount_num);
   // ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    //列宽自适应

    //设置列宽
    ui->tableWidget->setColumnWidth(0,100);
    ui->tableWidget->setColumnWidth(1,100);
    ui->tableWidget->setColumnWidth(2,100);
    ui->tableWidget->setColumnWidth(3,100);
    ui->tableWidget->setColumnWidth(4,250);
    ui->tableWidget->setColumnWidth(5,100);
    ui->tableWidget->setColumnWidth(6,205);


    ui->tableWidget->setWindowTitle("DEMO");
    QStringList header;
    header<<"操作设备"<<"操作动作"<< "组名称" << "分控数量" << "机位号"<< "角色" << "操作时间";
    ui->tableWidget->setHorizontalHeaderLabels(header);    //设置头标题
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    QString data;
    //写配置
 //   QFileInfo file(DIARY_RUNNING);
    QSettings settingsread(DIARY_RUNNING,QSettings::IniFormat);
    //     int Num = settingsread.value("number/now").toInt();

    for (int i = setRowCount_num*(page - 1) ; i < page*setRowCount_num ;i++ )
    {
        QTableWidgetItem *item1 = new QTableWidgetItem(settingsread.value(QString("%1/equipment").arg(i+1)).toString());        //操作设备

        QTableWidgetItem *item2 = new QTableWidgetItem(settingsread.value(QString("%1/action").arg(i+1)).toString());        //  操作动作

        QTableWidgetItem *item3 = new QTableWidgetItem(settingsread.value(QString("%1/group").arg(i+1)).toString());        //组名称


        QTableWidgetItem *item4 = new QTableWidgetItem(settingsread.value(QString("%1/number").arg(i+1)).toString());         //分控数量


        QTableWidgetItem *item5 = new QTableWidgetItem(settingsread.value(QString("%1/numberID").arg(i+1)).toString());           //机位号


        QTableWidgetItem *item6 = new QTableWidgetItem(settingsread.value(QString("%1/role").arg(i+1)).toString());              //角色


        QTableWidgetItem *item7 = new QTableWidgetItem(settingsread.value(QString("%1/time").arg(i+1)).toString());            //操作时间

        item1->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - setRowCount_num * (page-1),0,item1);

        item2->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - setRowCount_num * (page-1),1,item2);

        item3->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - setRowCount_num * (page-1),2,item3);

        item4->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - setRowCount_num * (page-1),3,item4);

        item5->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - setRowCount_num * (page-1),4,item5);

        item6->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - setRowCount_num * (page-1),5,item6);

        item7->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - setRowCount_num * (page-1),6,item7);

    }

}

void DiaryWindow::tablewigetSet_Fault(int page)
{

    //故障日记
    //表格初始化
    ui->tableWidget->setColumnCount(7);
    ui->tableWidget->setRowCount(setRowCount_num);
   // ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);    //列宽自适应

    //设置列宽
    ui->tableWidget->setColumnWidth(0,100);
    ui->tableWidget->setColumnWidth(1,100);
    ui->tableWidget->setColumnWidth(2,100);
    ui->tableWidget->setColumnWidth(3,100);
    ui->tableWidget->setColumnWidth(4,250);
    ui->tableWidget->setColumnWidth(5,100);
    ui->tableWidget->setColumnWidth(6,205);


    ui->tableWidget->setWindowTitle("DEMO");
    QStringList header;
    header<<"报警机位"<<"报警类型"<< "报警值" << "操作动作" << "操作方式"<< "角色" << "操作时间";
    ui->tableWidget->setHorizontalHeaderLabels(header);    //设置头标题
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    //写配置
 //   QFileInfo file(DIARY_FAULT);
    QSettings settingsread(DIARY_FAULT,QSettings::IniFormat);
    //     int Num = settingsread.value("number/now").toInt();
    for (int i = setRowCount_num*(page - 1) ; i < page*setRowCount_num  ;i++ )
    {
        QTableWidgetItem *item1 = new QTableWidgetItem(settingsread.value(QString("%1/ID").arg(i+1)).toString());        //操作设备

        QTableWidgetItem *item2 = new QTableWidgetItem(settingsread.value(QString("%1/Fault").arg(i+1)).toString());        //  操作动作

        QTableWidgetItem *item3 = new QTableWidgetItem(settingsread.value(QString("%1/Data").arg(i+1)).toString());        //组名称


        QTableWidgetItem *item4 = new QTableWidgetItem(settingsread.value(QString("%1/Action").arg(i+1)).toString());         //分控数量


        QTableWidgetItem *item5 = new QTableWidgetItem(settingsread.value(QString("%1/Howto").arg(i+1)).toString());           //机位号


        QTableWidgetItem *item6 = new QTableWidgetItem(settingsread.value(QString("%1/User").arg(i+1)).toString());              //角色


        QTableWidgetItem *item7 = new QTableWidgetItem(settingsread.value(QString("%1/time").arg(i+1)).toString());            //操作时间

        item1->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - setRowCount_num * (page-1) ,0,item1);

        item2->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - setRowCount_num * (page-1) ,1,item2);

        item3->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - setRowCount_num * (page-1) ,2,item3);

        item4->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - setRowCount_num * (page-1) ,3,item4);

        item5->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - setRowCount_num * (page-1) ,4,item5);

        item6->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - setRowCount_num * (page-1) ,5,item6);

        item7->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - setRowCount_num * (page-1) ,6,item7);


    }



}

void DiaryWindow::TimeOut2()
{
    //定时器回调
     ui->label_time->setText(now_data);  //更新时间
}




void DiaryWindow::on_pushButton_operration_clicked()
{
    //操作日志
    ui->tableWidget->clearContents();
    page_running = 1;
    breakbook = Operration;
    tablewigetSet_Run(page_running);
    ui->pushButton_operration->setStyleSheet("QPushButton{color: rgb(255, 255, 255); background-color: rgb(9, 82, 120);}");
    ui->pushButton_Running->setStyleSheet("QPushButton{color: rgb(0, 0, 0);background-color: rgb(240, 240, 240);}");
    ui->pushButton_fault->setStyleSheet("QPushButton{color: rgb(0, 0, 0); background-color: rgb(240, 240, 240);}");
    ui->label_page->setText(QString("第%1页").arg(page_running));

}


void DiaryWindow::on_pushButton_fault_clicked()
{
    //故障日志
    ui->tableWidget->clear();
    page_running = 1;
    breakbook = Fault;
    tablewigetSet_Fault(page_running);
    ui->pushButton_operration->setStyleSheet("QPushButton{color: rgb(0, 0, 0);background-color: rgb(240, 240, 240);}");
    ui->pushButton_Running->setStyleSheet("QPushButton{color: rgb(0, 0, 0);background-color: rgb(240, 240, 240);}");
    ui->pushButton_fault->setStyleSheet("QPushButton{color: rgb(255, 255, 255);  background-color: rgb(9, 82, 120);}");
    ui->label_page->setText(QString("第%1页").arg(page_running));

}
void DiaryWindow::on_pushButton_Running_clicked()
{
    //运行日志
    ui->tableWidget->clearContents();
    page_running = 1;
    breakbook = Run;        //选中日记
    tablewigetSet_Run(page_running);
    ui->pushButton_operration->setStyleSheet("QPushButton{color: rgb(0, 0, 0);background-color: rgb(240, 240, 240);}");
    ui->pushButton_Running->setStyleSheet("QPushButton{color: rgb(255, 255, 255);background-color: rgb(9, 82, 120);}");
    ui->pushButton_fault->setStyleSheet("QPushButton{color: rgb(0, 0, 0); background-color: rgb(240, 240, 240);}");
    ui->label_page->setText(QString("第%1页").arg(page_running));


}


void DiaryWindow::on_pushButton_previousPage_clicked()
{
    //上一页
    if(page_running == 1)
    {

    }else
    {
        page_running--;
        switch (breakbook)
        {
        case Run:  ui->tableWidget->clearContents();tablewigetSet_Run(page_running);break;
        case Operration: ui->tableWidget->clearContents();tablewigetSet_Operation(page_running);break;
        case Fault: ui->tableWidget->clearContents();tablewigetSet_Fault(page_running);break;

        }

        ui->label_page->setText(QString("第%1页").arg(page_running));

}

}


void DiaryWindow::on_pushButton_lastPage_clicked()
{
    //下一页
    QSettings settingsread(DIARY_RUNNING,QSettings::IniFormat);
    int run = settingsread.value("number/all").toInt();

    QSettings settingsread1(DIARY_OPERRATION,QSettings::IniFormat);
    int opera = settingsread1.value("number/all").toInt();

    QSettings settingsread2(DIARY_FAULT,QSettings::IniFormat);
    int fault = settingsread2.value("number/all").toInt();
    run = run/setRowCount_num +1 ;
    opera = opera/setRowCount_num +1;
    fault =  fault/setRowCount_num +1;

    switch (breakbook)
    {
    case Run:if( page_running == run )
        {

        }else
        {
            page_running++;
             ui->tableWidget->clearContents();
            tablewigetSet_Run(page_running);

        }break;


    case Operration:if( page_running == opera )
        {

        }else
        {
            page_running++;
             ui->tableWidget->clearContents();
            tablewigetSet_Operation(page_running);

        }break;

    case Fault:if( page_running == fault )
        {

        }else
        {
            page_running++;
             ui->tableWidget->clearContents();
            tablewigetSet_Fault(page_running);

        }break;

    }
      ui->label_page->setText(QString("第%1页").arg(page_running));


}



//页面切换
void DiaryWindow::on_pushButton_control_clicked()
{
    //分控操作页面

        this->close();
        oneWindow *one = new oneWindow;
        one->show();

}


void DiaryWindow::on_pushButton_oneConfig_clicked()
{
    //分控设置页面

         this->close();
        configWindow *config = new configWindow;
        config->show();


}

void DiaryWindow::on_pushButton_remote_clicked()
{
    //遥控设置
    this->close();
    remoteSet *remote = new remoteSet;
    remote->show();


}

//void DiaryWindow::on_pushButton_dairy_clicked()
//{
//    //操作日志

//        this->close();
//        DiaryWindow  *diary = new DiaryWindow;
//        diary->show();

//}


void DiaryWindow::on_pushButton_project_clicked()
{
    //项目信息

        this->close();
        Project_information  *project = new Project_information;
        project->show();


}


void DiaryWindow::on_pushButton_system_clicked()
{
    //系统信息

         this->close();
        Syetem_information *system = new Syetem_information;
        system->show();


}



