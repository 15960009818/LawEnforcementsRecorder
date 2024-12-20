#ifndef LOGINCONTROLLER_H
#define LOGINCONTROLLER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QString>
#include "../common/singleton.h"
#include "../service/loginservice.h"
class LoginController:public QObject
{
    SINGLETON(LoginController)
    Q_OBJECT
signals:
        void LoginCheckSignals(const QString& userAccount, const QString& userPassword, const QString& inputVerificationCode, const QString& generatedVerificationCode);
        void startedLoginSignal();
        void finishedLoginUISignal(const QString &uiMessage);
        void finishedLoginControllerSignal(const int messageflag);
public:
    void initController();

    static int getLoginFlag() ;
    static void setLoginFlag(int flag);

public slots:
        void LoginCheckSlots(const QString& userAccount, const QString& userPassword, const QString& inputVerificationCode, const QString& generatedVerificationCode);

private:
        static int loginflag;
        QThread *threadLoginCheck;
        LoginService *loginService;
private slots:
        void startLoginSlot();
        void quitLoginServiceThreadSlot();
        void finishedLoginControllerThreadSlot(const int messageflag);

};

#endif // LOGINCONTROLLER_H
