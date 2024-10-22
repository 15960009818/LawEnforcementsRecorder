#include <opencv2/opencv.hpp>
#include <iostream>
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
extern "C" {
    #include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/avutil.h>
    #include <libswresample/swresample.h>
    #include <libswscale/swscale.h>
    #include <libavdevice/avdevice.h>
}
int main() {
    // 初始化 FFmpeg
    av_register_all();

    // 打开摄像头
    cv::VideoCapture cap(0);
    if (!cap.isOpened()) {
        std::cerr << "无法打开摄像头!" << std::endl;
        return -1;
    }

    // 设置输出文件
    const char* outputFileName = "output.h264";

    // 创建输出格式上下文
    AVFormatContext* outputContext = nullptr;
    avformat_alloc_output_context2(&outputContext, nullptr, "h264", outputFileName);
    if (!outputContext) {
        std::cerr << "无法创建输出上下文!" << std::endl;
        return -1;
    }

    // 创建视频流
    AVStream* videoStream = avformat_new_stream(outputContext, nullptr);
    if (!videoStream) {
        std::cerr << "无法创建视频流!" << std::endl;
        return -1;
    }

    // 设置编码器参数
    AVCodecContext* codecContext = videoStream->codec;
    codecContext->codec_id = AV_CODEC_ID_H264;
    codecContext->codec_type = AVMEDIA_TYPE_VIDEO;
    codecContext->bit_rate = 400000;
    codecContext->width = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_WIDTH));
    codecContext->height = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    codecContext->time_base = {1, 25}; // 每秒 25 帧
    codecContext->pix_fmt = AV_PIX_FMT_YUV420P;

    // 打开编码器
    AVCodec* codec = avcodec_find_encoder(codecContext->codec_id);
    if (!codec) {
        std::cerr << "无法找到编码器!" << std::endl;
        return -1;
    }
    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        std::cerr << "无法打开编码器!" << std::endl;
        return -1;
    }

    // 写文件头
    avformat_write_header(outputContext, nullptr);

    cv::Mat frame;
    while (true) {
        cap >> frame; // 从摄像头读取帧
        if (frame.empty()) {
            break; // 如果没有帧，退出循环
        }

        // 将 OpenCV 图像转换为 AVFrame
        AVFrame* avFrame = av_frame_alloc();
        avFrame->format = AV_PIX_FMT_BGR24;
        avFrame->width = frame.cols;
        avFrame->height = frame.rows;
        av_frame_get_buffer(avFrame, 0);

        // 拷贝数据
        for (int y = 0; y < frame.rows; y++) {
            memcpy(avFrame->data[0] + y * avFrame->linesize[0], frame.ptr(y), frame.step[0]);
        }

        // 编码并写入文件
        AVPacket pkt;
        av_init_packet(&pkt);
        pkt.data = nullptr; // packet data will be allocated by the encoder
        pkt.size = 0;

        int response = avcodec_send_frame(codecContext, avFrame);
        if (response < 0) {
            std::cerr << "错误: 无法发送帧到编码器!" << std::endl;
            break;
        }

        while (response >= 0) {
            response = avcodec_receive_packet(codecContext, &pkt);
            if (response == AVERROR(EAGAIN) || response == AVERROR_EOF) {
                break;
            } else if (response < 0) {
                std::cerr << "错误: 无法接收编码后的数据!" << std::endl;
                break;
            }

            av_interleaved_write_frame(outputContext, &pkt);
            av_packet_unref(&pkt);
        }

        av_frame_free(&avFrame);
    }

    // 写文件尾
    av_write_trailer(outputContext);

    // 释放资源
    avcodec_close(codecContext);
    avformat_free_context(outputContext);
    cap.release();

    std::cout << "视频已保存为 " << outputFileName << std::endl;

    return 0;
}
