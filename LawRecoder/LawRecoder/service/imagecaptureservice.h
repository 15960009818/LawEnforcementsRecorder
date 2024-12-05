#ifndef IMAGECAPTURESERVICE_H
#define IMAGECAPTURESERVICE_H

#include <QObject>
#include <QDate>
#include <QList>
#include "../dao/picturedao.h"

class ImageCaptureService : public QObject
{
    Q_OBJECT
public:
    explicit ImageCaptureService(QObject *parent = nullptr);

public slots:
    // 查询指定日期的图片信息，分页显示，每页4条记录
    void GetDateImageSlots(const QDate &date, const int &page);

signals:
    // 图片列表准备好时发出的信号，传递查询结果
    void imageListReady(const QList<PictureDao> &pictureList);

    // 发生错误时发出的信号，传递错误代码
    void errorOccurred(int errorCode);
};

#endif // IMAGECAPTURESERVICE_H
