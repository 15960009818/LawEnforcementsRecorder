#ifndef INDEXWIN_H
#define INDEXWIN_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include <QPointer>
#include "../controller/indexcontroller.h"
#include "../common/singleton.h"
#include "../common/videocapturethread.h"
#include "../common/videosavethread.h"
#include "loginwin.h"
#include "settingwin.h"
#include "videocapturewin.h"
#include "imagecapturewin.h"
#include "common/capture/gstreamercap.h"
// 前向声明
class CameraCapture;

class IndexWin : public QWidget
{
    Q_OBJECT

private:
    // 设置 UI 界面
    void setUi();
    // 连接信号和槽
    void connectSignals();
    void startCapturing();
    void stopCapturing();
    void updateFrame(const QImage &image);
    void setupSaveTimer();
    void restartSaveThread();
    QLabel *videoLabel;
    QPushButton *startButton;
    QPushButton *stopButton;

    VideoCaptureThread *captureThread;
    VideoSaveThread *saveThread;
    QTimer *saveTimer;

    QLabel *LabTitle;          // 标题标签
    QPushButton *BtnVideo;      // 视频按钮
    QPushButton *BtnTakPic;     // 现场拍照按钮
    QPushButton *BtnBakVid;     // 录像回放按钮
    QPushButton *BtnSeePic;     // 现场图片按钮
    QPushButton *BtnSeeLog;     // 查看日志按钮
    QPushButton *BtnTrsVid;     // 录像转码按钮
    QPushButton *BtnSettings;   // 系统设置按钮
    QPushButton *BtnLogin;      // 登录按钮
    QPushButton *BtnReturn;     // 退出按钮
    QPushButton *BtnRgs;        // 注册按钮
    QGridLayout *glay;          // 网格布局
    QImage img;                 // 接收到的摄像头图像
    CameraCapture *cameraCapture; // 摄像头捕获类实例
    // 使用智能指针替代裸指针，避免内存泄漏
    QPointer<ImageCaptureWin> image;  // 图像查看窗口（使用 QPointer）
    QPointer<SettingWin> set;         // 设置窗口（使用 QPointer）
    QPointer<LoginWin> login;         // 登录窗口（使用 QPointer）
    QPointer<VideoCaptureWin> video;  // 视频回放窗口（使用 QPointer）
    void captureScreenshot();
     static IndexWin *instance;
     explicit IndexWin(QWidget *parent = nullptr);
     //FrameProcessingTask *frameProcessingTask;
     ~IndexWin();
     bool isArmDevice();
     GStreamerCap *gstCap;
public:


    static IndexWin *getInstance(QWidget *parent = nullptr);
    // 绘制界面图像
    void paintEvent(QPaintEvent *event) override;

private slots:
    // 按钮点击槽函数
    void BtnClicked();

};


#endif // IndexWin_H
