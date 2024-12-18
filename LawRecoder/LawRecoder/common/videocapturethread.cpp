#include "videocapturethread.h"
#include <QDate>
#include <QDebug>
#include <QDir>
#include <QProcess>
#include <QImageReader>
#include <QThreadPool>
#include "../service/savevideoandpictureservice.h"
#include "../dao/picturedao.h"
#include "serialgps.h"
VideoCaptureThread::VideoCaptureThread() {
    qDebug() << "VideoCaptureThread initialized.";
}

VideoCaptureThread::~VideoCaptureThread() {
    stop();
    qDebug() << "VideoCaptureThread destroyed.";
}

void VideoCaptureThread::startCapturing() {
    if (!isRunning()) {
        stopFlag = false;
        qDebug() << "Starting capture thread...";
        start();
    } else {
        qDebug() << "Capture thread is already running.";
    }
}

void VideoCaptureThread::stop() {
    stopFlag = true;
    if (isRunning()) {
        qDebug() << "Stopping capture thread...";
        wait();
        qDebug() << "Capture thread stopped.";
    }
}

void VideoCaptureThread::run() {
//#ifdef __arm__
    captureWithFFmpeg();
//#else
    //captureWithOpenCV();
//#endif
    emit finished();
}

//#ifdef __arm__
void VideoCaptureThread::captureWithFFmpeg() {
    avdevice_register_all();

    // Initialize FFmpeg components
    AVFormatContext *formatContext = nullptr;
    AVCodecContext *codecContext = nullptr;
    AVPacket *packet = av_packet_alloc();
    AVFrame *frame = av_frame_alloc();
    SwsContext *swsCtx = nullptr;

    if (!packet || !frame) {
        cleanupFFmpeg(formatContext, codecContext, packet, frame, swsCtx);
        return;
    }

    const char *deviceName = "/dev/video1";
    AVDictionary *options = nullptr;
    av_dict_set(&options, "video_size", "1024x768", 0);  // 设置分辨率
    av_dict_set(&options, "framerate", "30", 0);         // 设置帧率为 30 FPS
    av_dict_set(&options, "pixel_format", "yuyv422", 0); // 设置像素格式为 YUYV422

    AVInputFormat *inputFormat = av_find_input_format("v4l2");
    if (!inputFormat || avformat_open_input(&formatContext, deviceName, inputFormat, &options) < 0) {
        cleanupFFmpeg(formatContext, codecContext, packet, frame, swsCtx);
        return;
    }

    if (avformat_find_stream_info(formatContext, nullptr) < 0) {
        cleanupFFmpeg(formatContext, codecContext, packet, frame, swsCtx);
        return;
    }

    int videoStreamIndex = findVideoStreamIndex(formatContext);
    if (videoStreamIndex == -1) {
        cleanupFFmpeg(formatContext, codecContext, packet, frame, swsCtx);
        return;
    }

    codecContext = initializeCodecContext(formatContext, videoStreamIndex);
    if (!codecContext) {
        cleanupFFmpeg(formatContext, codecContext, packet, frame, swsCtx);
        return;
    }

    swsCtx = sws_getContext(
        codecContext->width, codecContext->height, codecContext->pix_fmt,
        codecContext->width, codecContext->height, AV_PIX_FMT_RGB24,
        SWS_FAST_BILINEAR, nullptr, nullptr, nullptr);

    if (!swsCtx) {
        cleanupFFmpeg(formatContext, codecContext, packet, frame, swsCtx);
        return;
    }

    const int frameIntervalMs = 33; // Target frame interval: 30ms
    auto lastCaptureTime = QDateTime::currentMSecsSinceEpoch();

    while (!stopFlag) {
        auto currentTime = QDateTime::currentMSecsSinceEpoch();
        if (currentTime - lastCaptureTime < frameIntervalMs) {
            QThread::msleep(1);
            continue;
        }
        lastCaptureTime = currentTime;

        if (av_read_frame(formatContext, packet) < 0) {
            break;
        }

        if (packet->stream_index == videoStreamIndex) {
            if (avcodec_send_packet(codecContext, packet) < 0) {
                av_packet_unref(packet);
                continue;
            }

            while (avcodec_receive_frame(codecContext, frame) == 0) {
                QImage image(codecContext->width, codecContext->height, QImage::Format_RGB888);
                uint8_t *dstData[1] = { image.bits() };
                int dstLinesize[1] = { 3 * codecContext->width };

                int result = sws_scale(
                    swsCtx,
                    frame->data, frame->linesize,
                    0, codecContext->height,
                    dstData, dstLinesize);

                if (result > 0) {
                    QMutexLocker locker(&captureMutex);
                    frameBuffer.append(image);
                    captureCondition.wakeAll();
                    processFrame(codecContext, frame, swsCtx);
                }
            }
        }

        av_packet_unref(packet);
    }

    cleanupFFmpeg(formatContext, codecContext, packet, frame, swsCtx);
}



