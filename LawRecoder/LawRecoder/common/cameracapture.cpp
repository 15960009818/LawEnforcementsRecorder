/*
 * has been fail
 **/
#include "cameracapture.h"
#include <QDebug>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

CameraCapture::CameraCapture(QObject *parent)
    : QObject(parent), timer(new QTimer(this)), cap() {
    connect(timer, &QTimer::timeout, this, &CameraCapture::captureFrame);
    qInfo() << "CameraCapture initialized";  // 信息日志
}

CameraCapture::~CameraCapture() {
    stopCapture();
    qInfo() << "CameraCapture destroyed";  // 信息日志
}

void CameraCapture::startCapture() {
#ifdef __arm__  // 如果是 ARM 架构的设备
    qDebug() << "Running on ARM architecture, starting GStreamer pipeline";  // 日志信息

    QString gstPipeline =
            "gst-launch-1.0 v4l2src device=/dev/video0 ! "
            "video/x-raw,format=YUY2,width=1024,height=768,framerate=30/1 ! "
            "videoconvert ! video/x-raw,format=RGB ! appsink";

    process->start(gstPipeline);

    if (!process->waitForStarted()) {
        qWarning() << "Failed to start GStreamer pipeline";
        return;
    }

    qInfo() << "GStreamer pipeline started";
    timer->start(30); // 每30毫秒处理一次图像
#else
    std::string devicePath = "/dev/video0";
    qDebug() << "Running on non-ARM architecture, using OpenCV with device:" << QString::fromStdString(devicePath);  // 日志信息

    cap.open(devicePath, cv::CAP_V4L2);

    if (!cap.isOpened()) {
        qWarning() << "Failed to open camera at" << QString::fromStdString(devicePath);
        return;
    }

    cap.set(cv::CAP_PROP_FRAME_WIDTH, 1024);
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 768);
    cap.set(cv::CAP_PROP_FPS, 30);

    timer->start(30); // 每30毫秒捕获一帧
    qInfo() << "Frame capture started";
#endif
}

void CameraCapture::stopCapture() {
#ifdef __arm__
    qDebug() << "Stopping GStreamer pipeline";  // 日志信息
    if (process->state() != QProcess::NotRunning) {
        process->kill();
        process->waitForFinished();
    }

    timer->stop();
    qInfo() << "Camera capture stopped";
#else
    qDebug() << "Releasing OpenCV capture";  // 日志信息
    if (cap.isOpened()) {
        cap.release();
    }

    timer->stop();
    qInfo() << "Camera capture stopped";
#endif
}

void CameraCapture::captureFrame() {
#ifdef __arm__
    if (!process->canReadLine()) {
        qWarning() << "No frame available from GStreamer pipeline";
        return;
    }

    QByteArray data = process->readLine();
    QImage img = processFrame(data); // 处理数据并转换为图像
    if (!img.isNull()) {
        emit frameCaptured(img);
    }
#else
    cv::Mat frame;
    cap >> frame;
    if (!frame.empty()) {
        QImage img(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        emit frameCaptured(img.rgbSwapped());
    }
#endif
}

QImage CameraCapture::processFrame(const QByteArray &data) {
#ifdef __arm__
    // 假设传输的数据为 RGB 格式，直接转换
    int width = 1024;
    int height = 768;
    QImage img(reinterpret_cast<const uchar *>(data.data()), width, height, QImage::Format_RGB888);
    return img.rgbSwapped();
#else
    return QImage();
#endif
}
