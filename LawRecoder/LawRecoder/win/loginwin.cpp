#include "loginwin.h"

LoginWin::LoginWin(QWidget *parent)
{
    setUi();
}

void LoginWin::setUi(){
    this->setFixedSize(400, 600);
        this->setWindowTitle("登录界面");

        labTitle = new QLabel("登录界面", this);
        labDvid = new QLabel("设备编号", this);
        labUserId = new QLabel("警员编号", this);
        labPwd = new QLabel("密码", this);
        labVfcd = new QLabel("验证码", this);

        editUserId = new QLineEdit(this);
        editUserId->setPlaceholderText("请输入警员编号");

        editPwd = new QLineEdit(this);
        editPwd->setEchoMode(QLineEdit::Password);
        editPwd->setPlaceholderText("请输入密码");

        editVfcd = new QLineEdit(this);
        editVfcd->setPlaceholderText("请输入验证码");

        combDvid = new QComboBox(this);
        combDvid->setEditable(true);
        combDvid->addItems({"HY-2022-001", "HY-2022-002", "HY-2022-003"});

        labVerificationCode = new VerificationCodeLabel(this);

        btnOk = new QPushButton("确定", this);
        btnReset = new QPushButton("清空", this);
        btnCancel = new QPushButton("取消", this);
        btnShowPwd = new QPushButton(this);
        btnShowPwd->setFixedSize(22, 22);
        btnShowPwd->setToolTip("密码可见");
        btnShowPwd->setCursor(Qt::PointingHandCursor);
        btnShowPwd->setStyleSheet("QPushButton{border-image:url(:/image/Login/眼睛_显示_o.png);background:transparent;}");

        QMargins margins = editPwd->textMargins();
        editPwd->setTextMargins(margins.left(), margins.top(), btnShowPwd->width(), margins.bottom());

        gridLayout = new QGridLayout;
        gridLayout->addWidget(labTitle, 0, 1, 1, 1);
        gridLayout->addWidget(labDvid, 1, 0, 1, 1);
        gridLayout->addWidget(combDvid, 1, 1, 1, 2);
        gridLayout->addWidget(labUserId, 2, 0, 1, 1);
        gridLayout->addWidget(editUserId, 2, 1, 1, 2);
        gridLayout->addWidget(labPwd, 3, 0, 1, 1);
        gridLayout->addWidget(editPwd, 3, 1, 1, 2);
        gridLayout->addWidget(btnShowPwd, 3, 2, 1, 1);
        gridLayout->addWidget(labVfcd, 4, 0, 1, 1);
        gridLayout->addWidget(editVfcd, 4, 1, 1, 1);
        gridLayout->addWidget(labVerificationCode, 4, 2, 1, 1);

        QHBoxLayout *buttonLayout = new QHBoxLayout;
        buttonLayout->addWidget(btnOk);
        buttonLayout->addWidget(btnReset);
        buttonLayout->addWidget(btnCancel);

        mainLayout = new QVBoxLayout(this);
        mainLayout->addLayout(gridLayout);
        mainLayout->addLayout(buttonLayout);

        setLayout(mainLayout);
}
/**
 * @brief LoginWin::connectSignals 统一管理按钮信号
 */
void LoginWin::connectSignals()
{

    connect(btnOk, &QPushButton::clicked, this, &LoginWin::BtnClicked);
    connect(btnReset, &QPushButton::clicked, this, &LoginWin::BtnClicked);
    connect(btnCancel, &QPushButton::clicked, this, &LoginWin::BtnClicked);
}

/**
 * @brief LoginWin::BtnClicked 确认登录按钮功能
 */
void LoginWin::BtnClicked()
{
    QPushButton* clickedButton = qobject_cast<QPushButton*>(sender());

    if (clickedButton == btnOk) {
        QString userId = editUserId->text();
        QString password = editPwd->text();
        QString inputVerificationCode = editVfcd->text();
        QString generatedVerificationCode = labVerificationCode->getVerificationCode();  // 假设有此方法获取生成的验证码
    emit Singleton<LoginController>::getInstance().LoginCheckSignals(userId, password, inputVerificationCode, generatedVerificationCode);


    } else if (clickedButton == btnReset) {
        editUserId->clear();
        editPwd->clear();
        editVfcd->clear();

    } else if (clickedButton == btnCancel) {
        close();
    }
}
