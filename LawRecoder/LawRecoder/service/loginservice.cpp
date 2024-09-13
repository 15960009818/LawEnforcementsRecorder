#include "loginservice.h"
#include <QDateTime>
#include <QDebug>
#include <../sqlite3.h>
#include "../controller/logincontroller.h"
#include "../common/singleton.h"
#include "../common/commondefs.h"

LoginService::LoginService(QObject *parent) : QObject(parent)
{
}

void LoginService::GetLoginCheckSlots(const QString &userAccount, const QString &userPassword)
{
    sqlite3 *db=nullptr;
    sqlite3_stmt *stmt;
    int rc;

    //打开数据库

    int res = sqlite3_open("data/video.db", &db);

    if(res == 0)//打开成功

    {

    qDebug()<<"open dataBase success!"<<endl;

    }

    else

    {

    qDebug()<<sqlite3_errcode(db)<<endl;

    qDebug()<<sqlite3_errmsg(db)<<endl;

    }

    // 准备 SQL 查询语句
    const char *sql = "SELECT user_id FROM user WHERE user_account = ? AND user_password = ?";

    // 准备 SQL 语句
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        qDebug() << "[DEBUG] Failed to prepare statement:" << sqlite3_errmsg(db);
        sqlite3_close(db);
        emit Singleton<LoginController>::getInstance().finishedLoginControllerSignal(SQLERROR);   // 语句准备失败
        return;
    }

    // 绑定查询参数
    sqlite3_bind_text(stmt, 1, userAccount.toUtf8().constData(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, userPassword.toUtf8().constData(), -1, SQLITE_STATIC);

    // 执行查询
    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW) {
        int userId = sqlite3_column_int(stmt, 0);
        qDebug() << "[DEBUG] Login successful, user ID:" << userId;

        // 更新用户的 `user_newlogintime`
        const char *update_sql = "UPDATE user SET user_newlogintime = ? WHERE user_id = ?";
        sqlite3_stmt *update_stmt;
        rc = sqlite3_prepare_v2(db, update_sql, -1, &update_stmt, nullptr);
        if (rc != SQLITE_OK) {
            qDebug() << "[DEBUG] Failed to prepare update statement:" << sqlite3_errmsg(db);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            emit Singleton<LoginController>::getInstance().finishedLoginControllerSignal(SQLERROR);  // 更新语句准备失败
            return;
        }

        // 获取当前时间并绑定参数
        QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");
        sqlite3_bind_text(update_stmt, 1, currentTime.toUtf8().constData(), -1, SQLITE_STATIC);
        sqlite3_bind_int(update_stmt, 2, userId);

        // 执行更新操作
        rc = sqlite3_step(update_stmt);
        if (rc != SQLITE_DONE) {
            qDebug() << "[DEBUG] Failed to update user_newlogintime:" << sqlite3_errmsg(db);
            emit Singleton<LoginController>::getInstance().finishedLoginControllerSignal(SQLERROR); // 更新失败
        } else {
            qDebug() << "[DEBUG] Updated user_newlogintime successfully.";
            emit Singleton<LoginController>::getInstance().finishedLoginControllerSignal(SQLSUCCESS);  // 登录成功
        }

        // 释放更新语句资源
        sqlite3_finalize(update_stmt);
    } else {
        qDebug() << "[DEBUG] Login failed: incorrect account or password.";
        emit Singleton<LoginController>::getInstance().finishedLoginControllerSignal(LOGINERROR);
        qDebug() << "[DEBUG] Emitting finishedLoginControllerSignal with value:" << LOGINERROR;
    }

    // 释放查询语句资源
    sqlite3_finalize(stmt);

    // 关闭数据库
    sqlite3_close(db);
}
