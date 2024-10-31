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
    // 加锁确保摄像头的独占访问
    QMutexLocker locker(&frameMutex);
    if (!cap.isOpened()) {
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

        // 没有有效帧时写入默认的 NULL 数据
        PictureDao picture;
        picture.setPictureName("NULL");
        picture.setPictureAddress("NULL");
        picture.setPictureDate("NULL");
        picture.setPictureUser("NULL");
        picture.setPictureType(0);
        picture.setPicturePath("NULL");

        SaveVideoAndPictureService service;
        service.insertPictureInfo(picture);
        return;
    }

    // 转换颜色格式并保存图像
    cv::Mat rgbFrame;
    cv::cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB);
    QImage img((uchar*)rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);

    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    QString picturePath = Singleton<DeviceService>::getInstance().getPicturePath();

    // 检查 picturePath 是否为空，如果为空则使用默认路径
    if (picturePath.isEmpty()) {
        picturePath = QDir::homePath() + "/Pictures"; // 默认保存路径，例如用户主目录下的 Pictures 文件夹
        QDir().mkpath(picturePath); // 如果目录不存在则创建
    }

    QString outfile = QDir::cleanPath(picturePath + "/" + timestamp + ".jpg");

    if (img.save(outfile, "JPEG")) {
        qDebug() << "Screenshot saved at:" << outfile;

        // 成功保存截图，准备 PictureDao 对象并写入数据库
        PictureDao picture;
        picture.setPictureName(timestamp);
        picture.setPictureAddress("null");
        picture.setPictureDate(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
        picture.setPictureUser("User");
        picture.setPictureType(1);
        picture.setPicturePath(outfile);

        SaveVideoAndPictureService service;
        service.insertPictureInfo(picture);
    } else {
        qDebug() << "Failed to save screenshot.";
    }
}
