// CameraCapture.h
#ifndef CAMERACAPTURE_H
#define CAMERACAPTURE_H

#include <opencv2/opencv.hpp>
#include <QObject>
#include <QImage>
#include <QTimer>

class CameraCapture : public QObject {
    Q_OBJECT

public:
    explicit CameraCapture(QObject *parent = nullptr);
    ~CameraCapture();
    void startCapture();
    void stopCapture();

signals:
    void frameCaptured(const QImage &frame);

private slots:
    void captureFrame();

private:
    cv::VideoCapture cap;
    QTimer *timer;
};

#endif // CAMERACAPTURE_H