#ifndef SETTINGCONTROLLER_H
#define SETTINGCONTROLLER_H

#include <QObject>
#include <QThread>
#include <QDebug>
#include <QString>
#include <QFileDialog>
#include <QString>
#include "../common/singleton.h"
#include "../win/settingwin.h"
class SettingController :public QThread
{
    Q_OBJECT
    SINGLETON(SettingController)
    qint64 getDirectorySize(const QString &path);
signals:

        void getPathSignals(QString PathName);
public:

    void initController();
    QString PathName;
public slots:
        void getPathSlots(QString PathName);

};

#endif // SETTINGCONTROLLER_H
