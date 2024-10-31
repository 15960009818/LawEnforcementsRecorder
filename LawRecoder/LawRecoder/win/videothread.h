#ifndef VIDEOTHREAD_H
#define VIDEOTHREAD_H

#include <QThread>
#include <opencv2/opencv.hpp>

class VideoThread : public QThread {
    Q_OBJECT

public:
    VideoThread(char *path);
    VideoThread() {}
    ~VideoThread();
    void setVideoPath(const QString &path);
    void pause();          // 暂停视频
    void resume();         // 恢复视频播放
    void stop();           // 停止视频播放
    void seekToFrame(int frameNumber);  // 跳转到指定帧
    bool isOpened() const; // 检查视频是否打开
    int getCount() const;  // 获取总帧数

    int getFrameWidth() const;
    int getFrameHeight() const;
signals:
    void dataSend2UI(int frame, const cv::Mat &frameData);  // 发送帧数据到 UI

protected:
    void run() override;   // 重载 run() 函数，线程执行体

private:
    cv::VideoCapture cap;  // OpenCV 视频捕获对象
    cv::Mat frame;         // 当前视频帧
    int current;           // 当前帧数
    int count;             // 视频总帧数
    int FPS;               // 视频帧率
    bool isRun;            // 是否正在播放
    bool isStop;           // 是否停止播放
    bool isRecord;         // 是否录制
     QString videoPath;
};

#endif // VIDEOTHREAD_H
