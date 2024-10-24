#include "imagecapturecontroller.h"
#include "../common/commondefs.h"
ImageCaptureController::ImageCaptureController()
{
    initController();
}

ImageCaptureController::~ImageCaptureController()
{

}

void ImageCaptureController::initController()
{
    connect(this,SIGNAL(dateSignal(QDate,int )),this,SLOT(getPictureMessage(QDate,int)));

}


void ImageCaptureController::getPictureMessage(QDate pictureDate,int row)
{
    qDebug() << "[DEBUG] getPictureMessage called.";
    qDebug() << "[DEBUG] PictureDate:" << pictureDate;

    // 首先判断日期是否有效
    if (!pictureDate.isValid()) {
        qDebug() << "[ERROR] 输入的日期无效";
        emit Singleton<ImageCaptureController>::getInstance().finishedPictureQuerySignal(SQLERROR, QList<PictureDao>());  // 发送错误状态和空列表
        return;
    }

    // 初始化线程池
    threadGetPictureMessage = new QThread;
    imageCaptureService = new ImageCaptureService();

    // 将 ImageCaptureService 移动到线程中
    imageCaptureService->moveToThread(threadGetPictureMessage);

    // 线程启动后调用 ImageCaptureService 的槽函数来处理获取图片信息
    connect(threadGetPictureMessage, &QThread::started, imageCaptureService, [=]() {
        imageCaptureService->GetDateImageSlots(pictureDate , row);
        qDebug() << "[DEBUG] ImageCaptureService started with pictureDate:" << pictureDate;
    });

    // 处理获取到的图片信息并发出信号
    connect(imageCaptureService, &ImageCaptureService::imageListReady, this, [=](const QList<PictureDao> &pictureList) {
        // 发出新信号，发送状态和图片列表
        emit finishedPictureQuerySignal(SQLSUCCESS, pictureList);  // 发送成功状态和图片列表
        threadGetPictureMessage->quit();  // 停止线程
    });

    // 处理服务类出错的情况（假设服务类有一个类似于 errorOccurred 的信号）
    connect(imageCaptureService, &ImageCaptureService::errorOccurred, this, [=](int errorCode) {
        emit finishedPictureQuerySignal(errorCode, QList<PictureDao>());  // 发送错误状态和空列表
        threadGetPictureMessage->quit();  // 停止线程
    });

    // 线程完成时销毁，先销毁线程再通知 UI
    connect(threadGetPictureMessage, &QThread::finished, threadGetPictureMessage, &QObject::deleteLater);
    connect(threadGetPictureMessage, &QThread::finished, imageCaptureService, &QObject::deleteLater);

    // 启动线程
    qDebug() << "[DEBUG] Starting threadGetPictureMessage...";
    threadGetPictureMessage->start();
}




QDate ImageCaptureController::getCurrentDate()
{
    return QDate::currentDate();  // 获取并返回当天的日期
}

