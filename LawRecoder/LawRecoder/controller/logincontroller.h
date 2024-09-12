#ifndef LOGINCONTROLLER_H
#define LOGINCONTROLLER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include "../common/singleton.h"
#include "../service/loginservice.h"
class LoginController:public QObject
{
    SINGLETON(LoginController)
    Q_OBJECT
signals:
        void LoginCheckSignals(const QString& userAccount, const QString& userPassword, const QString& inputVerificationCode, const QString& generatedVerificationCode);
        void startedLoginSignal();
        void finishedLoginServiceSignal(int result);
public:
    void initController();

    int getLoginFlag() const;
    void setLoginFlag(int flag);

public slots:
        void LoginCheckSlots(const QString& userAccount, const QString& userPassword, const QString& inputVerificationCode, const QString& generatedVerificationCode);
private:
        int loginflag;
        QThread *threadLoginCheck;
        LoginService *loginService;
private slots:
        void startLoginSlot();
        void quitLoginServiceThreadSlot();
        void finishedLoginServiceThreadSlot();
};

#endif // LOGINCONTROLLER_H
