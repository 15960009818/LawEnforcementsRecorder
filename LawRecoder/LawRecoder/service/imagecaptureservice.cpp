#include "imagecaptureservice.h"
#include "../dao/picturedao.h"
#include "../common/commondefs.h"
#include "../common/singleton.h"
#include "../controller/imagecapturecontroller.h"
#include <QDate>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

ImageCaptureService::ImageCaptureService(QObject *parent) : QObject(parent)
{
}

// 获取指定日期的图片信息，分页查询，每页 4 条记录
void ImageCaptureService::GetDateImageSlots(const QDate &date, const int &page)
{
    const int itemsPerPage = 4; // 每页固定返回 4 条记录
    int offset = (page - 1) * itemsPerPage; // 计算偏移量

    // 打开数据库连接
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data/video.db");

    if (!db.open()) {
        qDebug() << "Database open error:" << db.lastError().text();
        emit Singleton<ImageCaptureController>::getInstance()
                .finishedImageCaptureSignal(SQLERROR); // 发送错误信号
        return;
    }

    qDebug() << "Database opened successfully!";

    // 准备 SQL 查询语句
    QSqlQuery query(db);
    query.prepare("SELECT picture_id, picture_name, picture_address, picture_date, picture_user, picture_type, picture_path "
                  "FROM picture "
                  "WHERE picture_date = :date AND picture_type = 1 "
                  "LIMIT :limit OFFSET :offset");

    // 绑定参数
    query.bindValue(":date", date.toString("yyyy-MM-dd"));
    query.bindValue(":limit", itemsPerPage);
    query.bindValue(":offset", offset);

    QList<PictureDao> pictures; // 用于存储查询结果

    // 执行查询
    if (query.exec()) {
        while (query.next()) {
            PictureDao pic;

            // 从查询结果中提取数据并设置到 PictureDao
            pic.setPictureId(query.value(0).toInt());
            pic.setPictureName(query.value(1).toString());
            pic.setPictureAddress(query.value(2).toString());
            pic.setPictureDate(query.value(3).toString());
            pic.setPictureUser(query.value(4).toString());
            pic.setPictureType(query.value(5).toInt());
            pic.setPicturePath(query.value(6).toString());

            // 将 PictureDao 实例添加到列表中
            pictures.append(pic);
        }

        // 如果成功，发送成功状态和查询结果
        emit Singleton<ImageCaptureController>::getInstance()
                .finishedPictureQuerySignal(SQLSUCCESS, pictures); // 发送成功信号和图片列表
        qDebug() << "Query successful, found" << pictures.size() << "pictures.";
    } else {
        // 如果查询失败，输出错误并发送错误信号
        qDebug() << "Failed to execute query:" << query.lastError().text();
        emit Singleton<ImageCaptureController>::getInstance()
                .finishedImageCaptureSignal(SQLERROR); // 发送错误信号
    }

    // 关闭数据库
    db.close();
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
}
