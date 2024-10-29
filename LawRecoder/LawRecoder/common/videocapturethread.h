// VideoCaptureThread.h
#ifndef VIDEOCAPTURETHREAD_H
#define VIDEOCAPTURETHREAD_H

#include <QThread>
#include <QImage>
#include <opencv2/opencv.hpp>

class VideoCaptureThread : public QThread {
    Q_OBJECT

public:
    VideoCaptureThread(QObject *parent = nullptr);
    ~VideoCaptureThread();
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
};

#endif // VIDEOCAPTURETHREAD_H
