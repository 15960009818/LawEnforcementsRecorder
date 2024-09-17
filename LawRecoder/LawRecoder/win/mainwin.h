#ifndef MAINWIN_H
#define MAINWIN_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>

class MainWin :public QWidget
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
public:
    explicit MainWin(QWidget *parent = nullptr);

    // 设置UI界面
    void setUi();
    void connectSignals();
private slots:
    //void BtnClicked();
};

#endif // MAINWIN_H
