#include "settingcontroller.h"

#include <QStorageInfo>

SettingController::SettingController()
{
    initController();
}

/**
 * @brief LoginController::initController 初始化信号与槽
 */
void SettingController::initController()
{
    qDebug() << "[DEBUG] Initializing LoginController...";

    connect(this,SIGNAL(getPathSignals(QString PathName)),this,SLOT(getPathSlots(QString PathName)));

}
/**
 * @brief SettingController::getDirectorySize 递归计算文件夹及其子文件夹的总大小
 * @param path 需要计算的文件夹路径
 * @return 文件夹的总大小（字节）
 */
qint64 SettingController::getDirectorySize(const QString& path) {
    qint64 size = 0;
    QDir dir(path);
    QFileInfoList list = dir.entryInfoList(QDir::Files | QDir::Dirs | QDir::NoDotAndDotDot | QDir::Hidden);

    for (const QFileInfo &fileInfo : list) {
        if (fileInfo.isDir()) {
            size += getDirectorySize(fileInfo.absoluteFilePath()); // 递归计算子目录大小
        } else {
            size += fileInfo.size(); // 累加文件大小
        }
    }

    return size;
}

/**
 * @brief SettingController::getPathSlots 计算文件夹的总大小，并判断磁盘空间是否充足
 * @param PathName 用户选择的文件夹路径
 */
void SettingController::getPathSlots(QString PathName)
{
    qDebug() << "[DEBUG] Received path from SettingWin:" << PathName;

    // 计算文件夹大小
    qint64 folderSize = getDirectorySize(PathName);
    qDebug() << "[INFO] Total folder size:" << folderSize / (1024 * 1024) << "MB";

    // 获取磁盘可用空间
    QStorageInfo storageInfo(PathName);
    qint64 availableSpace = storageInfo.bytesAvailable();
    qDebug() << "[INFO] Available disk space:" << availableSpace / (1024 * 1024) << "MB";

    // 假设用户期望至少 1000MB 的剩余空间
    qint64 requiredSpaceMB = 1000;

    // 判断剩余空间是否足够
    if (availableSpace > requiredSpaceMB * 1024 * 1024) {
        qDebug() << "[INFO] Sufficient space available.";
    } else {
        qDebug() << "[WARNING] Insufficient space.";
    }
}
