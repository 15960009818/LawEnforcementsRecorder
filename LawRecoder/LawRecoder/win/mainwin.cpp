#include "mainwin.h"

MainWin::MainWin(QWidget *parent)
{

}

void MainWin::setUi()
{
    this->setMaximumSize(400,600);//固定尺寸
    this->setMinimumSize(400,600);
    this->setWindowTitle("执法记录仪主界面");

//网格布局
    BtnVideo=new QPushButton;
    BtnVideo->setIconSize(QSize(380,400));
    BtnVideo->setFixedHeight(380);
    BtnVideo->setFixedWidth(400);

    glay=new QGridLayout(this);
    LabTitle=new QLabel("执法记录仪");
    glay->addWidget(LabTitle,0,1,1,1);
    LabTitle->setStyleSheet("QLabel{font:20px;color:99ff99;background-color:rgb(99,f9,f9);}");
    glay->addWidget(BtnVideo,1,0,4,3);
    BtnTakPic=new QPushButton("现场拍照");
    glay->addWidget(BtnTakPic,5,0);
    BtnBakVid=new QPushButton("录像回放");
    glay->addWidget(BtnBakVid,5,1);
    BtnSeePic=new QPushButton("现场图片");
    glay->addWidget(BtnSeePic,5,2);
    BtnSeeLog=new QPushButton("查看日志");
    glay->addWidget(BtnSeeLog,6,0);
    BtnTrsVid=new QPushButton("录像转码");
    glay->addWidget(BtnTrsVid,6,1);
    BtnSettings=new QPushButton("系统设置");
    glay->addWidget(BtnSettings,6,2);
    BtnLogin=new QPushButton("登录");
    BtnReturn=new QPushButton("退出");
    glay->addWidget(BtnLogin,7,0);
    glay->addWidget(BtnReturn,7,0);
    BtnReturn->hide();
    BtnRgs=new QPushButton("注册");
    glay->addWidget(BtnRgs,7,2);

//    connect(BtnTakPic,SIGNAL(clicked(bool)),this,SLOT(BtnSlot()));
//    connect(BtnSeePic,SIGNAL(clicked(bool)),this,SLOT(BtnSlot()));
//    connect(BtnBakVid,SIGNAL(clicked(bool)),this,SLOT(BtnSlot()));
//    connect(BtnSeeLog,SIGNAL(clicked(bool)),this,SLOT(BtnSlot()));
//    connect(BtnTrsVid,SIGNAL(clicked(bool)),this,SLOT(BtnSlot()));
//    connect(BtnLogin,SIGNAL(clicked(bool)),this,SLOT(BtnSlot()));
//    connect(BtnRgs,SIGNAL(clicked(bool)),this,SLOT(BtnSlot()));
//    connect(BtnSettings,SIGNAL(clicked(bool)),this,SLOT(BtnSlot()));
//    connect(BtnReturn,SIGNAL(clicked(bool)),this,SLOT(BtnSlot()));

}

