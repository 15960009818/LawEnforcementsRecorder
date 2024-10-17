#include "settingwin.h"
#include "../controller/settingcontroller.h"
#include "../common/singleton.h"

SettingWin::SettingWin(QWidget *parent)
{
    setUi();
    connectSignals();
}

void SettingWin::setUi(){
    // 初始化界面
    this->setMaximumSize(400,600); // 固定尺寸
    this->setMinimumSize(400,600);
    this->setWindowTitle("系统设置");

    // 控件初始化
    lineEdit = new QLineEdit();
    lineEdit->setFocusPolicy(Qt::NoFocus); // 不可编辑

    glay = new QGridLayout(this);
    LabTitle = new QLabel("设置界面");
    LabDvid = new QLabel("设备编号");
    LabMemSpc = new QLabel("内存空间");
    LabMemSpc1 = new QLabel();
    LabMemRemain = new QLabel("内存剩余");
    LabVdPath = new QLabel("录像存储");
    LabPixPath = new QLabel("图片存储");
    LabMemRemain2 = new QLabel();

    // 初始化标签和编辑框
    editVdPath = new QLineEdit;
    editPixPath = new QLineEdit;
    editMemSpc = new QLineEdit;
    editMemRemain = new QLineEdit;

    BtnVdPath = new QPushButton("…");
    BtnPixPath = new QPushButton("…");
    BtnOk = new QPushButton("确定");
    BtnCancel = new QPushButton("取消");
    CombDvid = new QComboBox;
    CombDvid->setEditable(true);
    CombDvid->addItem("HY-2022-001");
    CombDvid->addItem("HY-2022-002");
    CombDvid->addItem("HY-2022-003");

    // 控件尺寸设置
    editMemSpc->setFixedWidth(180);
    editMemSpc->setFixedHeight(30);
    editMemRemain->setFixedWidth(180);
    editMemRemain->setFixedHeight(30);
    BtnOk->setFixedHeight(40);
    BtnCancel->setFixedHeight(40);
    CombDvid->setFixedWidth(180);
    CombDvid->setFixedHeight(30);
    editVdPath->setFixedWidth(120);
    editVdPath->setFixedHeight(40);
    editPixPath->setFixedWidth(120);
    editPixPath->setFixedHeight(40);
    BtnVdPath->setFixedWidth(40);
    BtnVdPath->setFixedHeight(40);
    BtnPixPath->setFixedWidth(40);
    BtnPixPath->setFixedHeight(40);

    // 布局设置
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

    // 初始化编辑框内容
    editMemSpc->setText("50 MB");
    editMemRemain->setText("0 MB");
}

void SettingWin::connectSignals()
{

    //设置视频路径按钮
    connect(BtnVdPath, &QPushButton::clicked, this, &SettingWin::VideoPathClickedSlots);
    //设置图片路径（包括截图和封面）按钮
    connect(BtnPixPath, &QPushButton::clicked, this, &SettingWin::PicturePathClickedSlots);
    //确定按钮
    connect(BtnOk, &QPushButton::clicked, this, &SettingWin::BtnOkSlot);
    //取消按钮
    connect(BtnCancel, &QPushButton::clicked, this, &SettingWin::BtnCancelSlot);

    // 连接剩余空间信号
       connect(&Singleton<SettingController>::getInstance(),SIGNAL(spaceSufficient(qint64 )),
               this,SLOT(handleSpaceSufficient(qint64)));

       // 连接空间不足信号
       connect(&Singleton<SettingController>::getInstance(), SIGNAL(spaceInsufficient(QString )),
               this,SLOT(handleSpaceInsufficient(QString)));
}

/**
 * @brief SettingWin::VideoPathClickedSlots 选择视频路径槽
 */
void SettingWin::VideoPathClickedSlots()
{
    // 用户选择路径
        PathName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                     "./",
                                                     QFileDialog::ShowDirsOnly
                                                     | QFileDialog::DontResolveSymlinks);

        this->editVdPath->setText(PathName); // 文件路径显示

        // 获取用户输入的期望空间（假设用户输入的是 MB）
        qint64 requiredSpaceMB = this->editMemSpc->text().toLongLong();

        // 发射路径选择信号
        emit Singleton<SettingController>::getInstance().pathSelected(PathName,requiredSpaceMB);

}

/**
 * @brief SettingWin::checkVideoPathSlots 判断文件夹是否满足可用空间
 */
void SettingWin::checkVideoPathSlots(){

}

/**
 * @brief SettingWin::PicturePathClickedSlots 选择图片路径槽
 */
void SettingWin::PicturePathClickedSlots()
{
    PathName = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                 "./",
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);
    this->editPixPath->setText(PathName); // 文件路径显示

}
void SettingWin::handleSpaceSufficient(qint64 availableSpaceMB)
{
    // 更新剩余空间显示
    editMemRemain->setText(QString::number(availableSpaceMB) + " MB");
}

void SettingWin::handleSpaceInsufficient(QString errorMsg)
{
    // 显示错误信息弹窗
    QMessageBox::warning(this, "Error", errorMsg);
}

/**
 * @brief SettingWin::BtnOkSlot 确认按钮槽
 */
void SettingWin::BtnOkSlot()
{

}

/**
 * @brief SettingWin::BtnCancelSlot 返回（退出）槽
 */
void SettingWin::BtnCancelSlot()
{

}
