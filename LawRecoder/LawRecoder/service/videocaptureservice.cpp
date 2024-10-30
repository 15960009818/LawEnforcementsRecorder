#include "videocaptureservice.h"
#include "../../sqlite3.h"
#include "../dao/videodao.h"
#include "../common/commondefs.h"
#include "../common/singleton.h"
#include "../controller/videocapturecontroller.h"
#include <QDebug>

VideoCaptureService::VideoCaptureService(QObject *parent) : QObject(parent) {

}

// 获取指定日期的视频信息，分页查询，每页 4 条记录
void VideoCaptureService::GetDateVideoSlots(const QDate &date, const int &page) {
    sqlite3 *db = nullptr;
    sqlite3_stmt *stmt;
    int rc;
    const int itemsPerPage = 4; // 每页固定返回 4 条记录
    int offset = (page - 1) * itemsPerPage; // 计算偏移量

    // 打开数据库
    int res = sqlite3_open("data/video.db", &db);
    if (res != SQLITE_OK) {
        qDebug() << "Database open error:" << sqlite3_errmsg(db);
        emit Singleton<VideoCaptureController>::getInstance().errorOccurredSignal(SQLERROR); // 发送错误信号
        return;
    }

    qDebug() << "Database opened successfully!";

    const char *sql = "SELECT video_id, video_name, video_address, video_date, video_user, video_type, video_path "
                      "FROM video "
                      "WHERE video_date = ? AND video_type = 1 "
                      "LIMIT ? OFFSET ?";

    // 准备 SQL 语句
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        qDebug() << "Failed to prepare statement:" << sqlite3_errmsg(db);
        sqlite3_close(db);
        emit Singleton<VideoCaptureController>::getInstance().errorOccurredSignal(SQLERROR); // 发送错误信号
        return;
    }

    // 绑定日期参数
    sqlite3_bind_text(stmt, 1, date.toString("yyyy-MM-dd").toUtf8().constData(), -1, SQLITE_STATIC);
    // 绑定 LIMIT 参数 (itemsPerPage 固定为 4)
    sqlite3_bind_int(stmt, 2, itemsPerPage);
    // 绑定 OFFSET 参数
    sqlite3_bind_int(stmt, 3, offset);

    QList<VideoDao> videos; // 用于存储查询结果
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW) {
        VideoDao video;

        // 从数据库查询结果中设置 VideoDao 的字段
        video.setVideoId(sqlite3_column_int(stmt, 0));
        video.setVideoName(QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 1))));
        video.setVideoAddress(QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 2))));
        video.setVideoDate(QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 3))));
        video.setVideoUser(QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 4))));
        video.setVideoType(sqlite3_column_int(stmt, 5));
        video.setVideoPath(QString::fromUtf8(reinterpret_cast<const char*>(sqlite3_column_text(stmt, 6))));

        // 将 VideoDao 实例添加到列表中
        videos.append(video);
    }

    if (rc != SQLITE_DONE) {
        qDebug() << "Failed to execute query:" << sqlite3_errmsg(db);
        emit Singleton<VideoCaptureController>::getInstance().errorOccurredSignal(SQLERROR); // 发送错误信号
    } else {
        // 如果成功，发送成功状态和查询结果
        emit videoListReady(videos); // 发送成功信号和视频列表
        qDebug() << "Query successful, found" << videos.size() << "videos.";
    }

    // 释放查询语句资源
    sqlite3_finalize(stmt);
    // 关闭数据库
    sqlite3_close(db);
}
