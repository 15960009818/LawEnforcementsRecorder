#include "loginservice.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDateTime>
#include <QDebug>
#include "../controller/logincontroller.h"
#include "../common/singleton.h"
#include "../common/commondefs.h"

LoginService::LoginService(QObject *parent) : QObject(parent)
{
}

void LoginService::GetLoginCheckSlots(const QString &userAccount, const QString &userPassword)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data/video.db");

    // 打开数据库
    if (!db.open()) {
        qDebug() << "Database open error:" << db.lastError().text();
        emit Singleton<LoginController>::getInstance().finishedLoginControllerSignal(SQLERROR); // 发送数据库打开错误信号
        return;
    }

    // 查询用户
    QSqlQuery query(db);
    query.prepare("SELECT user_id FROM user WHERE user_account = :account AND user_password = :password");
    query.bindValue(":account", userAccount);
    query.bindValue(":password", userPassword);

    if (query.exec() && query.next()) {
        int userId = query.value(0).toInt();
        qDebug() << "[DEBUG] Login successful, user ID:" << userId;

        // 更新用户登录时间
        QSqlQuery updateQuery(db);
        updateQuery.prepare("UPDATE user SET user_newlogintime = :loginTime WHERE user_id = :userId");
        updateQuery.bindValue(":loginTime", QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss"));
        updateQuery.bindValue(":userId", userId);

        if (updateQuery.exec()) {
            qDebug() << "[DEBUG] Updated user_newlogintime successfully.";
            emit Singleton<LoginController>::getInstance().finishedLoginControllerSignal(SQLSUCCESS);  // 发送登录成功信号
        } else {
            qDebug() << "[DEBUG] Failed to update user_newlogintime:" << updateQuery.lastError().text();
            emit Singleton<LoginController>::getInstance().finishedLoginControllerSignal(SQLERROR);  // 发送更新失败信号
        }
    } else {
        qDebug() << "[DEBUG] Login failed: incorrect account or password.";
        emit Singleton<LoginController>::getInstance().finishedLoginControllerSignal(LOGINERROR); // 发送登录失败信号
    }

    db.close();
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
}
