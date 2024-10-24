#include "imagecaptureservice.h"
#include "../../sqlite3.h"
#include "../dao/picturedao.h"
#include "../common/commondefs.h"
#include "../common/singleton.h"
#include "../controller/imagecapturecontroller.h"
#include <QDate>
#include <QString>

ImageCaptureService::ImageCaptureService(QObject *parent) : QObject(parent)
{
}

// 获取指定日期的图片信息
void ImageCaptureService::GetDateImageSlots(const QDate &date , const int &Row)
{
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt;
    int rc;

    // 打开数据库
    int res = sqlite3_open("data/video.db", &db);
    if (res != SQLITE_OK) {
        qDebug() << "Database open error:" << sqlite3_errmsg(db);
        emit Singleton<ImageCaptureController>::getInstance()
                .finishedImageCaptureSignal(SQLERROR); // 发送错误信号
        return;
    }

    qDebug() << "Database opened successfully!";

    // 准备 SQL 查询语句
    const char *sql = "SELECT picture_id, picture_name, picture_address, picture_date, picture_user, picture_type, picture_path "
                      "FROM picture "
                      "WHERE picture_date = ? AND picture_type = 1";

    // 准备 SQL 语句
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        qDebug() << "Failed to prepare statement:" << sqlite3_errmsg(db);
        sqlite3_close(db);
        emit Singleton<ImageCaptureController>::getInstance()
                .finishedImageCaptureSignal(SQLERROR); // 发送错误信号
        return;
    }

    // 绑定日期参数
    sqlite3_bind_text(stmt, 1, date.toString("yyyy-MM-dd").toUtf8().constData(), -1, SQLITE_STATIC);

    QList<PictureDao> pictures; // 用于存储查询结果
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        PictureDao pic;

        // 从数据库查询结果中设置 PictureDao 的字段
        pic.setPictureId(sqlite3_column_int(stmt, 0));
        pic.setPictureName(QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));
        pic.setPictureAddress(QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))));
        pic.setPictureDate(QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))));
        pic.setPictureUser(QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))));
        pic.setPictureType(sqlite3_column_int(stmt, 5));
        pic.setPicturePath(QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6))));

        // 将 PictureDao 实例添加到列表中
        pictures.append(pic);
    }

    if (rc != SQLITE_DONE) {
        qDebug() << "Failed to execute query:" << sqlite3_errmsg(db);
        emit Singleton<ImageCaptureController>::getInstance()
                .finishedImageCaptureSignal(SQLERROR); // 发送错误信号
    } else {
        // 如果成功，发送成功状态和查询结果
        emit Singleton<ImageCaptureController>::getInstance()
                .finishedPictureQuerySignal(SQLSUCCESS, pictures); // 发送成功信号和图片列表
        qDebug() << "Query successful, found" << pictures.size() << "pictures.";
    }

    // 释放查询语句资源
    sqlite3_finalize(stmt);
    // 关闭数据库
    sqlite3_close(db);
}

