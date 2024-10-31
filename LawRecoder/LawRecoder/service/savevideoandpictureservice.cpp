#include "savevideoandpictureservice.h"

SaveVideoAndPictureService::SaveVideoAndPictureService(QObject *parent) 
    : QObject(parent) 
{
}

bool SaveVideoAndPictureService::openDatabase(sqlite3 *&db) {
    int rc = sqlite3_open("data/video.db", &db); 
    if (rc) {
        qDebug() << "Can't open database:" << sqlite3_errmsg(db);
        return false;
    }
    return true;
}

bool SaveVideoAndPictureService::insertVideoInfo(const VideoDao& video) {
    sqlite3 *db;
    if (!openDatabase(db)) {
        return false;
    }

    const char *sql = "INSERT INTO video (video_name, video_address, video_date, video_user, video_type, video_path) VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        qDebug() << "Failed to prepare statement:" << sqlite3_errmsg(db);
        sqlite3_close(db);
        return false;
    }

    
    sqlite3_bind_text(stmt, 1, video.getVideoName().toUtf8().constData(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, video.getVideoAddress().toUtf8().constData(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, video.getVideoDate().toUtf8().constData(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, video.getVideoUser().toUtf8().constData(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, video.getVideoType());
    sqlite3_bind_text(stmt, 6, video.getVideoPath().toUtf8().constData(), -1, SQLITE_STATIC);

    
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        qDebug() << "Failed to insert video:" << sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}

bool SaveVideoAndPictureService::insertPictureInfo(const PictureDao& picture) {
    sqlite3 *db;
    if (!openDatabase(db)) {
        return false;
    }

    const char *sql = "INSERT INTO picture (picture_name, picture_address, picture_date, picture_user, picture_type, picture_path) VALUES (?, ?, ?, ?, ?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, nullptr) != SQLITE_OK) {
        qDebug() << "Failed to prepare statement:" << sqlite3_errmsg(db);
        sqlite3_close(db);
        return false;
    }

    // 绑定参数
    sqlite3_bind_text(stmt, 1, picture.getPictureName().toUtf8().constData(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, picture.getPictureAddress().toUtf8().constData(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, picture.getPictureDate().toUtf8().constData(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 4, picture.getPictureUser().toUtf8().constData(), -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 5, picture.getPictureType());
    sqlite3_bind_text(stmt, 6, picture.getPicturePath().toUtf8().constData(), -1, SQLITE_STATIC);

    
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        qDebug() << "Failed to insert picture:" << sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}
