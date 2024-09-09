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
        void LoginCheckSignals(const QString& userId, const QString& password, const QString& inputVerificationCode, const QString& generatedVerificationCode);
public:

public slots:
        void LoginCheckSlots(const QString& userId, const QString& password, const QString& inputVerificationCode, const QString& generatedVerificationCode);
};

#endif // LOGINCONTROLLER_H
