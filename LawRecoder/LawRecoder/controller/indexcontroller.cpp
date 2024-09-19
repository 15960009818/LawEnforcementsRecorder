#include "indexcontroller.h"

IndexController::IndexController()
{

    recordThread = new RecordThread(this);

    // 将获取到的摄像头img发去处理
    connect(recordThread, &RecordThread::imgReady, this, &IndexController::handleImageReady);

    recordThread->start();
}
IndexController::~IndexController(){
    qDebug() << "[DEBUG] IndexController destroyed.";
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
