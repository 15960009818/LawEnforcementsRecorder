#ifndef DEVICESERVICE_H
#define DEVICESERVICE_H

#include <QObject>

class DeviceService:public QObject
{
    Q_OBJECT
public:
    DeviceService();
    QString getVideoPath();
    QString getPicturePath();
    void setVideoPath(const QString *VideoPath);
    void setPicturePath(const QString *PicturePath);
private:
    QString VideoPath;
    QString PicturePath;
};

#endif // DEVICESERVICE_H
