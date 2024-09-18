#ifndef RECORDINGUNIT_H
#define RECORDINGUNIT_H

#include <QObject>
#include <QTimer>
#include <QThread>
#include <QImage>
extern "C" {
#include <libavcodec/avcodec.h>
    #include <libavformat/avformat.h>
    #include <libavutil/avutil.h>
    #include <libswresample/swresample.h>
    #include <libswscale/swscale.h>
    #include <libavdevice/avdevice.h>
}
class RecordingUnit
{
public:
    RecordingUnit();
    virtual ~RecordingUnit() {}
    //
    void run();//重写线程中的run 执行线程要执行的任务


signals://信号：

private:
    void registerFFmpeg();//注册组件
    AVFormatContext *getCameraCapture(QString camera);
    QImage transCameraCaptureToIMG(AVCodecContext *codec, AVFrame *picture, SwsContext *sws_contentRGB, uint8_t *bufferRGB);
    AVFrame *transCameraCaptureToYUV(AVCodecContext *codec, AVFrame *picture, SwsContext *sws_content, uint8_t *buffer);
    void sendImg(QImage img);
    void sendYUV(AVFrame *pictureYUV);

};

#endif // RECORDINGUNIT_H
