#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_mainButton_clicked()
{
    //分控操作页面跳转
    this->hide();
    oneWindow *one  = new oneWindow;
    one->show();

}


void MainWindow::on_setButton_clicked()
{
    //参数设置页面跳转
    this->hide();
    configWindow *config = new configWindow;
    config->show();
}


void MainWindow::on_remoteButton_clicked()
{
    //遥控设置
    this->hide();
    RemoteControl *remote = new RemoteControl;
    remote->show();

}


void MainWindow::on_diaryButton_clicked()
{
    //日记查询
    this->hide();
    DiaryWindow *diary = new DiaryWindow;
    diary->show();


}


void MainWindow::on_projectButton_clicked()
{
    //项目信息
    this->hide();
    Project_information *project = new Project_information;
    project->show();

}


void MainWindow::on_systemButton_clicked()
{
    //系统信息
    this->hide();
    Syetem_information *system = new Syetem_information;
    system->show();

}

