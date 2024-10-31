#include "videocapturewin.h"
#include "indexwin.h"
#include "../common/singleton.h"
#include "../controller/videocapturecontroller.h"
#include <QMessageBox>

VideoCaptureWin::VideoCaptureWin(QWidget *parent)
{
    setUi();            // 设置UI界面
    connectSignals();   // 连接信号和槽
    qDebug()<<"[INFO] VideoCaptureWin Initializing Success";
}

void VideoCaptureWin::setUi()
{
    this->setMaximumSize(400, 600);
    this->setMinimumSize(400, 600);
    this->setWindowTitle("视频选择界面");

    glay = new QGridLayout(this);
    LabTitle = new QLabel("视频列表");
    LabTitle->setStyleSheet("QLabel{font:20px;color:99ff99;background-color:rgb(99,f9,f9);}");
    LabDate = new QLabel("请选择日期");

    // 视频显示控件
    LabShowBigVid = new QLabel(this);
    LabShowBigVid->setFixedSize(380, 280);
    LabShowBigVid->hide();

    LabVidName = new QLabel("视频名称：");
    LabVidTime = new QLabel("视频创建时间：");
    LabVidPath = new QLabel("视频路径");
    LabVidNameS = new QLabel(this);
    LabVidTimeS = new QLabel(this);
    LabVidPathS = new QLabel(this);

    LabVidName->hide();
    LabVidTime->hide();
    LabVidPath->hide();
    LabVidNameS->hide();
    LabVidTimeS->hide();
    LabVidPathS->hide();

    BtnReturnList = new QPushButton("返回视频列表");
    BtnReturnList->hide();
    TlBtnReturn = new QToolButton(this);
    ActReturn = new QAction(this);
    ActReturn->setIcon(QIcon(":/image/image/player/return.png"));
    TlBtnReturn->setIconSize(QSize(48, 48));
    TlBtnReturn->setDefaultAction(ActReturn);

    // 日期选择器
    editdatetime = new QDateEdit(QDate::currentDate(), this);
    editdatetime->setCalendarPopup(true);
    editdatetime->setDisplayFormat("yyyy/MM/dd");
    editdatetime->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    // 视频列表控件
    videowins = new QListWidget();
    videowins->setViewMode(QListView::IconMode);
    videowins->setMovement(QListView::Static);
    videowins->setResizeMode(QListWidget::Adjust);
    videowins->setFlow(QListWidget::LeftToRight);
    videowins->setLayoutMode(QListWidget::Batched);
    videowins->setWrapping(true);
    videowins->setIconSize(QSize(140, 100));

    BtnMore = new QPushButton("查看更多");

    // 布局
    glay->addWidget(LabTitle, 0, 1, 1, 1);
    glay->addWidget(TlBtnReturn, 1, 0, 1, 1);
    glay->addWidget(LabDate, 2, 0, 1, 1);
    glay->addWidget(editdatetime, 2, 1, 1, 3);
    glay->addWidget(videowins, 4, 0, 4, 5);
    glay->addWidget(BtnMore, 10, 1, 1, 2);
    glay->addWidget(BtnReturnList, 10, 1, 1, 2);
}

void VideoCaptureWin::connectSignals()
{
    connect(&Singleton<VideoCaptureController>::getInstance(), &VideoCaptureController::finishVideoSignal,
            this, &VideoCaptureWin::onFinishedVideoQuery);

    connect(TlBtnReturn, &QToolButton::clicked, this, &VideoCaptureWin::BtnClicked);
    connect(editdatetime, &QDateEdit::dateChanged, this, &VideoCaptureWin::BtnClicked);
    connect(BtnReturnList, &QPushButton::clicked, this, &VideoCaptureWin::BtnClicked);
    connect(BtnMore, &QPushButton::clicked, this, &VideoCaptureWin::BtnClicked);
}

void VideoCaptureWin::onFinishedVideoQuery(const QString &message, const QList<VideoDao> &videoList) {
    videowins->clear();

    if (message.contains("数据库错误")) {
        QMessageBox::critical(this, "Database Error", message);
        qDebug() << "Database error occurred:" << message;
    } else if (message.contains("没有找到任何视频记录")) {
        QMessageBox::information(this, "No Records Found", message);
        qDebug() << "No video records found.";
    } else if (message.contains("视频信息获取成功")) {
        qDebug() << "Successfully queried videos, found" << videoList.size() << "videos.";

        for (const VideoDao &video : videoList) {
            qDebug() << "Video ID:" << video.getVideoId()
                     << ", Name:" << video.getVideoName()
                     << ", Date:" << video.getVideoDate()
                     << ", Path:" << video.getVideoPath();

            QWidget *itemWidget = new QWidget();
            QHBoxLayout *layout = new QHBoxLayout(itemWidget);
            layout->setContentsMargins(5, 5, 5, 5);
            layout->setSpacing(10);

            QLabel *iconLabel = new QLabel();
            QPixmap pixmap(video.getVideoPath());

            if (!pixmap.isNull()) {
                iconLabel->setPixmap(pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            } else {
                qDebug() << "Warning: Failed to load video icon for path:" << video.getVideoPath();
            }
            layout->addWidget(iconLabel, 1);

            QLabel *nameLabel = new QLabel(video.getVideoName());
            layout->addWidget(nameLabel, 3);

            itemWidget->setLayout(layout);

            QListWidgetItem *item = new QListWidgetItem();
            item->setSizeHint(itemWidget->sizeHint());
            videowins->addItem(item);
            videowins->setItemWidget(item, itemWidget);
        }
    } else if (message.contains("查询失败")) {
        QMessageBox::warning(this, "Query Error", message);
        qDebug() << "Query failed with message:" << message;
    } else {
        QMessageBox::warning(this, "Unknown Error", "发生未知错误。");
        qDebug() << "An unknown error occurred.";
    }
}

void VideoCaptureWin::BtnClicked()
{
    QObject* obj = sender();

    if (obj == TlBtnReturn) {
        qDebug() << "返回按钮点击";
        IndexWin* indexWindow = new IndexWin();
        indexWindow->show();
        this->hide();
    } else if (obj == BtnReturnList) {
        qDebug() << "返回视频列表按钮点击";
    } else if (obj == BtnMore) {
        page++;
        emit Singleton<VideoCaptureController>::getInstance().dateSignal(selectedDate, page);
        qDebug() << "查看更多按钮点击";
    } else if (obj == editdatetime) {
        page = 1;
        selectedDate = editdatetime->date();
        QString formattedDate = selectedDate.toString("yyyy-MM-dd");
        qDebug() << "日期更改：" << formattedDate;
        emit Singleton<VideoCaptureController>::getInstance().dateSignal(selectedDate, page);
    }
}
