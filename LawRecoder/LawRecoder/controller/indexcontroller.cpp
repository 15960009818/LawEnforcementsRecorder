#include "indexcontroller.h"
//#include <QCameraInfo>
#include <QStringList>
IndexController::IndexController()
{
    initController();

}
IndexController::~IndexController(){
    qDebug() << "[DEBUG] IndexController destroyed.";
}

void IndexController::initController(){

    recordThread = new RecordThread(this);
    recordThread->start();
    // 将获取到的摄像头img发去处理
    connect(recordThread, &RecordThread::imgReady, this, &IndexController::handleImageReady);

}
/**
 * @槽函数: handleImageReady
 * @描述: 处理 RecordThread 发出的 imgReady 信号
 * @参数: const QImage &img - 接收到的图像数据
 */
void IndexController::handleImageReady(const QImage &img)
{

    qDebug() << "Received image of size:" << img.size();

    emit sendImg(img);
}



