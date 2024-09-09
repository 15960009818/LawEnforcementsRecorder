#ifndef LOGINCONTROLLER_H
#define LOGINCONTROLLER_H

#include <QObject>
#include <QThread>
#include "../common/singleton.h"
class LoginController:public QObject
{
    SINGLETON(LoginController)
    Q_OBJECT
signals:
        void LoginCheckSignals(const QString& userAccount, const QString& userPassword, const QString& inputVerificationCode, const QString& generatedVerificationCode);
        void startedLoginSignal();

public:
    void initController();

    int getLoginFlag() const;
    void setLoginFlag(int flag);

public slots:
        void LoginCheckSlots(const QString& userAccount, const QString& userPassword, const QString& inputVerificationCode, const QString& generatedVerificationCode);
private:
        int loginflag;
private slots:
        void startLoginSlot();
};

#endif // LOGINCONTROLLER_H
