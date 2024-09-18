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
class RecordThread : public QThread
{
    Q_OBJECT

public:
    explicit RecordThread(QObject *parent = nullptr);
    void setParams(AVCodecContext* codec, AVFrame* picture, SwsContext* sws_content, uint8_t* buffer, bool isImg);

protected:
    void run() override;

signals:
    void imgReady(const QImage &img);
    void YUVReady(AVFrame *YUV);

private:
    RecordingUnit *recordingUnit;
    AVCodecContext *codec;
    AVFrame *picture;
    SwsContext *sws_content;
    uint8_t *buffer;
    bool isImage;

    QMutex mutex;
    QWaitCondition condition;
    bool started = false;

    void processImg();
    void processYUV();
};

#endif // RECORDTHREAD_H
