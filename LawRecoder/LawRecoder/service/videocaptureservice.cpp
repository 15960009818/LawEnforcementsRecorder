#include "videocaptureservice.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "../dao/videodao.h"
#include "../common/commondefs.h"
#include "../common/singleton.h"
#include "../controller/videocapturecontroller.h"

VideoCaptureService::VideoCaptureService(QObject *parent) : QObject(parent) {
}

// 获取指定日期的视频信息，分页查询，每页 4 条记录
void VideoCaptureService::GetDateVideoSlots(const QDate &date, const int &page) {
    const int itemsPerPage = 4; // 每页固定返回 4 条记录
    int offset = (page - 1) * itemsPerPage; // 计算偏移量

    // 打开数据库
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("data/video.db");

    if (!db.open()) {
        qDebug() << "Database open error:" << db.lastError().text();
        emit Singleton<VideoCaptureController>::getInstance().errorOccurredSignal(SQLERROR); // 发送错误信号
        return;
    }

    qDebug() << "Database opened successfully!";

    QSqlQuery query(db);
    query.prepare("SELECT video_id, video_name, video_address, video_date, video_user, video_type, video_path "
                  "FROM video "
                  "WHERE DATE(video_date) = DATE(:video_date) AND video_type = 1 "
                  "LIMIT :limit OFFSET :offset");

    // 绑定参数
    qDebug() << "Binding parameters:";
    qDebug() << ":video_date ->" << date.toString("yyyy-MM-dd");
    qDebug() << ":limit ->" << itemsPerPage;
    qDebug() << ":offset ->" << offset;
    // 绑定参数，确保传入的日期格式为 'yyyy-MM-dd'
    query.bindValue(":video_date", date.toString("yyyy-MM-dd"));
    query.bindValue(":limit", itemsPerPage);
    query.bindValue(":offset", offset);

    QList<VideoDao> videos; // 用于存储查询结果

    if(query.exec())
    {
    while (query.next()) {
        VideoDao video;
        video.setVideoId(query.value(0).toInt());
        video.setVideoName(query.value(1).toString());
        video.setVideoAddress(query.value(2).toString());
        video.setVideoDate(query.value(3).toString());
        video.setVideoUser(query.value(4).toString());
        video.setVideoType(query.value(5).toInt());
        video.setVideoPath(query.value(6).toString());

        // 将 VideoDao 实例添加到列表中
        videos.append(video);
    }

    // 如果成功，发送成功状态和查询结果
    emit videoListReady(videos); // 发送成功信号和视频列表
    qDebug() << "Query successful, found" << videos.size() << "videos.";
    }else
    {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        emit Singleton<VideoCaptureController>::getInstance().errorOccurredSignal(SQLERROR); // 发送错误信号
        return;
    }
    // 关闭数据库
    db.close();
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
}
