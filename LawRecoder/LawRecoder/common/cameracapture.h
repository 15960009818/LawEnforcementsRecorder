#ifndef CAMERACAPTURE_H
#define CAMERACAPTURE_H

#include <opencv2/opencv.hpp>
#include <QObject>
#include <QImage>
#include <QTimer>
#include <QProcess>

extern "C" {
#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libswscale/swscale.h>
}

class CameraCapture : public QObject {
    Q_OBJECT

public:
    explicit CameraCapture(QObject *parent = nullptr);
    ~CameraCapture();
    void startCapture();
    void stopCapture();

    QImage processFrame(const QByteArray &data);
signals:
    void frameCaptured(const QImage &frame);

private slots:
    void captureFrame();

private:
    cv::VideoCapture cap;
    QTimer *timer;
    QProcess *process;
    // FFmpeg-specific parameters
    AVFormatContext *formatContext = nullptr;
    AVCodecContext *codecContext = nullptr;
    int videoStreamIndex = -1;
};

#endif // CAMERACAPTURE_H

