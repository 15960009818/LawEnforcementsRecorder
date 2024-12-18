#ifndef VIDEOCAPTURETHREAD_H
#define VIDEOCAPTURETHREAD_H

#include <QThread>
#include <QImage>
#include <QMutex>
#include <QWaitCondition>
#include <QQueue>
#include <opencv2/opencv.hpp>
#include <QRunnable>
#include <QList>
extern "C" {
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
#include <libswscale/swscale.h>
#include <libavdevice/avdevice.h>

}

#include "../service/deviceservice.h"
#include "../service/savevideoandpictureservice.h"
#include "../common/singleton.h"

class VideoCaptureThread : public QThread {
    Q_OBJECT
    SINGLETON(VideoCaptureThread);

public:


    void captureScreenshot();
    void startCapturing();
    void stop();

    QImage MatToQImage(const cv::Mat &mat);

    //void captureWithFFmpeg();
    void startCapture();
    void captureFrames();
    void captureWithFFmpeg();

signals:
    void frameCaptured(const QImage &img);
    void finished();

protected:
    void run() override;

private:
    bool stopFlag;

    // OpenCV相关
    cv::VideoCapture cap;
    cv::Mat latestFrame;
    QMutex frameMutex;

    // 帧队列
    QQueue<cv::Mat> frameQueue;
    QMutex queueMutex;
    QWaitCondition frameAvailable;

    // FFmpeg相关
    AVFormatContext *formatContext = nullptr;
    AVCodecContext *codecContext = nullptr;
    AVFrame *avFrame = nullptr;
    AVPacket *packet = nullptr;
    SwsContext *swsContext = nullptr;
    int videoStreamIndex = -1;
    QMutex captureMutex;
    QWaitCondition captureCondition;
    QList<QImage> frameBuffer;
    SaveVideoAndPictureService *saveVideoAndPictureService;
    QQueue<AVPacket> packetBuffer;
    void initializeFFmpeg();
    void cleanupFFmpeg();
    int findVideoStreamIndex(AVFormatContext *formatContext);
    AVCodecContext *initializeCodecContext(AVFormatContext *formatContext, int videoStreamIndex);
    //void processFrame(AVCodecContext *codecContext, AVFrame *frame, SwsContext *swsCtx);
    void cleanupFFmpeg(AVFormatContext *formatContext, AVCodecContext *codecContext, AVPacket *packet, AVFrame *frame, SwsContext *swsCtx);
    void processFrame(AVCodecContext *codecContext, AVFrame *frame, SwsContext *swsCtx);
    void processFrames();
    bool openVideoDevice(AVFormatContext *&formatContext);
};
// FrameProcessingTask: 用于处理单个视频帧的任务
class FrameProcessingTask : public QObject, public QRunnable {
    Q_OBJECT
public:
    FrameProcessingTask(AVCodecContext *codecContext, AVFrame *frame, SwsContext *swsCtx);
    void run() override;
    FrameProcessingTask();


private:
    AVCodecContext *codecContext;
    AVFrame *frame;
    SwsContext *swsCtx;
};
#endif // VIDEOCAPTURETHREAD_H
