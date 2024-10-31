#include "indexwin.h"
#include "settingwin.h"
#include "../controller/settingcontroller.h"
#include "../common/singleton.h"
#include "../service/deviceservice.h"
SettingWin::SettingWin(QWidget *parent) : QWidget(parent)
{
    // 初始化界面和信号连接
    setUi();
    connectSignals();
}

/**
 * @brief 设置界面布局及控件
 */
void SettingWin::setUi()
{
    // 固定窗口大小，设置窗口标题
    this->setMaximumSize(400, 600);
    this->setMinimumSize(400, 600);
    this->setWindowTitle("系统设置");

    // 初始化控件
    lineEdit = new QLineEdit();
    lineEdit->setFocusPolicy(Qt::NoFocus);  // 设置不可编辑

    // 布局管理器
    glay = new QGridLayout(this);

    // 标签控件
    LabTitle = new QLabel("设置界面");
    LabDvid = new QLabel("设备编号");
    LabMemSpc = new QLabel("内存空间");

    LabMemRemain = new QLabel("内存剩余");
    LabVdPath = new QLabel("录像存储");
    LabPixPath = new QLabel("图片存储");


    // 编辑框控件
    editVdPath = new QLineEdit;
    editPixPath = new QLineEdit;
    editMemSpc = new QLineEdit;
    editMemRemain = new QLineEdit;

    // 按钮控件
    BtnVdPath = new QPushButton("…");
    BtnPixPath = new QPushButton("…");
    BtnOk = new QPushButton("确定");
    BtnCancel = new QPushButton("取消");

    // 设备编号下拉框
    CombDvid = new QComboBox;
    CombDvid->setEditable(true);
    CombDvid->addItems({"HY-2022-001", "HY-2022-002", "HY-2022-003"});

    // 控件尺寸设置
    setWidgetSizes();

    // 布局添加控件
    addWidgetsToLayout();

    // 初始化内存空间和内存剩余
    editMemSpc->setText("50 MB");
    editMemRemain->setText("0 MB");
}

/**
 * @brief 设置控件的尺寸
 */
void SettingWin::setWidgetSizes()
{
    editMemSpc->setFixedSize(180, 30);
    editMemRemain->setFixedSize(180, 30);
    BtnOk->setFixedHeight(40);
    BtnCancel->setFixedHeight(40);
    CombDvid->setFixedSize(180, 30);
    editVdPath->setFixedSize(120, 40);
    editPixPath->setFixedSize(120, 40);
    BtnVdPath->setFixedSize(40, 40);
    BtnPixPath->setFixedSize(40, 40);
}

/**
 * @brief 将控件添加到布局中
 */
void SettingWin::addWidgetsToLayout()
{
    glay->addWidget(LabTitle, 0, 1, 1, 1, Qt::AlignCenter);
    glay->addWidget(LabDvid, 1, 0, 1, 1, Qt::AlignRight);
    glay->addWidget(CombDvid, 1, 1, 1, 2, Qt::AlignLeft);
    glay->addWidget(LabMemSpc, 2, 0, 1, 1, Qt::AlignRight);
    glay->addWidget(editMemSpc, 2, 1, 1, 1);
    glay->addWidget(LabMemRemain, 3, 0, 1, 1, Qt::AlignRight);
    glay->addWidget(editMemRemain, 3, 1, 1, 1, Qt::AlignLeft);
    glay->addWidget(LabVdPath, 4, 0, 1, 1, Qt::AlignRight);
    glay->addWidget(editVdPath, 4, 1, 1, 2, Qt::AlignLeft);
    glay->addWidget(BtnVdPath, 4, 2, 1, 1, Qt::AlignLeft);
    glay->addWidget(LabPixPath, 5, 0, 1, 1, Qt::AlignRight);
    glay->addWidget(editPixPath, 5, 1, 1, 2, Qt::AlignLeft);
    glay->addWidget(BtnPixPath, 5, 2, 1, 1, Qt::AlignLeft);
    glay->addWidget(BtnOk, 7, 0, 1, 1, Qt::AlignRight);
    glay->addWidget(BtnCancel, 7, 2, 1, 1, Qt::AlignLeft);
}

/**
 * @brief 连接信号和槽
 */
void SettingWin::connectSignals()
{
    // 路径按钮信号连接
    connect(BtnVdPath, &QPushButton::clicked, this, &SettingWin::VideoPathClickedSlots);
    connect(BtnPixPath, &QPushButton::clicked, this, &SettingWin::PicturePathClickedSlots);

    // 确认、取消按钮信号连接
    connect(BtnOk, &QPushButton::clicked, this, &SettingWin::BtnOkSlot);
    connect(BtnCancel, &QPushButton::clicked, this, &SettingWin::BtnCancelSlot);

    // 处理存储空间信号
    connect(&Singleton<SettingController>::getInstance(), SIGNAL(spaceSufficient(qint64)),
            this, SLOT(handleSpaceSufficient(qint64)));
    connect(&Singleton<SettingController>::getInstance(), SIGNAL(spaceInsufficient(QString)),
            this, SLOT(handleSpaceInsufficient(QString)));
}

/**
 * @brief 选择视频存储路径槽函数
 */
void SettingWin::VideoPathClickedSlots()
{
    PathName = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "./",
                                                 QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    editVdPath->setText(PathName);  // 显示选中的路径

    // 获取用户输入的期望空间（MB）
    qint64 requiredSpaceMB = editMemSpc->text().toLongLong();

    // 发射路径选择信号
    emit Singleton<SettingController>::getInstance().pathSelected(PathName, requiredSpaceMB);
}

/**
 * @brief 选择图片存储路径槽函数
 */
void SettingWin::PicturePathClickedSlots()
{
    PathName = QFileDialog::getExistingDirectory(this, tr("Open Directory"), "./",
                                                 QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);


    Singleton<DeviceService>::getInstance().setPicturePath(&PathName);
    editPixPath->setText(PathName);  // 显示选中的路径
}

/**
 * @brief 处理可用空间的信号
 */
void SettingWin::handleSpaceSufficient(qint64 availableSpaceMB)
{
    editMemRemain->setText(QString::number(availableSpaceMB) + " MB");  // 更新剩余空间
}

/**
 * @brief 处理空间不足的信号
 */
void SettingWin::handleSpaceInsufficient(QString errorMsg)
{
    QMessageBox::warning(this, "Error", errorMsg);  // 显示警告信息
}

/**
 * @brief 确认按钮槽函数，进入主界面
 */
void SettingWin::BtnOkSlot()
{
    IndexWin* indexWindow = new IndexWin();  // 创建主界面窗口
    indexWindow->show();  // 显示主界面
    this->hide();  // 隐藏设置窗口
    resettingFlag = 1;  // 标记已经进行设置
}

/**
 * @brief 取消按钮槽函数，根据状态退出或返回主界面
 */
void SettingWin::BtnCancelSlot()
{
    if (resettingFlag == 1) {
        IndexWin* indexWindow = new IndexWin();
        indexWindow->show();  // 显示主界面
        this->close();  // 关闭设置窗口
    } else {
        close();  // 直接退出应用程序
    }
}
