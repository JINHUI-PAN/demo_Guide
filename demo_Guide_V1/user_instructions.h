#ifndef USER_INSTRUCTIONS_H
#define USER_INSTRUCTIONS_H
#include <QMainWindow>
#include <QSettings>
#include <QFileInfo>
#include <QDebug>
#include <QTime>
#include "demo_guide.h"
#include "passwork.h"
#include "onewindow.h"
#include "configwindow.h"
#include "parameter_window.h"


namespace Ui {
class user_instructions;
}

class user_instructions : public QMainWindow
{
    Q_OBJECT

public:
    explicit user_instructions(QWidget *parent = nullptr);
    ~user_instructions();

    void Key_Config(void);      //按键初始化


protected:
    virtual void resizeEvent(QResizeEvent *event)   override;   //重写事件




private slots:
    void on_pushButton_clicked();

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

private:
    Ui::user_instructions *ui;

    //变量
    QString  project_name = "";

//    QString  CONFIG_NAME = "CONGIF.ini";

    //按键键值
    QString KeyValue = "";        //按键值

    QString PassWork0 = "";      //超级管理员密码
    QString PassWork1 = "";
    QString PassWork2 = "";
    QString UserName = "";      //爬架管理员名字
    QString UserPhone = "";     //爬架管理员电话
    QString ID = "";            //  项目ID
    QString State = "";         //项目是否被锁定 "false" 被锁定
    bool data_fale = false;     //项目过期标志 true 未过期，false 过期

    int Year = 0 ,Month = 0,Day = 0 ;
    int a[3];
    QDate DAY;


    //函数
    void  Config_init(void);
    bool eventFilter(QObject *watch ,QEvent *event);  //获取重装点击控件
    void HideControls(void);    //  隐藏控件
    void CheckProjectdate();    //检查项目是否过期
};

#endif // USER_INSTRUCTIONS_H
