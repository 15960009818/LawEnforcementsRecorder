#include "imagecapturewin.h"


ImageCaptureWin::ImageCaptureWin(QWidget *parent)
{
    setUi();            // 设置UI界面
    connectSignals();    // 连接信号和槽
}

void ImageCaptureWin::setUi()
{
    this->setMaximumSize(400, 600);
        this->setMinimumSize(400, 600);
        this->setWindowTitle("照片查看界面");

        glay = new QGridLayout(this);
        LabTitle = new QLabel("照片列表");
        LabTitle->setStyleSheet("QLabel{font:20px;color:99ff99;background-color:rgb(99,f9,f9);}");
        LabDate = new QLabel("请选择日期");
        LabShowBigPic = new QLabel(this);
        LabShowBigPic->setFixedSize(380, 280);
        LabShowBigPic->hide();

        LabShowDetlPic = new QLabel(this);
        LabShowDetlPic->setFixedSize(300, 200);
        LabShowDetlPic->hide();

        LabptName = new QLabel("图片名称：");
        LabptTime = new QLabel("图片创建时间：");
        LabptPath = new QLabel("图片路径");
        LabptNameS = new QLabel(this);
        LabptTimeS = new QLabel(this);
        LabptPathS = new QLabel(this);

        LabptName->hide();
        LabptTime->hide();
        LabptPath->hide();
        LabptNameS->hide();
        LabptTimeS->hide();
        LabptPathS->hide();

        BtnReturnList = new QPushButton("返回图片列表");
        BtnReturnList->hide();
        TlBtnReturn = new QToolButton(this);
        ActReturn = new QAction(this);
        ActReturn->setIcon(QIcon(":/image/image/player/return.png"));
        TlBtnReturn->setIconSize(QSize(48, 48));
        TlBtnReturn->setDefaultAction(ActReturn);

        TlBtnCalendar = new QToolButton(this);

        // 在构造函数中
        editdatetime = new QDateEdit(QDate::currentDate(), this);
        editdatetime->setCalendarPopup(true);
        editdatetime->setDisplayFormat("yyyy/MM/dd");
        editdatetime->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);




        videowins = new QListWidget();
        videowins->setViewMode(QListView::IconMode);
        videowins->setMovement(QListView::Static);
        videowins->setResizeMode(QListWidget::Adjust);
        videowins->setFlow(QListWidget::LeftToRight);
        videowins->setLayoutMode(QListWidget::Batched);
        videowins->setWrapping(true);
        videowins->setIconSize(QSize(140, 100));

        BtnMore = new QPushButton("查看更多"); // 新增的“查看更多”按钮


        glay->addWidget(LabTitle, 0, 1, 1, 1);
        glay->addWidget(TlBtnReturn, 1, 0, 1, 1);
        glay->addWidget(LabDate, 2, 0, 1, 1);
        //glay->addWidget(calenda, 3, 1, 1, 2);
        glay->addWidget(TlBtnCalendar, 2, 4, 1, 1, Qt::AlignLeft | Qt::AlignVCenter);
        glay->addWidget(videowins, 4, 0, 4, 5);
        glay->addWidget(editdatetime, 2, 1, 1, 3);

        glay->addWidget(LabShowBigPic, 4, 0, 2, 5);
        glay->addWidget(LabShowDetlPic, 4, 0, 3, 4);
        glay->addWidget(LabptName, 7, 0, 1, 1);
        glay->addWidget(LabptTime, 8, 0, 1, 1);
        glay->addWidget(LabptPath, 9, 0, 1, 1);
        glay->addWidget(LabptNameS, 7, 1, 1, 1);
        glay->addWidget(LabptTimeS, 8, 1, 1, 1);
        glay->addWidget(LabptPathS, 9, 1, 1, 1);
        glay->addWidget(BtnMore, 10, 1, 1, 2); // 添加查看更多按钮
         glay->addWidget(BtnReturnList, 10, 1, 1, 2);
}

void ImageCaptureWin::connectSignals()
{

}

void ImageCaptureWin::paintEvent(QPaintEvent *event)
{

}

void ImageCaptureWin::BtnClicked()
{

}