int VideoCaptureThread::findVideoStreamIndex(AVFormatContext *formatContext) {
    for (unsigned int i = 0; i < formatContext->nb_streams; i++) {
        if (formatContext->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_VIDEO) {
            return i;
        }
    }
    return -1;
}
void VideoCaptureThread::processFrame(AVCodecContext *codecContext, AVFrame *frame, SwsContext *swsCtx) {
    AVFrame *clonedFrame = av_frame_clone(frame);
    if (!clonedFrame) {
        qDebug() << "Failed to clone frame!";
        return;
    }

    FrameProcessingTask *task = new FrameProcessingTask(codecContext, clonedFrame, swsCtx);
    QThreadPool::globalInstance()->start(task);
}

AVCodecContext* VideoCaptureThread::initializeCodecContext(AVFormatContext *formatContext, int videoStreamIndex) {
    AVCodec *codec = avcodec_find_decoder(formatContext->streams[videoStreamIndex]->codecpar->codec_id);
    if (!codec) {
        qWarning() << "Failed to find codec.";
        return nullptr;
    }

    AVCodecContext *codecContext = avcodec_alloc_context3(codec);
    if (!codecContext || avcodec_parameters_to_context(codecContext, formatContext->streams[videoStreamIndex]->codecpar) < 0) {
        qWarning() << "Failed to copy codec parameters.";
        avcodec_free_context(&codecContext);
        return nullptr;
    }

    if (avcodec_open2(codecContext, codec, nullptr) < 0) {
        qWarning() << "Failed to open codec.";
        avcodec_free_context(&codecContext);
        return nullptr;
    }

    return codecContext;
}

void VideoCaptureThread::cleanupFFmpeg(AVFormatContext *formatContext, AVCodecContext *codecContext, AVPacket *packet, AVFrame *frame, SwsContext *swsCtx) {
    if (packet) av_packet_free(&packet);
    if (frame) av_frame_free(&frame);
    if (codecContext) avcodec_free_context(&codecContext);
    if (formatContext) avformat_close_input(&formatContext);
    if (swsCtx) sws_freeContext(swsCtx);
}
//#else

//void VideoCaptureThread::captureWithOpenCV() {
//    cv::VideoCapture cap(0);
//    if (!cap.isOpened()) {
//        qWarning() << "Failed to open camera.";
//        return;
//    }

//    cap.set(cv::CAP_PROP_FOURCC, cv::VideoWriter::fourcc('M', 'J', 'P', 'G'));
//    cap.set(cv::CAP_PROP_FPS, 30);
//    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);
//    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);

//    qDebug() << "Camera opened successfully.";

//    while (!stopFlag) {
//        cv::Mat frame;
//        cap >> frame;

//        if (frame.empty()) {
//            qWarning() << "Empty frame received.";
//            continue;
//        }

//        cv::Mat rgbFrame;
//        cv::cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB);
//        QImage img((uchar*)rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);
//        emit frameCaptured(img.copy());
//    }

//    cap.release();
//}

