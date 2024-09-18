#ifndef FDECODEPLAY_H
#define FDECODEPLAY_H

#include <QString>
#include <QThread>
#include <QDebug>
extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/avutil.h>
    #include <libswresample/swresample.h>
    #include <libswscale/swscale.h>
    #include <libavdevice/avdevice.h>
}

/**
 * @类名: fdecodeplay
 * @描述: 一个用于解码视频的类，继承自 QThread，提供基本的视频解码功能
 * @作者: 翔君
 * @创建日期: 2024/9/16
 * @是否完成: 完成
 */
class fdecodeplay : public QThread
{
    Q_OBJECT
private:
    /**
     * @变量: picname
     * @描述: 用于存储视频文件的路径
     */
   QString picname;
public:
    /**
     * @函数名: fdecodeplay
     * @描述: 构造函数，初始化 fdecodeplay 类
     * @作者: 翔君
     * @创建日期: 2024/9/16
     * @是否完成: 完成
     */
    fdecodeplay();

    /**
     * @函数名: ~fdecodeplay
     * @描述: 析构函数，销毁 fdecodeplay 类对象
     * @作者: 翔君
     * @创建日期: 2024/9/16
     * @是否完成: 完成
     */
    virtual ~fdecodeplay() {}

    /**
     * @函数名: registerFFmpeg
     * @描述: 注册 FFmpeg 库，确保所有编解码器和设备可用
     * @作者: 翔君
     * @创建日期: 2024/9/16
     * @是否完成: 完成
     */
    void registerFFmpeg();

    /**
     * @函数名: run
     * @描述: 重写线程的 run 函数，执行解码任务
     * @作者: 翔君
     * @创建日期: 2024/9/16
     * @是否完成: 完成
     */
    void run();

    /**
     * @函数名: setPicname
     * @描述: 设置要解码的视频文件路径
     * @参数: value 指定的视频文件路径
     * @作者: 翔君
     * @创建日期: 2024/9/16
     * @是否完成: 完成
     */
    void setPicname(const QString &value);



    void decodeVideoStream(const QString &filename);
    QString getPicname();
signals:
    /**
     * @信号: sendEndFlag
     * @描述: 解码结束信号
     * @参数: flag 解码结束标志
     */
    void sendEndFlag(int flag);
};

#endif // FDECODEPLAY_H
