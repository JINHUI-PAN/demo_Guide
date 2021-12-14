#ifndef DEMO_GUIDE_H
#define DEMO_GUIDE_H

#include <QMainWindow>
#include <QSettings>
#include <QFileInfo>
#include <QDebug>
#include "user_instructions.h"


QT_BEGIN_NAMESPACE
namespace Ui { class demo_guide; }
QT_END_NAMESPACE

class demo_guide : public QMainWindow
{
    Q_OBJECT

public:
    demo_guide(QWidget *parent = nullptr);
    ~demo_guide();


private slots:
    void on_bandButton_clicked();

signals:
    void opendSerial(void);

private:
    Ui::demo_guide *ui;
    QString  project_name = "";
    QString  CONFIG_NAME = "CONGIF.ini";





    //函数
//    void Config_init(void);
//    void Project_init(void);
};
#endif // DEMO_GUIDE_H
