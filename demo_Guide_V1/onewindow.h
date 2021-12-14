#ifndef ONEWINDOW_H
#define ONEWINDOW_H

#include <QMainWindow>
#include <QProgressBar>
#include "QRoundProgressBar.h"
#include "serialport.h"
#include "configwindow.h"
#include "diarywindow.h"
#include "remoteset.h"
#include "project_information.h"
#include "syetem_information.h"
// #include "onewindow.h"
#include <QDebug>
#include <QTimer>
#include <QMessageBox>

namespace Ui {
class oneWindow;
}

class oneWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit oneWindow(QWidget *parent = nullptr);
    ~oneWindow();


    QTimer *timer1;         //定时器1
    QTimer *timer2;         //定时器2
    QTimer *timer3;         //定时器3

    int progerss_location = 0;      //  ID的下标位
    int YesOrNOButton_mark = YES;   //全选和反选标志


    //爬升高度相关
    int Roun_Hight =0;              //  当前爬升高度
    int time_fale =0;               //计时
    bool time_mark = false;         //  上升标志
    bool Waining_wigFale = false ;          //弹框隐藏还是消失标志 ,false 隐藏，true 已经显示
    int Motor_OldState = 0;         //判断页面状态




private slots:

     void TimeOut1();   //定时1回调函数
     void TimeOut2();   //定时2回调函数
     void TimeOut3();   //定时3回调函数


    void on_pushButton_rising_clicked();

    void on_pushButton_main_clicked();

    void on_oneTeamButton_clicked();

    void on_twoTeamButton_clicked();

    void on_pushButton_tight_clicked();

    void on_pushButton_Loose_clicked();

    void on_pushButton_control_clicked();

    void on_pushButton_oneConfig_clicked();

    void on_pushButton_remote_clicked();

    void on_pushButton_project_clicked();

    void on_pushButton_system_clicked();

    void on_pushButton_dairy_clicked();

    void on_YesOrNOButton_clicked();

     void on_pushButton_01_clicked();

     void on_pushButton_02_clicked();

     void on_pushButton_03_clicked();

     void on_pushButton_04_clicked();

     void on_pushButton_05_clicked();

     void on_pushButton_06_clicked();

     void on_pushButton_07_clicked();

     void on_pushButton_08_clicked();

     void on_pushButton_09_clicked();

     void on_pushButton_10_clicked();

     void on_pushButton_11_clicked();

     void on_pushButton_12_clicked();

     void on_pushButton_13_clicked();

     void on_pushButton_14_clicked();

     void on_pushButton_15_clicked();

     void on_pushButton_16_clicked();

     void on_pushButton_17_clicked();

     void on_pushButton_18_clicked();

     void on_pushButton_19_clicked();

     void on_pushButton_20_clicked();

     void on_pushButton_21_clicked();

     void on_pushButton_22_clicked();

     void on_pushButton_23_clicked();

     void on_pushButton_24_clicked();

     void on_pushButton_25_clicked();

     void on_pushButton_26_clicked();

     void on_pushButton_27_clicked();

     void on_pushButton_28_clicked();

     void on_pushButton_29_clicked();

     void on_pushButton_30_clicked();

     void on_pushButton_31_clicked();

     void on_pushButton_32_clicked();

     void on_pushButton_33_clicked();

     void on_pushButton_34_clicked();

     void on_pushButton_35_clicked();

     void on_pushButton_36_clicked();

     void on_pushButton_37_clicked();

     void on_pushButton_38_clicked();

     void on_pushButton_39_clicked();

     void on_pushButton_40_clicked();

     void on_pushButton_Repair_clicked();   //错误排查按钮

private:
    Ui::oneWindow *ui;

    //函数
    void RounBar_set(void);     //设置停机按钮

    void pushButton_Set(void);  //设置显示按钮

    void Icon_hide(int num);    //隐藏柱状图  num为分控数量
    void Icon_show(void);       //显示全部柱子
    bool eventFilter(QObject *watch ,QEvent *event);    //获取重装点击控件

    void ID_IconSet(Motor_dev MotorID);                     //  设置分控编号

    int  Find_ID(int ID,QByteArray array);                   //查找ID位置
    void Set_ProgressBarData( int ID,int value);            //更新显示数据
    void Set_ProgressBarState(int ID,int State);           //设置状态

    void Check_Error(int id);                            //错误弹窗更新

    void Set_Yes_StyleSheet(void);                     //设置选中状态
    void Set_NO_StyleSheet(void);                      //未选中状态
    void Update_data();                               //数据处理

    int Check_StateOK(int Group);                    //查询分控是否有错误状态
    int Check_ChooseNum(void);                       //检查选中个数
    QString Check_State(uint8_t state);             //将State转成
    QString Check_IdNumber(Motor_dev Motor_unm);        //查找选中ID数量


     //变量
     //选中和未选中进度条和控件样式
    QString progressBar_StyleSheet_YES ="QProgressBar{border-color: rgba(220, 220, 220,255);background-color: rgba(220, 220, 220,255);}" ;
    QString progressBar_StyleSheet_NO = "QProgressBar{border-color: rgba(220, 220, 220,150);background-color: rgba(220, 220, 220,150);}";
    QString pushButton_StyleSheet_YES = "QPushButton{background-color: rgb(85, 170, 255);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"  //
                                           "QPushButton:hover{background-color: rgb(85, 170, 255);color: rgb(255, 255, 255);}"  //按键本色
                                            "QPushButton:pressed{background-color: rgb(85, 170, 255);border-style: inset;}";

    QString pushButton_StyleSheet_NO = "QPushButton{background-color: rgb(220, 220, 220);border:0.5px groove gray;border-radius:15px;padding:2px 4px;border-style: outset;color: rgb(255, 255, 255);}"  //
                                                "QPushButton:hover{background-color: rgb(220, 220, 220);color: rgb(255, 255, 255);}"  //按键本色
                                                 "QPushButton:pressed{background-color: rgb(220, 220, 220);border-style: inset;}";


    //状态进度条样式
    QString Style_progress_risining = QString("QProgressBar{border-color: rgb(220, 220, 220);background-color: rgb(220, 220, 220);} QProgressBar::chunk{background:rgb(57, 172, 0)}");
    QString Style_progress_overload = QString("QProgressBar{border-color: rgb(220, 220, 220);background-color: rgb(220, 220, 220);} QProgressBar::chunk{background:rgb(255,0, 0)}");
    QString Style_label_boder_move = QString("border-color: rgb(57, 172, 0);color: rgb(57, 172, 0);");
    QString Style_label_boder_MO = QString("border-color: rgb(0, 0, 0);color: rgb(0, 0, 0);");
    QString Style_label_boder_NO = QString("border-color: rgb(255, 0, 0);color: rgb(255, 0, 0);");

    //预警状态  background-color: rgb(225, 152, 43); 黄色
    QString Style_progress_warning = QString("QProgressBar{border-color: rgb(220, 220, 220);background-color: rgb(220, 220, 220);} QProgressBar::chunk{background:rgb(225, 152, 43)}");
    QString Style_label_boder_warning = QString("border-color: rgb(225, 152, 43);color:rgb(225, 152, 43);");

};

#endif // ONEWINDOW_H
