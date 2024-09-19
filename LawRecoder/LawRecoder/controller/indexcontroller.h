#ifndef INDEXCONTROLLER_H
#define INDEXCONTROLLER_H

#include <QObject>
#include <QThread>
#include "../common/singleton.h"
#include "../common/recordthread.h"
class IndexController :public QObject
{
    
    SINGLETON(IndexController)
    Q_OBJECT
signals:
        void sendImg(const QImage &img);
public slots:
        void handleImageReady(const QImage &img);
private:
     RecordThread *recordThread;
    
};

#endif // INDEXCONTROLLER_H
