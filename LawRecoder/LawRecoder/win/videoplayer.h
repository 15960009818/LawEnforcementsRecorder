#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QWidget>
#include <QPushButton>
#include <QSlider>
#include <QLabel>
#include <QTimer>
#include <opencv2/videoio.hpp>
#include <opencv2/core/mat.hpp>

#include "videothread.h"
#include "videocapturewin.h"
class VideoPlayer : public QWidget {
    Q_OBJECT

public:
    explicit VideoPlayer(VideoCaptureWin *videoCaptureWin, QWidget *parent = nullptr);
    ~VideoPlayer();

protected slots:
    void goBack();
private slots:
    void playVideo(const QString &videoPath);
    void playPause();           // 控制播放/暂停
    void updateSlider(int frame, const cv::Mat &frameData);  // 更新进度条和视频显示
    void sliderMoved(int position);  // 拖动进度条更新视频位置


private:
    VideoThread *videoThread;   // 视频处理线程
    QLabel *videoLabel;         // 显示视频的 QLabel
    QPushButton *playButton;    // 播放/暂停按钮
    QSlider *slider;            // 进度条
    bool isPlaying;             // 当前是否正在播放
    int totalFrames;            // 视频的总帧数

};

#endif // VIDEOPLAYER_H
