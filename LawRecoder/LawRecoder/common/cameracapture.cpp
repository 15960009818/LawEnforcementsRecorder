// CameraCapture.cpp
#include "cameracapture.h"


CameraCapture::CameraCapture(QObject *parent)
    : QObject(parent), timer(new QTimer(this)) {
    connect(timer, &QTimer::timeout, this, &CameraCapture::captureFrame);
}



CameraCapture::~CameraCapture() {
    stopCapture();
}

void CameraCapture::startCapture() {
    if (!cap.isOpened()) {
        cap.open(0); // 打开默认摄像头
        if (!cap.isOpened()) {
            qWarning("Failed to open camera.");
            return;
        }
    }
    timer->start(30); // 每30毫秒捕获一帧
}

void CameraCapture::stopCapture() {
    if (cap.isOpened()) {
        cap.release();
    }
    timer->stop();
}

void CameraCapture::captureFrame() {
    cv::Mat frame;
    cap >> frame; // 捕获一帧
    if (!frame.empty()) {
        // 转换 OpenCV Mat 为 QImage
        QImage img(frame.data, frame.cols, frame.rows, frame.step, QImage::Format_RGB888);
        emit frameCaptured(img.rgbSwapped()); // 发射信号传递图像
    }
}
