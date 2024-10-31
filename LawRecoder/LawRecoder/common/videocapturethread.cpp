// VideoCaptureThread.cpp
#include "VideoCaptureThread.h"
#include <QDate>
#include <QDebug>
#include <QDir>
#include "../service/savevideoandpictureservice.h"
#include "../dao/picturedao.h"
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

        // 不加限制可能截图拿不到那一帧
        {
            QMutexLocker locker(&queueMutex);
            if (frameQueue.size() >= 50) {
                frameQueue.dequeue(); // 丢弃最旧的帧，保持队列长度
            }
            frameQueue.enqueue(frame.clone());
            //qDebug() << "Frame added to queue. Queue size:" << frameQueue.size();
        }

        // 转换颜色格式并发送帧
        cv::Mat rgbFrame;
        cv::cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB);
        QImage img((uchar*)rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);
        emit frameCaptured(img.copy());
        //qDebug() << "Frame captured and emitted.";
    }

    cap.release();
    emit finished();
}


void VideoCaptureThread::captureScreenshot() {
    // 加锁确保摄像头的独占访问,不然获取不到帧
    QMutexLocker locker(&frameMutex);
    if (!cap.isOpened()) {
        // 因为摄像头一直被播放线程占用，实际仍然可以拿到
        qWarning("Camera not open. Attempting to reopen...");
        if (!cap.open(0)) {
            qWarning("Failed to reopen camera for screenshot.");
            return;
        }
    }

    cv::Mat frame;
    cap >> frame;
    if (frame.empty()) {
        qWarning("Failed to capture screenshot - empty frame.");
        
        // 如果未能捕获到有效的帧，则准备 PictureDao 对象并写入 NULL
        PictureDao picture;
        picture.setPictureName("NULL"); // 使用 setter 方法
            picture.setPictureAddress("NULL"); // 根据实际需要填入
            picture.setPictureDate("NULL"); // 可以根据需要修改
            picture.setPictureUser("NULL"); // 可以根据需要修改
            picture.setPictureType(0); // 或者根据实际情况填入
            picture.setPicturePath("NULL"); // 图片路径为 NULL

        SaveVideoAndPictureService service;
        service.insertPictureInfo(picture); // 写入数据库
        return; // 返回
    }

    // 转换颜色格式并保存图像
    cv::Mat rgbFrame;
    cv::cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB);
    QImage img((uchar*)rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);

    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    QString picturePath = QDir::cleanPath(Singleton<DeviceService>::getInstance().getPicturePath() + "/" + timestamp + ".jpg");


    if (img.save(picturePath, "JPEG")) {
        qDebug() << "Screenshot saved at:" << picturePath;

        // 成功保存截图，准备 PictureDao 对象并写入数据库
        PictureDao picture;
        picture.setPictureName(timestamp); // 使用 setter 方法设置值
        picture.setPictureAddress("null"); // 保存的文件路径
        picture.setPictureDate(QDateTime::currentDateTime().toString("yyyy-MM-dd")); // 记录当前日期
        picture.setPictureUser("User"); // 根据实际需要填入
        picture.setPictureType(1); // 或者根据实际情况填入
        picture.setPicturePath(picturePath); // 图片路径也进行修改

        SaveVideoAndPictureService service;
        service.insertPictureInfo(picture); // 写入数据库

    } else {
        qDebug() << "Failed to save screenshot.";
    }
}
