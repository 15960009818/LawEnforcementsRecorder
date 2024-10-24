#ifndef IMAGECAPTURESERVICE_H
#define IMAGECAPTURESERVICE_H

#include <QObject>
#include <QDebug>
#include <QList>
#include "../dao/picturedao.h"

class ImageCaptureService : public QObject
{
    Q_OBJECT
public:
    explicit ImageCaptureService(QObject *parent = nullptr);

public slots:
    void GetDateImageSlots(const QDate &date, const int &Page);

signals:
    void imageListReady(const QList<PictureDao> &pictureList); // 图片列表准备好时发出的信号
    void errorOccurred(int errorCode); // 错误发生时发出的信号
};

#endif // IMAGECAPTURESERVICE_H
