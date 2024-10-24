#ifndef IMAGECAPTURECONTROLLER_H
#define IMAGECAPTURECONTROLLER_H

#include <QObject>
#include <QThread>
#include <QDate>
#include <QDebug>

#include "../common/singleton.h"
#include "../dao/picturedao.h"
#include "../service/imagecaptureservice.h"
class ImageCaptureController:public QObject
{
    Q_OBJECT
    SINGLETON(ImageCaptureController)
signals:
     void finishedPictureQuerySignal(int status, const QList<PictureDao>& pictures);
    void finishedImageCaptureSignal(int statue);
    void dateSignal(QDate date,int row);
public:
    void initController();
private slots:
    void getPictureMessage(QDate pictureDate,int page);
private:
    
    QDate getCurrentDate();
    QThread *threadGetPictureMessage;
    ImageCaptureService *imageCaptureService;

};

#endif // IMAGECAPTURECONTROLLER_H
