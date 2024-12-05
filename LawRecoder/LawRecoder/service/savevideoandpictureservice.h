#ifndef SAVEVIDEOANDPICTURESERVICE_H
#define SAVEVIDEOANDPICTURESERVICE_H

#include <QObject>
#include <QString>
#include <QSqlDatabase>
#include "../dao/videodao.h"
#include "../dao/picturedao.h"

class SaveVideoAndPictureService : public QObject
{
    Q_OBJECT
public:
    explicit SaveVideoAndPictureService(QObject *parent = nullptr);

    // 插入视频信息
    bool insertVideoInfo(const VideoDao& video);

    // 插入图片信息
    bool insertPictureInfo(const PictureDao& picture);

private:
    // 创建数据库连接
    QSqlDatabase createDatabaseConnection();
};

#endif // SAVEVIDEOANDPICTURESERVICE_H
