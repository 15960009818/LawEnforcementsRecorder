#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QThread>
#include <QMutex>
#include <QWaitCondition>
#include <QTimer>
#include <opencv2/opencv.hpp>

class VideoThread : public QThread {
    Q_OBJECT

public:
    explicit VideoThread(const QString &videoPath, QObject *parent = nullptr);
    ~VideoThread();

    bool isOpened() const;
    void stop();
    void pause();
    void resume();
    void seekToFrame(int frame);
    void setSpeedFactor(double factor);
    int getTotalFrames() const { return totalFrames; }
    void captureScreenshot();
     bool screenshotRequested = false; // 截图请求标志
protected:
    void run() override;

signals:
    void dataSend2UI(int frame, const cv::Mat &frameData);
    void finishedPlaying();

private:
    cv::VideoCapture cap;
    int totalFrames;
    double fps;
    int currentFrame;
    bool playing;
    bool paused;
    bool stopped;
    double speedFactor;
    QMutex mutex;
    QWaitCondition waitCondition;
    QTimer *timer;
     //void captureScreenshot(const cv::Mat &frame);

};

#endif // VIDEOTHREAD_H
