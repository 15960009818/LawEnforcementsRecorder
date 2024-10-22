#ifndef IMAGECAPTURECONTROLLER_H
#define IMAGECAPTURECONTROLLER_H

#include <QObject>
#include <QThread>
#include <QDate>
#include <../dao/picturedao.h>
class ImageCaptureController:public QThread
{
    Q_OBJECT
public:

private:
    QList<PictureDao> getPictureMessage(QDate pictureDate);
};

#endif // IMAGECAPTURECONTROLLER_H
