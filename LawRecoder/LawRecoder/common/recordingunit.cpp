#include "recordingunit.h"
#include <QDebug>
RecordingUnit::RecordingUnit()
{
    registerFFmpeg();
}

void RecordingUnit::registerFFmpeg()
{
    av_register_all();
    avdevice_register_all();//注册设备
}

/**
 * @brief RecordingUnit::getCameraCapture 获取摄像头信息
 */
AVFormatContext* RecordingUnit::getCameraCapture(QString camera)
{
    // 分配AVFormatContext，用于存放打开设备的信息
    AVFormatContext *forcontent = avformat_alloc_context();
    if (!forcontent) {
        qDebug() << "AVFormatContext 分配失败";
        return nullptr;
    }

    // DirectShow格式，用于访问摄像头
    AVInputFormat *fmt = av_find_input_format("dshow");
    QString b = "video=" + camera;
    QByteArray ba = b.toLatin1();
    char *ch = ba.data();

    // 打开设备
    int res = avformat_open_input(&forcontent, ch, fmt, nullptr);
    if (res != 0) {
        qDebug() << "无法打开摄像头";
        avformat_free_context(forcontent);
        return nullptr;
    }

    // 查找视频流信息
    res = avformat_find_stream_info(forcontent, nullptr);
    if (res < 0) {
        qDebug() << "没有流媒体信息";
        avformat_close_input(&forcontent);
        return nullptr;
    }

    return forcontent;
}

// 将获取到的图像信息转化为 IMG 格式并返回 QImage
QImage RecordingUnit::transCameraCaptureToIMG(AVCodecContext* codec, AVFrame* picture, SwsContext* sws_contentRGB, uint8_t* bufferRGB)
{
    // 处理图像转为 RGB 格式
    AVFrame* pictureRGB = av_frame_alloc();
    pictureRGB->width = codec->width;
    pictureRGB->height = codec->height;
    pictureRGB->format = AV_PIX_FMT_RGB32;

    avpicture_fill((AVPicture*)pictureRGB, bufferRGB, AV_PIX_FMT_RGB32, codec->width, codec->height);

    sws_scale(sws_contentRGB, picture->data, picture->linesize, 0, picture->height, pictureRGB->data, pictureRGB->linesize);

    QImage img((uchar*)bufferRGB, codec->width, codec->height, QImage::Format_RGB32);

    av_frame_free(&pictureRGB);
    return img;
}

// 将获取到的图像信息转化为 YUV 格式并返回 AVFrame*
AVFrame* RecordingUnit::transCameraCaptureToYUV(AVCodecContext* codec, AVFrame* picture, SwsContext* sws_content, uint8_t* buffer)
{
    // 处理图像转为 YUV 格式
    AVFrame* pictureYUV = av_frame_alloc();
    pictureYUV->width = codec->width;
    pictureYUV->height = codec->height;
    pictureYUV->format = AV_PIX_FMT_YUV420P;

    avpicture_fill((AVPicture*)pictureYUV, buffer, AV_PIX_FMT_YUV420P, codec->width, codec->height);

    sws_scale(sws_content, picture->data, picture->linesize, 0, picture->height, pictureYUV->data, pictureYUV->linesize);

    return pictureYUV;
}

