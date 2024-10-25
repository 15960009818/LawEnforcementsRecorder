#ifndef VIDEOCAPTURESERVICE_H
#define VIDEOCAPTURESERVICE_H

#include <QObject>
#include <QDate>
#include <QList>
#include "../dao/videodao.h" // 假设有一个用于视频信息的类

class VideoCaptureService : public QObject {
    Q_OBJECT

public:
    explicit VideoCaptureService(QObject *parent = nullptr);

    void GetDateVideoSlots(const QDate &date, const int &page); // 获取指定日期的视频信息

signals:
    void videoListReady(const QList<VideoDao> &videoList); // 视频列表准备好信号
    void errorOccurred(int errorCode); // 错误发生信号

};

#endif // VIDEOCAPTURESERVICE_H