//#endif
void VideoCaptureThread::captureScreenshot() {
    cv::Mat frame;
    {
        QMutexLocker locker(&queueMutex);
        if (!frameQueue.isEmpty()) {
            frame = frameQueue.dequeue(); // 从队列中获取最旧的帧
        } else {
            qWarning("Frame queue is empty, unable to capture screenshot.");
            return;
        }
    }

    if (frame.empty()) {
        qWarning("Failed to capture screenshot - empty frame.");
        // 没有有效帧时写入默认的 NULL 数据
        PictureDao picture;
        picture.setPictureName("NULL");
        picture.setPictureAddress("NULL");
        picture.setPictureDate("NULL");
        picture.setPictureUser("NULL");
        picture.setPictureType(0);
        picture.setPicturePath("NULL");

        SaveVideoAndPictureService service;
        service.insertPictureInfo(picture);
        return;
    }

    // 使用 MatToQImage 进行图像转换
    QImage img = MatToQImage(frame);

    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");
    QString picturePath = Singleton<DeviceService>::getInstance().getPicturePath();

    if (picturePath.isEmpty()) {
        picturePath = QDir::homePath() + "/Pictures";
        QDir().mkpath(picturePath);
    }

    QString outfile = QDir::cleanPath(picturePath + "/" + timestamp + ".jpg");

    // 初始化 SerialGPS 对象并读取地理位置信息
    SerialGPS gpsReader;
    QString gpsData = "NULL";
    if (gpsReader.open()) {
        gpsData = gpsReader.getLastGPSData();
        gpsReader.close();
    } else {
        qWarning() << "Failed to initialize GPS reader, setting address to NULL.";
    }

    if (img.save(outfile, "JPEG")) {
        qDebug() << "Screenshot saved at:" << outfile;

        PictureDao picture;
        picture.setPictureName(timestamp);
        picture.setPictureAddress(gpsData); // 写入 GPS 地理位置信息
        picture.setPictureDate(QDateTime::currentDateTime().toString("yyyy-MM-dd"));
        picture.setPictureUser("User");
        picture.setPictureType(1);
        picture.setPicturePath(outfile);

        SaveVideoAndPictureService service;
        service.insertPictureInfo(picture);
    } else {
        qDebug() << "Failed to save screenshot.";
    }
}

QImage VideoCaptureThread::MatToQImage(const cv::Mat &mat) {
    switch (mat.type()) {
    case CV_8UC1: {
        QImage image(mat.cols, mat.rows, QImage::Format_Indexed8);
        image.setColorCount(256);
        for(int i = 0; i < 256; i++) {
            image.setColor(i, qRgb(i, i, i));
        }

        uchar *pSrc = mat.data;
        for(int row = 0; row < mat.rows; row++) {
            uchar *pDest = image.scanLine(row);
            memcpy(pDest, pSrc, mat.cols);
            pSrc += mat.step;
        }
        return image;
    }
    case CV_8UC3: {
        const uchar *pSrc = (const uchar*)mat.data;
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return image.rgbSwapped();  // OpenCV 的 BGR 转为 RGB
    }
    case CV_8UC4: {
        const uchar *pSrc = (const uchar*)mat.data;
        QImage image(pSrc, mat.cols, mat.rows, mat.step, QImage::Format_ARGB32);
        return image.copy();
    }
    default:
        qWarning() << "Unsupported image format for Mat to QImage conversion.";
        break;
    }
    return QImage();
}

FrameProcessingTask::FrameProcessingTask(AVCodecContext *codecContext, AVFrame *frame, SwsContext *swsCtx)
: codecContext(codecContext), frame(frame), swsCtx(swsCtx)
{

}

void FrameProcessingTask::run() {
    //qDebug()<<"FrameProcessingTask start";
    if (!codecContext || !frame || !swsCtx) {
        qDebug() << "Invalid parameters for FrameProcessingTask.";
        return;
    }

    if (codecContext->width <= 0 || codecContext->height <= 0) {
        qDebug() << "Invalid codec context dimensions.";
        return;
    }

    QImage img(codecContext->width, codecContext->height, QImage::Format_RGB888);
    if (img.isNull()) {
        qDebug() << "Failed to allocate QImage.";
        return;
    }

    uint8_t *dest[4] = { img.bits(), nullptr, nullptr, nullptr };
    int destLinesize[4] = { static_cast<int>(img.bytesPerLine()), 0, 0, 0 };

    if (!frame->data[0] || frame->linesize[0] <= 0) {
        qDebug() << "Invalid frame data or linesize.";
        return;
    }

    if (sws_scale(swsCtx, frame->data, frame->linesize, 0, codecContext->height, dest, destLinesize) <= 0) {
        qDebug() << "sws_scale failed.";
        return;
    }
    qDebug() << "Preparing to send frame of size:" << img.size();
    emit Singleton<VideoCaptureThread>::getInstance().frameCaptured(img.copy());
    //qDebug()<<"send frame success";
    // 如果 frame 是独立分配的，才释放它
    av_frame_free(&frame);
}



FrameProcessingTask::FrameProcessingTask()
{

}
