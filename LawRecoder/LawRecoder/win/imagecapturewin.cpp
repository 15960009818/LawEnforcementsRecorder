#include "imagecapturewin.h"
#include "imagedetialwin.h"
#include "indexwin.h"
#include "../common/singleton.h"
#include "../controller/imagecapturecontroller.h"
#include <QMessageBox>
ImageCaptureWin::ImageCaptureWin(QWidget *parent)
    : QWidget(parent) // 确保调用父类构造函数
{
    setUi();            // 设置UI界面
    connectSignals();    // 连接信号和槽

    // 设置为当前日期
    selectedDate = QDate::currentDate();
    editdatetime->setDate(selectedDate); // 将日期编辑器设置为当前日期
    page = 1; // 重置页数
    emit Singleton<ImageCaptureController>::getInstance().dateSignal(selectedDate, page); // 发出信号查询当天的图片记录
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
    connect(&Singleton<ImageCaptureController>::getInstance(), &ImageCaptureController::finishPictureSignal,
                this, &ImageCaptureWin::onFinishedPictureQuery);
    // 将返回按钮 TlBtnReturn 连接到 BtnClicked 槽函数
    connect(TlBtnReturn, &QToolButton::clicked, this, &ImageCaptureWin::BtnClicked);

    // 将编辑日期选择器 editdatetime 连接到 BtnClicked 槽函数
    connect(editdatetime, &QDateEdit::dateChanged, this, &ImageCaptureWin::BtnClicked);

    // 将返回图片列表按钮 BtnReturnList 连接到 BtnClicked 槽函数
    connect(BtnReturnList, &QPushButton::clicked, this, &ImageCaptureWin::BtnClicked);

    // 将查看更多按钮 BtnMore 连接到 BtnClicked 槽函数
    connect(BtnMore, &QPushButton::clicked, this, &ImageCaptureWin::BtnClicked);


}

void ImageCaptureWin::onFinishedPictureQuery(const QString &message, const QList<PictureDao> &pictureList) {
    // 清空之前的列表内容
    videowins->clear();

    // 根据返回的状态消息判断查询结果
    if (message.contains("数据库错误")) {
        QMessageBox::critical(this, "Database Error", message);
        qDebug() << "Database error occurred:" << message;
    } else if (message.contains("没有找到任何图片记录")) {
        QMessageBox::information(this, "No Records Found", message);
        qDebug() << "No picture records found.";
    } else if (message.contains("图片信息获取成功")) {
        qDebug() << "Successfully queried pictures, found" << pictureList.size() << "pictures.";

        for (const PictureDao &picture : pictureList) {
            // 打印每个图片的详细信息
            qDebug() << "Picture ID:" << picture.getPictureId()
                     << ", Name:" << picture.getPictureName()
                     << ", Date:" << picture.getPictureDate()
                     << ", Path:" << picture.getPicturePath();

            // 创建一个 QWidget 作为 QListWidgetItem 的内容
            QWidget *itemWidget = new QWidget();
            QHBoxLayout *layout = new QHBoxLayout(itemWidget);
            layout->setContentsMargins(5, 5, 5, 5);
            layout->setSpacing(10);

            // 图片按钮，占左侧 1/4
            QPushButton *pictureButton = new QPushButton();
            QPixmap pixmap(picture.getPicturePath());
            if (!pixmap.isNull()) {
                pictureButton->setIcon(QIcon(pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation)));
            } else {
                qDebug() << "Warning: Failed to load image icon for path:" << picture.getPicturePath();
            }
            pictureButton->setIconSize(QSize(50, 50)); // 设置按钮的图标大小
            pictureButton->setText(picture.getPictureName()); // 可选：显示图片名称
            layout->addWidget(pictureButton, 1);

            // 图片日期，占2/4
            QLabel *dateLabel = new QLabel(picture.getPictureDate());
            layout->addWidget(dateLabel, 2);

            // 上传按钮
            QPushButton *uploadButton = new QPushButton("上传");
            layout->addWidget(uploadButton, 1);
             uploadButton->setProperty("picturePath", picture.getPicturePath());
             // 连接上传按钮信号到槽函数
             connect(uploadButton, &QPushButton::clicked, this, [this, uploadButton]() {
                     QString picturePath = uploadButton->property("picturePath").toString();
                     openUploadWindow(picturePath);
             });
            itemWidget->setLayout(layout);

            // 创建 QListWidgetItem 并将自定义 QWidget 设置为其内容
            QListWidgetItem *item = new QListWidgetItem();
            item->setSizeHint(itemWidget->sizeHint());
            videowins->addItem(item);
            videowins->setItemWidget(item, itemWidget);

            // 连接按钮点击信号到槽函数
            connect(pictureButton, &QPushButton::clicked, [=]() {
                // 发出信号的同时，直接在按钮点击时显示图片详情页面
                emit pictureClicked(picture); // 发出信号，传递图片数据

                qDebug() << "图片按钮被点击，路径：" << picture.getPicturePath()
                         << "名称：" << picture.getPictureName();

                // 创建并显示图片详情页面
                ImageDetailWin *detailWindow = new ImageDetailWin(picture);  // 传递图片对象
                detailWindow->show();  // 显示详情窗口

                // 可选: 关闭当前窗口或其他处理
                this->close(); // 如果点击图片按钮时想要关闭当前页面，取消这一行如果不需要
            });

        }
    } else if (message.contains("查询失败")) {
        QMessageBox::warning(this, "Query Error", message);
        qDebug() << "Query failed with message:" << message;
    } else {
        QMessageBox::warning(this, "Unknown Error", "发生未知错误。");
        qDebug() << "An unknown error occurred.";
    }
}

void ImageCaptureWin::BtnClicked()
{
    QObject* obj = sender();  // 获取发出信号的对象

    if (obj == TlBtnReturn) {
        qDebug() << "返回按钮点击";
        IndexWin* indexWindow =IndexWin::getInstance();  // 创建主界面窗口
        indexWindow->show();  // 显示主界面
        this->hide();  // 隐藏设置窗口
    } else if (obj == BtnReturnList) {
        // 返回图片列表按钮点击的处理逻辑
        qDebug() << "返回图片列表按钮点击";
    } else if (obj == BtnMore) {
        page++;
        emit Singleton<ImageCaptureController>::getInstance().dateSignal(selectedDate,page);
        qDebug()<<"查看更多按钮点击";

    } else if (obj == editdatetime) {
        page = 1;
        // 日期选择器更改的处理逻辑
        selectedDate = editdatetime->date();

        // 将日期格式化为(yyyy-MM-dd)
        QString formattedDate = selectedDate.toString("yyyy-MM-dd");
        qDebug() << "日期更改：" << formattedDate;

        // 通过信号发送 QDate 对象,第一次
        emit Singleton<ImageCaptureController>::getInstance().dateSignal(selectedDate,page);

    }
}
void ImageCaptureWin::openUploadWindow(const QString &filePath) {
    FileTransWin *uploadWindow = new FileTransWin(); // 打开上传窗口
    uploadWindow->setFilePath(filePath);                 // 设置文件路径
    uploadWindow->show();                                // 显示窗口
}
