#ifndef SAVEVIDEOANDPICTURESERVICE_H
#define SAVEVIDEOANDPICTURESERVICE_H

#include <QObject>
#include <QString>
#include <QList>
#include <QDebug>
#include "../sqlite3.h"
#include "../dao/videodao.h"
#include "../dao/picturedao.h"
class SaveVideoAndPictureService : public QObject
{
    Q_OBJECT
public:
    explicit SaveVideoAndPictureService(QObject *parent = nullptr);

     bool insertVideoInfo(const VideoDao& video);
     bool insertPictureInfo(const PictureDao& picture);
private:
    bool openDatabase(sqlite3 *&db); // 数据库打开辅助函数
    
};

#endif // SAVEVIDEOANDPICTURESERVICE_H
