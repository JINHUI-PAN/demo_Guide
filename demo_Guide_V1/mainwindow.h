#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "onewindow.h"
#include "configwindow.h"
#include "remotecontrol.h"
#include "diarywindow.h"
#include "project_information.h"
#include "syetem_information.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_mainButton_clicked();

    void on_setButton_clicked();

    void on_remoteButton_clicked();

    void on_diaryButton_clicked();

    void on_projectButton_clicked();

    void on_systemButton_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
