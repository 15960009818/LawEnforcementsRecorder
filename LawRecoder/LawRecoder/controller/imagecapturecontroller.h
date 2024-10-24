#ifndef IMAGECAPTURECONTROLLER_H
#define IMAGECAPTURECONTROLLER_H

#include <QObject>
#include <QThread>
#include <QDate>
#include <QDebug>
#include <QString>
#include "../common/singleton.h"
#include "../dao/picturedao.h"
#include "../service/imagecaptureservice.h"
class ImageCaptureController:public QObject
{
    Q_OBJECT
    SINGLETON(ImageCaptureController)
signals:
     void finishedPictureQuerySignal(int status, const QList<PictureDao>& pictures);
    void finishPictureSignal(const QString &message, const QList<PictureDao> &pictureList);
    void finishedImageCaptureSignal(int statue);
    void dateSignal(QDate date,int row);
public:
    void initController();
public slots:
    void sendPictureMessageSlot(int SQLRESULT, const QList<PictureDao> &pictureMessage);
private slots:
    void getPictureMessageSlot(QDate pictureDate,int page);
private:
    
    QDate getCurrentDate();
    QThread *threadGetPictureMessage;
    ImageCaptureService *imageCaptureService;

};

#endif // IMAGECAPTURECONTROLLER_H
