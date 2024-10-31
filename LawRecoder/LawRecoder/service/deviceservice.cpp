#include "deviceservice.h"
#include <QDebug>

DeviceService::DeviceService()
{
    qDebug() << "[INFO] DeviceService initialized";
}

DeviceService::~DeviceService(){

}

QString DeviceService::getVideoPath()
{
    qDebug() << "[INFO] Retrieving video path:" << VideoPath;
    return VideoPath;
}

QString DeviceService::getPicturePath()
{
    qDebug() << "[INFO] Retrieving picture path:" << PicturePath;
    return PicturePath;
}

void DeviceService::setVideoPath(const QString *VideoPath)
{
    // 清空当前视频路径
    this->VideoPath.clear();

    // 检查指针是否为空，并设置视频路径
    if (VideoPath != nullptr)
    {
        this->VideoPath = *VideoPath;
        qDebug() << "[INFO] Video path set to:" << this->VideoPath;
    }
    else
    {
        qDebug() << "[WARNING] Attempted to set video path, but received a null pointer.";
    }
}

void DeviceService::setPicturePath(const QString *PicturePath)
{
    // 清空当前图片路径
    this->PicturePath.clear();

    // 检查指针是否为空，并设置图片路径
    if (PicturePath != nullptr)
    {
        this->PicturePath = *PicturePath;
        qDebug() << "[INFO] Picture path set to:" << this->PicturePath;
    }
    else
    {
        qDebug() << "[WARNING] Attempted to set picture path, but received a null pointer.";
    }
}
