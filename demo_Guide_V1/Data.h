#ifndef DATA_H
#define DATA_H
#include<QList>
#include <QApplication>
//#include <QtSerialPort/QSerialPort>
//#include <QtSerialPort/QSerialPortInfo>


//串口通讯
//QSerialPort *m_serial;  //全局串口对象

//宏定义
#define PAGE_Motor_max  40
#define ALL_Motor_max   80
/* 主控和分控交互部分 */
//#define  STOP                   0x52            /* 读取数据或者停止操作控制码 */
//#define TIGHTEN					0x4D            /* 钩紧控制码 */
//#define RELAX					0x4E			/* 松钩控制码 */
//#define RISE					0x58			/* 上升控制码 */
//#define FALL					0x4A			/* 下降控制码 */
//#define SCAN                    0x60            /*  扫描分机号*/
//#define CONTROL_END1            0x52            /* 结束码第一位 */
//#define CONTROL_END2            0xFF            /* 结束码第二位 */

///* 分控返回状态 */
//#define NORMAL_STATE            0x50                /* 正常状态 */
//#define OVERLOAD_STATE          0x53                /* 超载状态 */
//#define UNDERLOAD_STATE         0x47                /* 欠载状态 */
//#define RELAX_FINISH_STATE		0x51                /* 松钩结束 */

//Risining,     //上升
//Falling,      //下降
// Stop          //停机

/* 广播地址 */
#define BROADCAST_ID			0xDC                /* 广播地址 */


//结构体
struct Branch{                                              /* 分控参数*/
    uint8_t id;											    /* 分控ID */
    uint8_t state;											/* 分控下状态 */
    uint8_t mark;											/* 分控返回错误标识 */
    uint8_t weight;											/* 分控承载 */
    uint8_t current;										/*  分控电流值 */
    bool choose;                                         /*  分控是否被选中*/
    uint8_t bolt;                                           /*  与主机通讯断线次数*/
//    bool checkbox = false;                                  /*  修改参数是否被选中  默认不选中*/
    bool mark_fale  = false;                                /*  错误是否被记录 */
} ;

struct Motor_parameter{                             //参数配置

    QString Only_ID ="";                            /* 唯一ID*/
    uint8_t reset_value;                            /* 清零值*/
    uint8_t overload_value;							/* 超载值 */
    uint8_t loss_value;                             /* 失载值 */
    uint8_t tighten_value;							/* 钩紧值 */
    uint8_t loose_value;                            /* 松钩值*/
    uint8_t current_value;							/* 电流值 */
    // << "过载值/T" << "失载值/T"<< "紧钩值/T" << "松钩值/T"  << "电流值/A";
} ;

/* 分控参数 */
struct branch_dev{
    struct Branch branch;								/* 分控实时参数 */
    struct Motor_parameter parameter;	/* 分控参数 */
};

typedef struct  {                                       //设备参数
    uint8_t Remote_lock;                                //遥控和按键在主页面才会有效果
    uint8_t login;                                      //主控是否登录
    uint8_t power;                                     //电源状态
    uint8_t Motor_state;                               //电机状态
    uint8_t Group;                                      // 第几组
    uint8_t all_num;                                  //总数量
    uint8_t page1_num;                                 //第一组数量
    uint8_t page2_num;                                //第二组数量
//    uint8_t choose_num;                                //选择数量
    struct  branch_dev branch_oneGroup[PAGE_Motor_max];     //分组1
    struct  branch_dev branch_twoGroup[PAGE_Motor_max];     //分组2
    uint8_t page1_id[PAGE_Motor_max];                  //第一组id
    uint8_t page2_id[PAGE_Motor_max];                  //第二组id
//    uint8_t page_id[ALL_Motor_max];                  //未分组id
    uint8_t all_id[ALL_Motor_max];                   //总id
} Motor_dev;


typedef struct {
    QString  MainID;        //主控ID
    QString  User;          //操作人员
    QString  Action;        //操作动作
    QString  Howto;         //操作方式

} User_data;


struct Parameter_SET{                                        /* 功能配置*/
    uint8_t led;											 /* led */
    uint8_t loose;											 /* 失载时间 */
    uint8_t checkcurrent;								     /* 是否检测电流 */
    uint8_t bolt;										    /*  多久时间判定为断线 */
    uint8_t startcurrent;                                   /*  分控开机后多久时间检测电流*/
    uint8_t runCurrent;                                     /*  运行时间电流检测*/
    uint8_t curentMAX;                                      /*  电流检测峰值*/
    uint8_t baud;                                           /*  波特率更改*/

} ;





//枚举
//总状态
enum {
    MASTER_SIGN_OFF  = 0,  //未登录
    MASTER_SIGN_ON   = 1,  //登录
    LOCK,                  //不在操控页面，遥控和按键锁定
    UNLOCK,                //在操控页面，解锁遥控和按键
    POWER_OFF,       //电源关
    POWER_ON,        //电源开

};

//是否被选中状态
enum {
    YES = true,      //选中
    NO = false,      //未选中
};
//标志位是否有错状态
enum {
    Right,           //标志位没错
    Wrong,           //标志报错

};

//分控返回状态
enum {
    Stop = 0x01,        //停机
    Risining = 2,     //上升
    Loose = 3  ,      //松钩
    Tight = 4 ,       //紧钩
    Falling=5 ,       //下降
    Scan =6,          //扫描
    Set = 7,          //设置参数
    Wait =8,           //等待切换响应
    Overload= 9 ,       //超载
    LOSS =10,           //失载
    CurrentOver =11 ,   //电流过大
    Parameter,          //参数功能配置
    Bolt ,              //断线
    Warning,            //预警状态


};





#endif // DATA_H
