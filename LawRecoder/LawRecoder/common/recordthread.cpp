#include "recordthread.h"

/**
 * @类名: RecordThread
 * @描述: 负责处理图像数据的线程类，继承自 QThread。
 *        该类通过调用 RecordingUnit 工具类的函数来处理摄像头获取的图像，并将处理后的图像发送给主线程。
 * @作者: 翔君
 * @创建日期: 2024/9/16
 * @是否完成: 完成
 */
RecordThread::RecordThread(QObject *parent)
    : QThread(parent), recordingUnit(new RecordingUnit)
{
}

/**
 * @函数名: setParams
 * @描述: 设置线程的参数，包括解码器上下文、帧数据、色彩空间转换上下文、缓冲区和处理类型（图像或 YUV）。
 * @参数: AVCodecContext *codec - 解码器上下文
 *        AVFrame *picture - 原始帧数据
 *        SwsContext *sws_content - 色彩空间转换上下文
 *        uint8_t *buffer - 数据缓冲区
 *        bool isImg - 是否处理图像（true）还是 YUV 数据（false）
 * @返回值: 无
 * @作者: 翔君
 * @创建日期: 2024/9/18
 * @是否完成: 完成
 */
void RecordThread::setParams(AVCodecContext* codec, AVFrame* picture, SwsContext* sws_content, uint8_t* buffer, bool isImg)
{
    QMutexLocker locker(&mutex);
    this->codec = codec;
    this->picture = picture;
    this->sws_content = sws_content;
    this->buffer = buffer;
    this->isImage = isImg;
    started = true;
    condition.wakeOne();
}

/**
 * @函数名: run
 * @描述: 线程执行的入口函数，根据参数选择处理图像或 YUV 数据。
 * @参数: 无
 * @返回值: 无
 * @作者: 翔君
 * @创建日期: 2024/9/18
 * @是否完成: 完成
 */
void RecordThread::run()
{
    QMutexLocker locker(&mutex);
    while (!started)
        condition.wait(&mutex);

    if (isImage)
        processImg();
    else
        processYUV();
}

/**
 * @函数名: processImg
 * @描述: 处理图像，将摄像头捕获的帧转换为 QImage 并通过信号发送。
 * @参数: 无
 * @返回值: 无
 * @作者: 翔君
 * @创建日期: 2024/9/18
 * @是否完成: 完成
 */
void RecordThread::processImg()
{
    QImage img = recordingUnit->transCameraCaptureToIMG(codec, picture, sws_content, buffer);
    emit imgReady(img);
}

/**
 * @函数名: processYUV
 * @描述: 处理图像，将摄像头捕获的帧转换为 YUV 格式的 AVFrame 并通过信号发送。
 * @参数: 无
 * @返回值: 无
 * @作者: 翔君
 * @创建日期: 2024/9/18
 * @是否完成: 完成
 */
void RecordThread::processYUV()
{
    AVFrame *YUV = recordingUnit->transCameraCaptureToYUV(codec, picture, sws_content, buffer);
    emit YUVReady(YUV);
}
