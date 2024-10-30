// VideoCaptureThread.cpp
#include "VideoCaptureThread.h"
#include <QDate>
#include <QDebug>
#include <QDir>

VideoCaptureThread::VideoCaptureThread(QObject *parent) : QThread(parent), stopFlag(false) {
    deviceService = new DeviceService;
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
    QMutexLocker locker(&frameMutex);
     // 加锁保护摄像头操作
    stopFlag = false;
    if (!cap.open(0)) {
        qWarning("Failed to open camera.");
        return;
    }
    qDebug() << "Camera opened successfully.";

    while (!stopFlag) {
        cv::Mat frame;
        cap >> frame;
        if (frame.empty()) {
            qWarning() << "Empty frame received.";
            continue;
        }

        // 使用互斥锁保护队列并限制大小
        {
            QMutexLocker locker(&queueMutex);
            if (frameQueue.size() >= 50) {
                frameQueue.dequeue(); // 丢弃最旧的帧，保持队列长度
            }
            frameQueue.enqueue(frame.clone());
            qDebug() << "Frame added to queue. Queue size:" << frameQueue.size();
        }

        // 转换颜色格式并发送帧
        cv::Mat rgbFrame;
        cv::cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB);
        QImage img((uchar*)rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);
        emit frameCaptured(img.copy());
        qDebug() << "Frame captured and emitted.";
    }

    cap.release();
    emit finished();
}

void VideoCaptureThread::captureScreenshot() {
    QMutexLocker locker(&frameMutex); // 加锁确保摄像头的独占访问
    if (!cap.isOpened()) {
        qWarning("Camera not open. Attempting to reopen...");
        if (!cap.open(0)) {
            qWarning("Failed to reopen camera for screenshot.");
            return;
        }
    }

    cv::Mat frame;
    cap >> frame; // 直接捕获当前帧
    if (frame.empty()) {
        qWarning("Failed to capture screenshot - empty frame.");
        return;
    }

    // 转换颜色格式并保存图像
    cv::Mat rgbFrame;
    cv::cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB);
    QImage img((uchar*)rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);

    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    QString filePath = deviceService->getPicturePath() + timestamp + ".jpg";


    if (img.save(filePath, "JPEG")) {
        qDebug() << "Screenshot saved at:" << filePath;
    } else {
        qDebug() << "Failed to save screenshot.";
    }
}
