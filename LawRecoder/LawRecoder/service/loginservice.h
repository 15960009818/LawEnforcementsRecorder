#ifndef LOGINSERVICE_H
#define LOGINSERVICE_H

#include <QObject>
#include "../common/singleton.h"
class LoginService :public QObject
{
    Q_OBJECT
public:
    explicit LoginService(QObject *parent = nullptr);


signals:

public slots:

    void GetLoginCheckSlots(const QString &userAccount, const QString &userPassword);
};

#endif // LOGINSERVICE_H
