#include "videoplayer.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSlider>
#include <QImage>
#include <QPixmap>
#include <QDebug>
#include <QCloseEvent>
#include <QMessageBox>
/**
 * @brief VideoPlayer 构造函数
 *
 * 构造函数用于初始化视频播放器界面，创建视频线程并设置视频播放控制界面元素。
 * 它还会设置播放按钮、进度条、播放速度下拉框、截图按钮等控件，并将它们布局到界面上。
 */
VideoPlayer::VideoPlayer(const QString &videoPath, QWidget *parent)
    : QWidget(parent), isPlaying(false)
{

    // 创建视频播放线程并传入视频路径
    videoThread = new VideoThread(videoPath, this);

    // 检查视频是否成功打开
    if (!videoThread->isOpened())
    {
        qDebug() << "Error opening video!";
        QMessageBox::critical(this, tr("Error"), tr("The video file is corrupted or cannot be opened."));
        return;
    }

    // 获取视频的总帧数
    totalFrames = videoThread->getTotalFrames();

    // 创建视频显示标签
    videoLabel = new QLabel(this);
    videoLabel->setFixedSize(640, 480);
    videoLabel->setAlignment(Qt::AlignCenter);

    // 创建播放按钮、进度条、速度选择框和截图按钮
    playButton = new QPushButton("Play", this);
    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, totalFrames - 1);

    // 创建速度选择下拉框并添加选项
    speedComboBox = new QComboBox(this);
    speedComboBox->addItem("1x");
    speedComboBox->addItem("2x");
    speedComboBox->addItem("0.5x");

    // 添加截图按钮
    //screenshotButton = new QPushButton("Screenshot", this);

    // 设置界面布局
    QVBoxLayout *mainLayout = new QVBoxLayout;
    QHBoxLayout *controlLayout = new QHBoxLayout;
    controlLayout->addWidget(playButton);
    controlLayout->addWidget(slider);
    controlLayout->addWidget(speedComboBox);
    //controlLayout->addWidget(screenshotButton);

    mainLayout->addWidget(videoLabel);
    mainLayout->addLayout(controlLayout);
    setLayout(mainLayout);

    // 绑定按钮和槽函数
    connect(playButton, &QPushButton::clicked, this, &VideoPlayer::playPause);
    connect(slider, &QSlider::sliderMoved, this, &VideoPlayer::sliderMoved);
    connect(videoThread, &VideoThread::dataSend2UI, this, &VideoPlayer::updateSlider);
    connect(videoThread, &VideoThread::finishedPlaying, this, &VideoPlayer::finishedPlaying);
    connect(speedComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &VideoPlayer::changeSpeed);

    connect(screenshotButton, &QPushButton::clicked, this, &VideoPlayer::onScreenshotButtonClicked);

    qDebug() << "VideoPlayer initialized successfully.";

    // 启动视频播放线程
    videoThread->start();
}

/**
 * @brief VideoPlayer 析构函数
 *
 * 析构函数会停止并销毁视频播放线程。
 */
VideoPlayer::~VideoPlayer()
{
    QCloseEvent event;
    closeEvent(&event);

    // 确保资源释放完成
    videoThread->stop();
    videoThread->wait();
    delete videoThread;
}


/**
 * @brief 播放/暂停按钮的槽函数
 *
 * 该函数根据当前播放状态切换播放按钮的文本，并控制视频线程的播放或暂停。
 */
void VideoPlayer::playPause()
{
    if (isPlaying)
    {
        playButton->setText("Play");  // 如果正在播放，设置按钮文本为 "Play"
        videoThread->pause();         // 暂停视频播放
    }
    else
    {
        playButton->setText("Pause"); // 如果暂停，设置按钮文本为 "Pause"
        videoThread->resume();        // 恢复视频播放
    }
    isPlaying = !isPlaying;  // 切换播放状态
}

/**
 * @brief 更新进度条
 *
 * 该函数会在接收到视频帧时更新进度条的值，并在界面上显示当前帧。
 *
 * @param frame 当前帧的帧号
 * @param frameData 当前帧的数据
 */
void VideoPlayer::updateSlider(int frame, const cv::Mat &frameData)
{
    qDebug() << "Received frame:" << frame;
    slider->setValue(frame);  // 更新进度条的值

    if (!frameData.empty())
    {
        // 将 BGR 格式的图像转换为 RGB 格式以便显示
        cv::Mat rgbFrame;
        cv::cvtColor(frameData, rgbFrame, cv::COLOR_BGR2RGB);

        // 创建 QImage 用于显示
        QImage qimg(rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step[0], QImage::Format_RGB888);
        videoLabel->setPixmap(QPixmap::fromImage(qimg));
    }
    else
    {
        qDebug() << "Empty frame received!";
    }
}

/**
 * @brief 进度条移动事件的槽函数
 *
 * 该函数在进度条移动时被调用，用于让视频播放线程跳转到指定帧。
 *
 * @param position 当前进度条的位置
 */
void VideoPlayer::sliderMoved(int position)
{
    videoThread->seekToFrame(position);
}

/**
 * @brief 播放结束后的槽函数
 *
 * 该函数在视频播放结束后被调用，重置播放按钮并将进度条设置为0。
 */
void VideoPlayer::finishedPlaying()
{
    isPlaying = false;
    playButton->setText("Play");
    slider->setValue(0);          // 重置进度条为0
}

/**
 * @brief 变更播放速度的槽函数
 *
 * 该函数根据选择的速度选项调整视频的播放速度。
 *
 * @param index 当前选择的速度索引
 */
void VideoPlayer::changeSpeed(int index)
{
    double speedFactor = 1.0;
    switch (index)
    {
        case 1: speedFactor = 2.0; break;  // 2x速度
        case 2: speedFactor = 0.5; break;  // 0.5x速度
        default: speedFactor = 1.0; break; // 默认1x速度
    }
    videoThread->setSpeedFactor(speedFactor);  // 设置视频线程的播放速度
}

/**
 * @brief 截图按钮点击事件的槽函数
 *
 * 该函数在点击截图按钮时触发，通知视频线程进行截图操作。
 */
void VideoPlayer::onScreenshotButtonClicked()
{
    videoThread->screenshotRequested = true;  // 截图请求
}
/**
 * @brief 窗口关闭事件
 *
 * 当窗口关闭时，停止视频播放并销毁视频线程。
 *
 * @param event 关闭事件
 */
void VideoPlayer::closeEvent(QCloseEvent *event)
{
    if (isPlaying)
    {
        videoThread->stop();  // 停止视频播放线程
        videoThread->wait();  // 等待线程完成
    }
    event->accept();  // 接受关闭事件
}
