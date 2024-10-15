#ifndef SETTINGCONTROLLER_H
#define SETTINGCONTROLLER_H

#include <QObject>
#include <QDebug>
#include <QString>
#include <QFileDialog>
#include "../common/singleton.h"
#include "../win/settingwin.h"

class SettingController : public QObject
{
    Q_OBJECT
    SINGLETON(SettingController)

private:
    SettingWin* settingWin; // 指向 SettingWin 的指针

signals:
    void spaceSufficient(qint64 availableSpaceMB);  // 信号：剩余空间足够
    void spaceInsufficient(QString errorMsg);        // 信号：空间不足

public:
    SettingController(SettingWin* settingWinInstance, QObject* parent = nullptr)
        : QObject(parent), settingWin(settingWinInstance) {
        initController();
    }

    void initController(); // 初始化信号与槽
    QString PathName; // 可以考虑移除

private slots:
    void getPathSlots(QString PathName, qint64 requiredSpaceMB);

private:
    qint64 getDirectorySize(const QString& path); // 递归计算文件夹大小
};

#endif // SETTINGCONTROLLER_H
