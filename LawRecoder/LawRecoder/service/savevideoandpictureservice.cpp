#include "savevideoandpictureservice.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>
#include <QThread>

SaveVideoAndPictureService::SaveVideoAndPictureService(QObject *parent)
    : QObject(parent)
{
}

QSqlDatabase SaveVideoAndPictureService::createDatabaseConnection() {
    const QString connectionName = QString("VideoAndPictureConnection_%1").arg((quintptr)QThread::currentThreadId());
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connectionName);
    db.setDatabaseName("data/video.db");

    if (!db.open()) {
        qDebug() << "Can't open database:" << db.lastError().text();
        return QSqlDatabase();
    }

    return db;
}

bool SaveVideoAndPictureService::insertVideoInfo(const VideoDao& video) {
    QSqlDatabase db = createDatabaseConnection();
    if (!db.isOpen()) {
        return false;
    }

    // 打印即将插入的视频信息
    qDebug() << "[DEBUG] Inserting video info:";
    qDebug() << "Name:" << video.getVideoName();
    qDebug() << "Address:" << video.getVideoAddress();
    qDebug() << "Date:" << video.getVideoDate();
    qDebug() << "User:" << video.getVideoUser();
    qDebug() << "Type:" << video.getVideoType();
    qDebug() << "Path:" << video.getVideoPath();

    QSqlQuery query(db);
    query.prepare("INSERT INTO video (video_name, video_address, video_date, video_user, video_type, video_path) "
                  "VALUES (:name, :address, :date, :user, :type, :path)");

    query.bindValue(":name", video.getVideoName());
    query.bindValue(":address", video.getVideoAddress());
    query.bindValue(":date", video.getVideoDate());
    query.bindValue(":user", video.getVideoUser());
    query.bindValue(":type", video.getVideoType());
    query.bindValue(":path", video.getVideoPath());

    if (!query.exec()) {
        qDebug() << "Failed to insert video:" << query.lastError().text();
        db.close();  // 关闭数据库连接
        return false;
    }

    db.close();  // 关闭数据库连接
    return true;
}

bool SaveVideoAndPictureService::insertPictureInfo(const PictureDao& picture) {
    QSqlDatabase db = createDatabaseConnection();
    if (!db.isOpen()) {
        return false;
    }

    // 打印即将插入的图片信息
    qDebug() << "[DEBUG] Inserting picture info:";
    qDebug() << "Name:" << picture.getPictureName();
    qDebug() << "Address:" << picture.getPictureAddress();
    qDebug() << "Date:" << picture.getPictureDate();
    qDebug() << "User:" << picture.getPictureUser();
    qDebug() << "Type:" << picture.getPictureType();
    qDebug() << "Path:" << picture.getPicturePath();

    QSqlQuery query(db);
    query.prepare("INSERT INTO picture (picture_name, picture_address, picture_date, picture_user, picture_type, picture_path) "
                  "VALUES (:name, :address, :date, :user, :type, :path)");

    query.bindValue(":name", picture.getPictureName());
    query.bindValue(":address", picture.getPictureAddress());
    query.bindValue(":date", picture.getPictureDate());
    query.bindValue(":user", picture.getPictureUser());
    query.bindValue(":type", picture.getPictureType());
    query.bindValue(":path", picture.getPicturePath());

    if (!query.exec()) {
        qDebug() << "Failed to insert picture:" << query.lastError().text();
        db.close();  // 关闭数据库连接
        return false;
    }

    db.close();  // 关闭数据库连接
    return true;
}
