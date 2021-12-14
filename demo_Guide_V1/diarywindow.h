#ifndef DIARYWINDOW_H
#define DIARYWINDOW_H

#include <QMainWindow>
#include <QTableWidget>
#include <QTableWidgetItem>
#include <QTreeWidget>
#include <QHeaderView>
#include <QSettings>
#include <QFileInfo>
#include "configwindow.h"
//#include "diarywindow.h"
#include "remoteset.h"
#include "project_information.h"
#include "syetem_information.h"
#include "onewindow.h"
#include <QTimer>

namespace Ui {
class DiaryWindow;
}

class DiaryWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit DiaryWindow(QWidget *parent = nullptr);
    ~DiaryWindow();


    //函数
    void tablewigetSet_Run(int page);       // 参数表格设置
    void tablewigetSet_Operation(int page);       // 参数表格设置
    void tablewigetSet_Fault(int page);       // 参数表格设置
    QTimer *timer2;         //定时器1
    uint8_t  page_running = 1;     //第几页
    uint8_t  setRowCount_num = 10;       //每页显示数量
    uint8_t  breakbook;                 //查看哪个日记


    //枚举
    //日记
    enum {
        Run = 1,              //运行日记
        Operration,          //操作日记
        Fault               //错误日记
    };

private slots:

     void TimeOut2();   //定时1回调函数
    void on_pushButton_operration_clicked();

    void on_pushButton_fault_clicked();

    void on_pushButton_previousPage_clicked();      //上一页

    void on_pushButton_lastPage_clicked();          //下一页

    void on_pushButton_control_clicked();

    void on_pushButton_oneConfig_clicked();

    void on_pushButton_remote_clicked();

 //   void on_pushButton_dairy_clicked();

    void on_pushButton_project_clicked();

    void on_pushButton_system_clicked();

    void on_pushButton_Running_clicked();

private:
    Ui::DiaryWindow *ui;
};

#endif // DIARYWINDOW_H
