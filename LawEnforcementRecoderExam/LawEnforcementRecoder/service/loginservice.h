#ifndef LOGINSERVICE_H
#define LOGINSERVICE_H

#include <QObject>
#include <QString>
class LoginService
{
public:
    LoginService();
public slots:
    //登录验证
    bool LoginCheck(QString userAccount,QString userPassword);
    //登录时间记录
    bool LoginTime(QString userAccount);
};

#endif // LOGINSERVICE_H
