// VideoCaptureThread.cpp
#include "VideoCaptureThread.h"
#include <QDebug>

VideoCaptureThread::VideoCaptureThread(QObject *parent) : QThread(parent), stopFlag(false) {
    qDebug() << "VideoCaptureThread initialized.";
}

VideoCaptureThread::~VideoCaptureThread() {
    stop(); // 确保线程退出并释放摄像头资源
    qDebug() << "VideoCaptureThread destroyed.";
}

void VideoCaptureThread::startCapturing() {
    if (!isRunning()) { // 确保线程未在运行时才启动
        stopFlag = false;
        qDebug() << "Starting capture thread...";
        start();
    } else {
        qDebug() << "Capture thread is already running.";
    }
}

void VideoCaptureThread::stop() {
    stopFlag = true;
    if (isRunning()) {
        qDebug() << "Stopping capture thread...";
        wait(); // 等待线程退出
        qDebug() << "Capture thread stopped.";
    }
}

void VideoCaptureThread::run() {
    stopFlag = false;  // 确保每次启动时都将 stopFlag 置为 false

    if (!cap.isOpened()) {  // 确保摄像头仅打开一次
        cap.open(0);  // 打开默认摄像头
        if (!cap.isOpened()) {
            qWarning("Failed to open camera.");
            return;
        }
        qDebug() << "Camera opened successfully.";
    }

    while (!stopFlag) {
        if (!cap.isOpened()) {  // 再次检查摄像头状态
            qWarning("Camera closed unexpectedly.");
            break;
        }

        cv::Mat frame;
        cap >> frame;  // 读取帧
        if (frame.empty()) {
            qWarning() << "Empty frame received.";
            break;
        }

        // 转换颜色格式并发送帧
        cv::Mat rgbFrame;
        cv::cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB);
        QImage img((uchar*)rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);
        emit frameCaptured(img.copy());
        qDebug() << "Frame captured and emitted.";
    }

    if (cap.isOpened()) {
        cap.release();  // 释放摄像头资源
        qDebug() << "Camera capture released.";
    }

    emit finished(); // 通知捕获完成
}

