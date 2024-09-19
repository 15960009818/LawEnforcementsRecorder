#ifndef RECORDTHREAD_H
#define RECORDTHREAD_H

#include <QThread>
#include <QImage>
#include <QDebug>
#include <QMutex>
#include <QWaitCondition>
#include "recordingunit.h"
extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/avutil.h>
    #include <libswresample/swresample.h>
    #include <libswscale/swscale.h>
    #include <libavdevice/avdevice.h>
}

// 定义一个枚举类型，用于选择处理的类型（图像、YUV 或两者）
enum ProcessType { NONE, IMAGE, YUV, BOTH };

class RecordThread : public QThread
{
    Q_OBJECT

public:
    explicit RecordThread(QObject *parent = nullptr);

    // 设置线程的参数，包括处理类型
    void setParams(AVCodecContext *codec, AVFrame *picture, SwsContext *sws_content, uint8_t *buffer, ProcessType type);

protected:
    void run() override;

signals:
    // 图像和 YUV 数据准备完成的信号
    void imgReady(const QImage &img);
    void YUVReady(AVFrame *YUV);

private:
    // 处理函数：处理图像和 YUV
    void processImg();
    void processYUV();

    RecordingUnit *recordingUnit;  // 录制单元
    AVCodecContext *codec;         // 解码器上下文
    AVFrame *picture;              // 原始帧数据
    SwsContext *sws_content;       // 色彩空间转换上下文
    uint8_t *buffer;               // 数据缓冲区
    ProcessType processType;       // 当前的处理类型

    QMutex mutex;                  // 互斥锁，保护共享数据
    QWaitCondition condition;      // 用于线程的条件变量
    bool started = false;          // 标识线程是否启动
};

#endif // RECORDTHREAD_H
