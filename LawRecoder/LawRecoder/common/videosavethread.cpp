#include "VideoSaveThread.h"
#include <QDate>
#include <QDebug>
#include <QDir>
#include "../service/savevideoandpictureservice.h"

VideoSaveThread::VideoSaveThread(QObject *parent) : QThread(parent), stopFlag(false) {
    qDebug() << "VideoSaveThread initialized.";
}

VideoSaveThread::~VideoSaveThread() {
    stop();
    qDebug() << "VideoSaveThread destroyed.";
}

void VideoSaveThread::startSaving(const QString &filename, int width, int height, int fps) {
    if (filename.isEmpty()) {
        qWarning() << "Filename cannot be empty.";
        return; // 或者抛出异常
    }

    outputFile = QDir::cleanPath(filename);
    frameWidth = width;
    frameHeight = height;
    frameRate = fps;
    stopFlag = false;

    qDebug() << "Starting save thread with file:" << outputFile;
    start();
}

void VideoSaveThread::stopSaving() {
    qDebug() << "Request to stop saving.";
    stop();
}

void VideoSaveThread::stop() {
    stopFlag = true;
    qDebug() << "Stopping save thread...";

    // 先停止视频写入
    if (videoWriter.isOpened()) {
        videoWriter.release();
        qDebug() << "Video writer released.";
    }

    // 手动退出线程的事件循环
    quit();
    wait(); // 等待线程退出
    qDebug() << "Save thread stopped.";
}

void VideoSaveThread::run() {
    // 从设备服务中获取视频路径
    QString videoPath = Singleton<DeviceService>::getInstance().getVideoPath();
    outputFile = QDir::cleanPath(videoPath + "/" + QFileInfo(outputFile).fileName());

    qDebug() << "Trying to open video writer for file:" << outputFile;

    // 打开视频写入器
    if (!videoWriter.open(outputFile.toStdString(), cv::VideoWriter::fourcc('M', 'P', '4', 'V'), frameRate, cv::Size(frameWidth, frameHeight))) {
        qWarning() << "Failed to open video writer for file:" << outputFile;
        return;
    }

    qDebug() << "Video writer opened successfully.";

    // 获取当前时间作为视频日期
    QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

    // 准备 VideoDao 对象并填充视频信息
    VideoDao video;
    video.setVideoName("Sample Video");
    video.setVideoAddress(outputFile);
    video.setVideoDate(currentDateTime);
    video.setVideoUser("User");
    video.setVideoType(1);
    video.setVideoPath(outputFile);

    // 插入视频信息到数据库
    SaveVideoAndPictureService *savevideoandpictureservice = new SaveVideoAndPictureService;
    bool result = savevideoandpictureservice->insertVideoInfo(video);
    delete savevideoandpictureservice; // 释放内存

    if (result) {
        qDebug() << "[DEBUG] Video information inserted successfully.";
    } else {
        qWarning() << "[DEBUG] Failed to insert video information.";
    }

    exec(); // 启动事件循环

    videoWriter.release();
    qDebug() << "Video writer released.";
}

void VideoSaveThread::saveFrame(const QImage &image) {
    if (!stopFlag && videoWriter.isOpened()) {
        cv::Mat frame(image.height(), image.width(), CV_8UC3, (void*)image.bits(), image.bytesPerLine());
        cv::cvtColor(frame, frame, cv::COLOR_RGB2BGR);
        videoWriter.write(frame);
        //qDebug() << "Frame saved.";
    }
}
