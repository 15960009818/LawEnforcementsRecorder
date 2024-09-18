#ifndef RECORDINGUNIT_H
#define RECORDINGUNIT_H

#include <QObject>
#include <QTimer>
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
    AVFormatContext *getCameraCapture(QString camera);
    QImage transCameraCaptureToIMG(AVCodecContext *codec, AVFrame *picture, SwsContext *sws_contentRGB, uint8_t *bufferRGB);
    AVFrame *transCameraCaptureToYUV(AVCodecContext *codec, AVFrame *picture, SwsContext *sws_content, uint8_t *buffer);



signals://信号：

private:
    void registerFFmpeg();//注册组件


};

#endif // RECORDINGUNIT_H
