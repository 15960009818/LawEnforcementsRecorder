#ifndef LOGINWIN_H
#define LOGINWIN_H

#include <QWidget>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QGridLayout>
#include <QVBoxLayout>
#include "../common/verificationcodelabel.h"  // 假设这个类在项目中定义
#include "../controller/logincontroller.h"
class LoginWin : public QWidget
{
    Q_OBJECT
public:
    explicit LoginWin(QWidget *parent = nullptr);

    // 设置UI界面
    void setUi();



    void connectSignals();
private slots:
    void BtnClicked();
private:
    QLabel *labTitle;
    QLabel *labDvid;
    QLabel *labUserId;
    QLabel *labPwd;
    QLabel *labVfcd;
    QLineEdit *editUserId;
    QLineEdit *editPwd;
    QLineEdit *editVfcd;
    QComboBox *combDvid;
    VerificationCodeLabel *labVerificationCode;  // 自定义验证码控件
    QPushButton *btnOk;
    QPushButton *btnReset;
    QPushButton *btnCancel;
    QPushButton *btnShowPwd;

    QGridLayout *gridLayout;
    QVBoxLayout *mainLayout;
};

#endif // LOGINWIN_H
