// VideoCaptureThread.h
#ifndef VIDEOCAPTURETHREAD_H
#define VIDEOCAPTURETHREAD_H

#include <QThread>
#include <QImage>
#include <QMutex>
#include <QWaitCondition>
#include <opencv2/opencv.hpp>
#include <QQueue>
#include "../service/deviceservice.h"
class VideoCaptureThread : public QThread {
    Q_OBJECT

public:
    VideoCaptureThread(QObject *parent = nullptr);
    ~VideoCaptureThread();
     void captureScreenshot();
    void startCapturing();
    void stop();  // 添加停止方法

signals:
    void frameCaptured(const QImage &image);
    void finished();  // 新增停止信号

protected:
    void run() override;

private:
    bool stopFlag;
    cv::VideoCapture cap; // 将摄像头对象放在类中
    QMutex frameMutex; // 互斥锁
    cv::Mat latestFrame; // 最新帧缓存
    QQueue<cv::Mat> frameQueue;
    QMutex queueMutex;
    QWaitCondition frameAvailable;
    cv::Mat frame;
    DeviceService *deviceService;

};

#endif // VIDEOCAPTURETHREAD_H
