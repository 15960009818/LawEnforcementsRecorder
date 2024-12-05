// VideoCaptureThread.cpp
#include "videocapturethread.h"
#include <QDate>
#include <QDebug>
#include <QDir>
#include "../service/savevideoandpictureservice.h"
#include "../dao/picturedao.h"
VideoCaptureThread::VideoCaptureThread() {

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

    // 设置支持的格式
    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
    cap.set(cv::CAP_PROP_FPS, 30);
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

    qDebug() << "Camera opened successfully.";

    while (!stopFlag) {
        cv::Mat frame;
        cap >> frame;

        if (frame.empty()) {
            qWarning() << "Empty frame received.";
            continue;
        }

        {
            QMutexLocker locker(&queueMutex);
            if (frameQueue.size() >= 50) {
                frameQueue.dequeue(); // 丢弃最旧的帧，保持队列长度
            }
            frameQueue.enqueue(frame.clone());
        }

        // 转换颜色格式并发送帧
        cv::Mat rgbFrame;
        cv::cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB);
        QImage img((uchar*)rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);
        emit frameCaptured(img.copy());
    }

    cap.release();
    emit finished();
}



void VideoCaptureThread::captureScreenshot() {
    cv::Mat frame;
    {
        QMutexLocker locker(&queueMutex);
        if (!frameQueue.isEmpty()) {
            frame = frameQueue.dequeue(); // 从队列中获取最旧的帧
        } else {
            qWarning("Frame queue is empty, unable to capture screenshot.");
            return;
        }
    }

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

    // 使用 MatToQImage 进行图像转换
    QImage img = MatToQImage(frame);

    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    QString picturePath = Singleton<DeviceService>::getInstance().getPicturePath();

    if (picturePath.isEmpty()) {
        picturePath = QDir::homePath() + "/Pictures";
        QDir().mkpath(picturePath);
    }

    QString outfile = QDir::cleanPath(picturePath + "/" + timestamp + ".jpg");

    if (img.save(outfile, "JPEG")) {
        qDebug() << "Screenshot saved at:" << outfile;

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


QImage VideoCaptureThread::MatToQImage(const cv::Mat &mat) {
    switch (mat.type()) {
    case CV_8UC1: {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        image.setColorCount(256);
        for(int i = 0; i < 256; i++) {
            image.setColor(i, qRgb(i, i, i));
        }

        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row++) {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    case CV_8UC3: {
        const uchar *pSrc = (const uchar*)mat.data;
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();  // OpenCV 的 BGR 转为 RGB
    }
    case CV_8UC4: {
        const uchar *pSrc = (const uchar*)mat.data;
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    default:
        qWarning() << "Unsupported image format for Mat to QImage conversion.";
        break;
    }
    return QImage();
}
