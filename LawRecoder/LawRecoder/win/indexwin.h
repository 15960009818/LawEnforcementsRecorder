#ifndef INDEXWIN_H
#define INDEXWIN_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include "../controller/indexcontroller.h"
#include "../common/singleton.h"
#include "../common/VideoCaptureThread.h"
#include "../common/VideoSaveThread.h"
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

public:
    explicit IndexWin(QWidget *parent = nullptr);
    ~IndexWin();

    // 绘制界面图像
    void paintEvent(QPaintEvent *event) override;

private slots:
    // 按钮点击槽函数
    void BtnClicked();
    // 接收图像信号的槽函数
    //void recviceImage(const QImage &img);
};


#endif // IndexWin_H
