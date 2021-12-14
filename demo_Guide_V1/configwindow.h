#ifndef CONFIGWINDOW_H
#define CONFIGWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QDebug>
#include <QTreeWidget>
#include <QSettings>
#include <QFileInfo>
#include <QTimer>
#include "serialport.h"
#include "onewindow.h"
//#include "configwindow.h"
#include "diarywindow.h"
#include "remoteset.h"
#include "project_information.h"
#include "syetem_information.h"



namespace Ui {
class configWindow;
}

class configWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit configWindow(QWidget *parent = nullptr);
    ~configWindow();
    //函数
    void tablewigetSet(void);       // 参数表格设置
    void Key_Config(void);          //按键样式初始化


    void label_NumSet(int ID,QString Style);

    //变量
     QTimer *timer4;         //定时器1
     QTimer *timer2;         //定时器2
     QTimer *timer3;         //定时器3
     int Set_fale = 3;       //设置成功倒计时
     bool Save_finish = false;    //设置完成标志



     int ALL_Num = 70;       //后台分控数量
     int CheckBox_num =0 ;  // 参数修改选中个数
     double Check_double[5]; //double 数值
     int Check_int[5] = {-1,-1,-1,-1,-1} ;    //修改的参数值
     bool Check_fale[5] = {false,false,false,false,false};    //是否有修改


private slots:

     void TimeOut4();   //定时1回调函数
     void TimeOut2();   //定时2回调函数
     void TimeOut3();   //定时3回调函数


    void on_pushButton_comemain_clicked();   // 取消参数修改
    void on_pushButton_change_clicked();    //修改参数按钮

    void  getItem(QTableWidgetItem *item1);

    void on_checkBox_clicked();

     void on_pushButton_NO_clicked();

     void on_pushButton_del_clicked();

     void on_pushButton_ok_clicked();

     void on_pushButton_point_clicked();

     void on_pushButton_00_clicked();

     void on_pushButton_02_clicked();

     void on_pushButton_01_clicked();

     void on_pushButton_06_clicked();

     void on_pushButton_05_clicked();

     void on_pushButton_04_clicked();

     void on_pushButton_03_clicked();

     void on_pushButton_07_clicked();

     void on_pushButton_08_clicked();

     void on_pushButton_09_clicked();


     void on_pushButton_Scan_clicked();
     void on_pushButton_ScanOrStop_clicked();

     void on_pushButton_ScanSave_clicked();

     void on_pushButton_ScanCancel_clicked();


     void on_pushButton_allGroup_clicked();

     void on_pushButton_oneGroup_clicked();

     void on_pushButton_twoGroup_clicked();



     void on_pushButton_control_clicked();

     void on_pushButton_remote_clicked();

     void on_pushButton_project_clicked();

     void on_pushButton_system_clicked();

     void on_pushButton_dairy_clicked();

private:
    Ui::configWindow *ui;
    //变量
//    QString CONFIG_oneGroupNAME = "Setting_oneData.ini";
//    QString CONFIG_twoGroupNAME = "Setting_twoData.ini";
//   QByteArray arrray;

   QString KeyValue = "";
   int key_point = 0;


    //函数
    void Config_Setting(Motor_dev &Motor_config,QString GroupFile);      //分控参数保存
    void tablewiget_SetData(int Group,Motor_dev Motor);        //设置表格

 //   void Refarsh_Tablewiget(Motor_dev Motor_newdata);          //刷新表格数据

      bool eventFilter(QObject *watch ,QEvent *event);  //获取焦点
};

#endif // CONFIGWINDOW_H
