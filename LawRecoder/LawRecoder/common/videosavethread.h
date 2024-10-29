// VideoSaveThread.h
#ifndef VIDEOSAVETHREAD_H
#define VIDEOSAVETHREAD_H

#include <QThread>
#include <QImage>
#include <opencv2/opencv.hpp>

class VideoSaveThread : public QThread {
    Q_OBJECT

public:
    VideoSaveThread(QObject *parent = nullptr);
    ~VideoSaveThread();
    void startSaving(const QString &filename, int width, int height, int fps);
    void stop();  // 添加停止方法

    void stopSaving();
public slots:
    void saveFrame(const QImage &image);

protected:
    void run() override;

private:
    QString outputFile;
    int frameWidth, frameHeight, frameRate;
    cv::VideoWriter videoWriter;
    bool stopFlag;
};

#endif // VIDEOSAVETHREAD_H
