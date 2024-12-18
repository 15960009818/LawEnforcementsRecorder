#include "videocapturewin.h"
#include "indexwin.h"
#include "videoplayer.h"
#include "../common/singleton.h"
#include "../controller/videocapturecontroller.h"
#include <QMessageBox>
#include <QListWidgetItem>
#include <QInputDialog>
#include <QMessageBox>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QHttpMultiPart>
#include <QFile>
#include <QFileInfo>
#include <QMimeDatabase>
#include <QProgressDialog>
#include <QTimer>
VideoCaptureWin::VideoCaptureWin(QWidget *parent)
{
    setUi();            // 设置UI界面
    connectSignals();   // 连接信号和槽

    selectedDate = QDate::currentDate();
    editdatetime->setDate(selectedDate);
    page = 1;
    emit Singleton<VideoCaptureController>::getInstance().dateSignal(selectedDate,page);
    qDebug() << "[INFO] VideoCaptureWin Initializing Success";
}

void VideoCaptureWin::setUi()
{
//    this->setMaximumSize(400, 600);
//    this->setMinimumSize(400, 600);
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

            // 创建视频项的 QWidget
            QWidget *itemWidget = new QWidget();
            QHBoxLayout *layout = new QHBoxLayout(itemWidget);
            layout->setContentsMargins(5, 5, 5, 5);
            layout->setSpacing(10);

            // 视频缩略图
            QLabel *iconLabel = new QLabel();
            QPixmap pixmap(video.getVideoPath());

            if (!pixmap.isNull()) {
                iconLabel->setPixmap(pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            } else {
                qDebug() << "Warning: Failed to load video icon for path:" << video.getVideoPath();
            }
            layout->addWidget(iconLabel, 1);

            // 视频名称
            QLabel *nameLabel = new QLabel(video.getVideoName());
            layout->addWidget(nameLabel, 3);

            // 上传按钮
            QPushButton *uploadButton = new QPushButton("上传");
            layout->addWidget(uploadButton, 1);
             uploadButton->setProperty("videoPath", video.getVideoPath());
             // 连接上传按钮信号到槽函数
             connect(uploadButton, &QPushButton::clicked, this, [this, uploadButton]() {
                     QString videoPath = uploadButton->property("videoPath").toString();
                     openUploadWindow(videoPath);
             });
             itemWidget->setLayout(layout);

            // 添加到 QListWidget
            QListWidgetItem *item = new QListWidgetItem();
            item->setSizeHint(itemWidget->sizeHint());
            videowins->addItem(item);
            videowins->setItemWidget(item, itemWidget);

            // 将视频路径存储在 QListWidgetItem 的数据中
            item->setData(Qt::UserRole, video.getVideoPath());
        }
        // 防抖逻辑：限制快速点击
                QTimer *debounceTimer = new QTimer(this);
                debounceTimer->setSingleShot(true);
                // 在信号连接前断开所有旧的连接
                disconnect(videowins, &QListWidget::itemClicked, nullptr, nullptr);
                // 连接信号以选择视频
                connect(videowins, &QListWidget::itemClicked, [this, debounceTimer](QListWidgetItem *item) {
                    if (debounceTimer->isActive()) {
                        qDebug() << "Click ignored due to debounce timer.";
                        return; // 忽略快速连续点击
                    }

                    debounceTimer->start(500); // 500ms 的防抖时间
                    QString videoPath = item->data(Qt::UserRole).toString();

                    try {
                        if (videoPath.isEmpty() || !QFile::exists(videoPath)) {
                            throw std::runtime_error("Video file not found or path invalid.");
                        }

                        videoPlayer = new VideoPlayer(videoPath);

                        //videoPlayer->playPause(); // 调用可能会抛出异常
                        videoPlayer->show();

                        qDebug() << "Selected video path:" << videoPath;
                    } catch (const std::exception &e) {
                        qDebug() << "Error occurred while playing video:" << e.what();

                        if (videoPlayer) {
                            delete videoPlayer; // 确保释放资源
                            videoPlayer = nullptr;
                        }

                        // 显示错误信息给用户
                        QMessageBox::critical(this, "Error", QString("Failed to play video: %1").arg(e.what()));
                    } catch (...) {
                        qDebug() << "Unknown error occurred while playing video.";

                        if (videoPlayer) {
                            delete videoPlayer;
                            videoPlayer = nullptr;
                        }

                        QMessageBox::critical(this, "Error", "An unknown error occurred.");
                    }
                });


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
        IndexWin* indexWindow = IndexWin::getInstance();
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

void VideoCaptureWin::openUploadWindow(const QString &filePath) {
    FileTransWin *uploadWindow = new FileTransWin(); // 打开上传窗口
    uploadWindow->setFilePath(filePath);                 // 设置文件路径
    uploadWindow->show();                                // 显示窗口
}
