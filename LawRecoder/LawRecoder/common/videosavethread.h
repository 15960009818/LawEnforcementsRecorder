// VideoSaveThread.h
#ifndef VIDEOSAVETHREAD_H
#define VIDEOSAVETHREAD_H

#include <QThread>
#include <QImage>
#include <opencv2/opencv.hpp>
#include "../service/deviceservice.h"
#include "../service/savevideoandpictureservice.h"
#include "../common/singleton.h"
class VideoSaveThread : public QThread {
    Q_OBJECT
     SINGLETON(VideoSaveThread)
public:

    void startSaving(const QString &filename, int width, int height, int fps);
    void stop();  // 添加停止方法

    void stopSaving();
    void insertVideoInfoToDatabase();
public slots:
    void saveFrame(const QImage &image);

protected:
    void run() override;

private:
    QString outputFile;
    int frameWidth, frameHeight, frameRate;
    cv::VideoWriter videoWriter;
    bool stopFlag;
    SaveVideoAndPictureService *savevideoandpictureservice;
};

#endif // VIDEOSAVETHREAD_H
