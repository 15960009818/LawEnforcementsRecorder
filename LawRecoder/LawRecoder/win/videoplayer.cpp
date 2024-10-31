#include "videoplayer.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QDebug>
#include <QImage>
#include <QPixmap>

VideoPlayer::VideoPlayer(VideoCaptureWin *videoCaptureWin, QWidget *parent)
    : QWidget(parent), isPlaying(false) {

    // 连接信号
    connect(videoCaptureWin, &VideoCaptureWin::videoSelected, this, &VideoPlayer::playVideo);

    // 初始化 VideoThread 线程
    videoThread = new VideoThread();

    // 初始化组件
    videoLabel = new QLabel(this);
    videoLabel->setFixedSize(380, 400);
    videoLabel->setAlignment(Qt::AlignCenter);

    playButton = new QPushButton("Play", this);
    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, totalFrames - 1);

    // 布局
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addWidget(playButton);
    controlLayout->addWidget(slider);

    mainLayout->addWidget(videoLabel);
    mainLayout->addLayout(controlLayout);
    setLayout(mainLayout);

    // 信号与槽连接
    connect(playButton, &QPushButton::clicked, this, &VideoPlayer::playPause);
    connect(slider, &QSlider::sliderMoved, this, &VideoPlayer::sliderMoved);
    connect(videoThread, &VideoThread::dataSend2UI, this, &VideoPlayer::updateSlider);

    qDebug() << "VideoPlayer initialized successfully.";
}

VideoPlayer::~VideoPlayer() {
    qDebug() << "Stopping video thread...";
    videoThread->stop();  // 停止视频线程
    videoThread->wait();  // 确保线程结束
    delete videoThread;
    qDebug() << "Video thread stopped and deleted.";
}

void VideoPlayer::playVideo(const QString &videoPath) {
    qDebug() << "Playing video from path:" << videoPath;

    videoThread->setVideoPath(videoPath);  // 设置视频路径
    videoThread->start();  // 启动视频线程

    // 获取视频总帧数
    totalFrames = videoThread->getCount();
    slider->setRange(0, totalFrames - 1);
}

void VideoPlayer::playPause() {
    if (isPlaying) {
        playButton->setText("Play");
        qDebug() << "Pausing video...";
        videoThread->pause();  // 暂停线程
    } else {
        playButton->setText("Pause");
        qDebug() << "Resuming video...";
        videoThread->resume();  // 恢复线程
    }
    isPlaying = !isPlaying;
}

void VideoPlayer::updateSlider(int frame, const cv::Mat &frameData) {
    // 更新进度条
    slider->setValue(frame);
    qDebug() << "Updating slider to frame:" << frame;

    // 更新视频显示
    if (!frameData.empty()) {
        cv::Mat rgbFrame;
        cv::cvtColor(frameData, rgbFrame, cv::COLOR_BGR2RGB);  // 转换颜色格式
        QImage qimg(rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step[0], QImage::Format_RGB888);  // 确保 QImage 格式正确
        videoLabel->setPixmap(QPixmap::fromImage(qimg));  // 显示图像
        qDebug() << "Frame displayed on UI.";
    } else {
        qDebug() << "Empty frame received!";
    }
}

void VideoPlayer::sliderMoved(int position) {
    qDebug() << "Slider moved to position:" << position;
    videoThread->seekToFrame(position);  // 定位到指定帧
}
