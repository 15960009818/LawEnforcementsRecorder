#ifndef DEVICESERVICE_H
#define DEVICESERVICE_H

#include <QObject>
#include "../common/singleton.h"
class DeviceService:public QObject
{
    Q_OBJECT
    SINGLETON(DeviceService)//单例模式让所以设置的路径和图片视频保存路径相同
public:

    QString getVideoPath();
    QString getPicturePath();
    void setVideoPath(const QString *VideoPath);
    void setPicturePath(const QString *PicturePath);
private:
    QString VideoPath;
    QString PicturePath;

};

#endif // DEVICESERVICE_H
