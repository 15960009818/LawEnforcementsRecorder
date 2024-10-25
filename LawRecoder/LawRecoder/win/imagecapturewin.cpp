#include "imagecapturewin.h"
#include "indexwin.h"
#include "../common/singleton.h"
#include "../controller/imagecapturecontroller.h"
#include <QMessageBox>
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

            // 设置布局间距和边距
            layout->setContentsMargins(5, 5, 5, 5);
            layout->setSpacing(10);

            // 图片标签，占左侧 1/4
            QLabel *iconLabel = new QLabel();
            QPixmap pixmap(picture.getPicturePath());

            // 检查图标是否加载成功
            if (!pixmap.isNull()) {
                iconLabel->setPixmap(pixmap.scaled(50, 50, Qt::KeepAspectRatio, Qt::SmoothTransformation));
            } else {
                qDebug() << "Warning: Failed to load image icon for path:" << picture.getPicturePath();
            }

            layout->addWidget(iconLabel, 1); // 占1份宽度

//            // 名称标签，占右侧 1/4
//            QLabel *nameLabel = new QLabel(picture.getPictureName());
//            layout->addWidget(nameLabel, 1);
            // 日期标签，占2/4
            QLabel *dateLabel = new QLabel(picture.getPictureDate());
            layout->addWidget(dateLabel,2);

            itemWidget->setLayout(layout);

            // 创建 QListWidgetItem 并将自定义 QWidget 设置为其内容
            QListWidgetItem *item = new QListWidgetItem();
            item->setSizeHint(itemWidget->sizeHint()); // 设置项目大小
            videowins->addItem(item);
            videowins->setItemWidget(item, itemWidget); // 将 QWidget 作为内容设置
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
        IndexWin* indexWindow = new IndexWin();  // 创建主界面窗口
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
