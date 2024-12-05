#ifndef VIDEOPLAYER_H
#define VIDEOPLAYER_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QComboBox>
#include <QTimer>
#include "videothread.h"

class VideoPlayer : public QWidget {
    Q_OBJECT

public:
    explicit VideoPlayer(const QString &videoPath, QWidget *parent = nullptr);
    ~VideoPlayer();

    void onScreenshotButtonClicked();
public slots:
    void playPause();
    void updateSlider(int frame, const cv::Mat &frameData);
    void sliderMoved(int position);
    void finishedPlaying();
    void changeSpeed(int index);

private:
    QLabel *videoLabel;
    QPushButton *playButton;
    QSlider *slider;
    VideoThread *videoThread;
    bool isPlaying;
    QComboBox *speedComboBox;
    int totalFrames;
    QPushButton * screenshotButton;
};

#endif // VIDEOPLAYER_H
