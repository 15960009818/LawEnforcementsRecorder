#include "fdecodeplay.h"
#include <QString>
#include <QDir>

/**
 * @函数名: fdecodeplay
 * @描述: 构造函数，用于初始化 fdecodeplay 类的实例
 * @作者: 翔君
 * @创建日期: 2024/9/16
 * @是否完成: 完成
 */
fdecodeplay::fdecodeplay()
{
    // 初始化，删除数据库相关内容
}

/**
 * @函数名: registerFFmpeg
 * @描述: 注册 FFmpeg 库以确保所有编解码器和设备可用
 * @作者: 翔君
 * @创建日期: 2024/9/16
 * @是否完成: 完成
 */
void fdecodeplay::registerFFmpeg()
{
    av_register_all();
    avdevice_register_all();
}

/**
 * @函数名: decodeVideoStream
 * @描述: 打开并解码指定路径的视频文件，解码视频帧不作其他处理
 * @参数: filename 视频文件的路径
 * @作者: 翔君
 * @创建日期: 2024/9/16
 * @是否完成: 完成
 */
void fdecodeplay::decodeVideoStream(const QString &filename)
{
    AVFormatContext *formatContext = avformat_alloc_context();
    if (!formatContext) {
        qDebug() << "Failed to allocate format context";
        return;
    }

    int result = avformat_open_input(&formatContext, filename.toStdString().c_str(), nullptr, nullptr);
    if (result != 0) {
        qDebug() << "Failed to open video file:" << filename;
        avformat_free_context(formatContext);
        return;
    }

    result = avformat_find_stream_info(formatContext, nullptr);
    if (result < 0) {
        qDebug() << "Failed to find stream info";
        avformat_close_input(&formatContext);
        return;
    }

    int videoStreamIndex = -1;
    for (int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codec->codec_type == AVMEDIA_TYPE_VIDEO) {
            videoStreamIndex = i;
            break;
        }
    }

    if (videoStreamIndex == -1) {
        qDebug() << "No video stream found";
        avformat_close_input(&formatContext);
        return;
    }

    AVCodecContext *codecContext = formatContext->streams[videoStreamIndex]->codec;
    AVCodec *decoder = avcodec_find_decoder(codecContext->codec_id);
    if (!decoder) {
        qDebug() << "No decoder found for codec";
        avformat_close_input(&formatContext);
        return;
    }

    result = avcodec_open2(codecContext, decoder, nullptr);
    if (result != 0) {
        qDebug() << "Failed to open codec";
        avformat_close_input(&formatContext);
        return;
    }

    AVFrame *frame = av_frame_alloc();
    if (!frame) {
        qDebug() << "Failed to allocate frame";
        avcodec_close(codecContext);
        avformat_close_input(&formatContext);
        return;
    }

    AVPacket packet;
    av_init_packet(&packet);
    while (av_read_frame(formatContext, &packet) >= 0) {
        if (packet.stream_index == videoStreamIndex) {
            int frameFinished;
            avcodec_decode_video2(codecContext, frame, &frameFinished, &packet);
            if (frameFinished) {
                // 解码成功，可以在此处对解码后的帧进行处理
                qDebug() << "Decoded frame of size:" << codecContext->width << "x" << codecContext->height;
            }
        }
        av_packet_unref(&packet);
    }

    qDebug() << "Video stream decoding completed";

    av_frame_free(&frame);
    avcodec_close(codecContext);
    avformat_close_input(&formatContext);
}

/**
 * @函数名: run
 * @描述: 重写的线程运行函数，负责调用解码功能
 * @作者: 翔君
 * @创建日期: 2024/9/16
 * @是否完成: 完成
 */
void fdecodeplay::run()
{
    this->registerFFmpeg();
    QString filename = getPicname();  // 使用 picname 作为视频路径
    qDebug() << "picname is" << filename;
    this->decodeVideoStream(filename);
}

/**
 * @函数名: setPicname
 * @描述: 设置视频路径的函数
 * @参数: value 传入的路径值
 * @作者: 翔君
 * @创建日期: 2024/9/16
 * @是否完成: 完成
 */
void fdecodeplay::setPicname(const QString &value)
{
    picname = value;
}

QString fdecodeplay::getPicname()
{
    return picname;
}
