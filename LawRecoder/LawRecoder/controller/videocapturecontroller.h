#ifndef VIDEOCAPTURECONTROLLER_H
#define VIDEOCAPTURECONTROLLER_H

#include <QObject>
#include <QDate>
#include <QList>
#include <QThread>
#include "../dao/videodao.h" // 假设你有一个用于视频信息的类
#include "../service/videocaptureservice.h" // 视频捕获服务的类
#include "../common/singleton.h"
class VideoCaptureController : public QObject {
    Q_OBJECT
    SINGLETON(VideoCaptureController)
public:



private:
    void initController(); // 初始化控制器

public slots:
    void getVideoMessageSlot(QDate videoDate, int row); // 获取视频消息的槽函数
    void sendVideoMessageSlot(int SQLRESULT, const QList<VideoDao> &videoMessage); // 发送视频消息的槽函数

signals:
    void finishedVideoQuerySignal(int resultCode, const QList<VideoDao> &videoList); // 视频查询完成信号
    void finishVideoSignal(const QString &message, const QList<VideoDao> &videoList); // 完成视频信号
    void errorOccurredSignal(int sqlResult);
    void dateSignal(QDate selectedDate,int page);
private:
    QThread *threadGetVideoMessage; // 视频获取线程
    VideoCaptureService *videoCaptureService; // 视频捕获服务实例
    QDate getCurrentDate();
};

#endif // VIDEOCAPTURECONTROLLER_H
