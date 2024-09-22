#ifndef INDEXWIN_H
#define INDEXWIN_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include "../controller/indexcontroller.h"
#include "../common/singleton.h"
class IndexWin :public QWidget
{
    Q_OBJECT
private:
    QLabel *LabTitle;         // 标题标签
        QPushButton *BtnVideo;    // 视频按钮
        QPushButton *BtnTakPic;   // 现场拍照按钮
        QPushButton *BtnBakVid;   // 录像回放按钮
        QPushButton *BtnSeePic;   // 现场图片按钮
        QPushButton *BtnSeeLog;   // 查看日志按钮
        QPushButton *BtnTrsVid;   // 录像转码按钮
        QPushButton *BtnSettings; // 系统设置按钮
        QPushButton *BtnLogin;    // 登录按钮
        QPushButton *BtnReturn;   // 退出按钮
        QPushButton *BtnRgs;      // 注册按钮

        QGridLayout *glay;        // 网格布局

         QImage img;              //摄像头图像
public:
    explicit IndexWin(QWidget *parent = nullptr);

    // 设置UI界面
    void setUi();
    void connectSignals();
    void paintEvent(QPaintEvent *event);
private slots:
    void BtnClicked();
    void recviceImage(const QImage &img);
};

#endif // IndexWin_H
