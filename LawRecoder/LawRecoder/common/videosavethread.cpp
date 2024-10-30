#include "VideoSaveThread.h"
#include <QDebug>

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

    outputFile = filename;
    frameWidth = width;
    frameHeight = height;
    frameRate = fps;
    stopFlag = false;
    qDebug() << "Starting save thread with file:" << outputFile;
    start();
}

void VideoSaveThread::stopSaving() {
    stopFlag = true;
    outputFile = deviceService.getVideoPath();
    qDebug() << "Starting save thread with file:" << outputFile;
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
    qDebug() << "Trying to open video writer for file:" << outputFile;

        // 打开视频写入器
        if (!videoWriter.open(outputFile.toStdString(), cv::VideoWriter::fourcc('M', 'P', '4', 'V'), frameRate, cv::Size(frameWidth, frameHeight))) {
            qWarning() << "Failed to open video writer for file:" << outputFile;
            return;
        }

        qDebug() << "Video writer opened successfully.";

        exec();

        videoWriter.release();
        qDebug() << "Video writer released.";
}



void VideoSaveThread::saveFrame(const QImage &image) {
    if (!stopFlag && videoWriter.isOpened()) {
        cv::Mat frame(image.height(), image.width(), CV_8UC3, (void*)image.bits(), image.bytesPerLine());
        cv::cvtColor(frame, frame, cv::COLOR_RGB2BGR);
        videoWriter.write(frame);
        qDebug() << "Frame saved.";
    }
}
