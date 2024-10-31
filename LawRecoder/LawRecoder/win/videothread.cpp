#include "videothread.h"
#include <QDebug>

VideoThread::VideoThread(char *path) : current(0), isRun(false), isStop(false), isRecord(false) {
    if (cap.open(path)) {
        this->count = cap.get(cv::CAP_PROP_FRAME_COUNT);
        this->FPS = static_cast<int>(round(cap.get(cv::CAP_PROP_FPS)));
        qDebug() << "Video opened successfully. Total frames:" << count << "FPS:" << FPS;
    } else {
        qDebug() << "Failed to open video at path:" << path;
    }
}

VideoThread::~VideoThread() {
    stop();    // 停止视频线程
    wait();    // 等待线程结束
}
void VideoThread::setVideoPath(const QString &path) {
    videoPath = path;  // 设置视频路径
}

void VideoThread::run() {
    while (!isStop) {
        while (isRun) {
            if (cap.read(frame)) {
                current++;
                emit dataSend2UI(current, frame);  // 发送当前帧到 UI
            } else {
                qDebug() << "Failed to read frame. Stopping video playback.";
                isRun = false;  // 停止播放
                break;
            }
            msleep(1000 / FPS);  // 控制帧率
        }
        msleep(10);  // 防止占用过多 CPU 资源
    }
}


void VideoThread::pause() {
    isRun = !isRun;  // 切换播放状态
    if (isRun) {
        qDebug() << "Video resumed.";
    } else {
        qDebug() << "Video paused.";
    }
}

void VideoThread::resume() {
    isRun = true;  // 开始播放
    qDebug() << "Video playback started.";
}

void VideoThread::stop() {
    if (!this->isFinished()) {
        isStop = true;
        qDebug() << "Stopping video thread...";
        msleep(10);
        this->terminate();  // 终止线程
    }
}

void VideoThread::seekToFrame(int frameNumber) {
    if (cap.isOpened()) {
        cap.set(cv::CAP_PROP_POS_FRAMES, frameNumber);  // 跳转到指定帧
        qDebug() << "Seek to frame:" << frameNumber;
    } else {
        qDebug() << "Cannot seek, video not opened.";
    }
}
int VideoThread::getFrameWidth() const {
    return cap.get(cv::CAP_PROP_FRAME_WIDTH);
}

int VideoThread::getFrameHeight() const {
    return cap.get(cv::CAP_PROP_FRAME_HEIGHT);
}

bool VideoThread::isOpened() const {
    return cap.isOpened();
}

int VideoThread::getCount() const {
    return count;
}
