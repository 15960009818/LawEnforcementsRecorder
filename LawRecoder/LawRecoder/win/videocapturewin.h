#ifndef VIDEOCAPTUREWIN_H
#define VIDEOCAPTUREWIN_H

#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QDateEdit>
#include <QListWidget>
#include <QMessageBox>
#include <QDebug>
#include <QAction>
#include "../dao/videodao.h"
#include "../controller/videocapturecontroller.h"

class VideoCaptureWin : public QWidget {
    Q_OBJECT
signals:
    void videoSelected(const QString &videoPath); // 视频选中信号

public:
    explicit VideoCaptureWin(QWidget *parent = nullptr);

private:
    void setUi(); // 设置UI界面
    void connectSignals(); // 连接信号和槽

private slots:
    void onFinishedVideoQuery(const QString &message, const QList<VideoDao> &videoList); // 查询视频完成的处理函数
    void BtnClicked(); // 按钮点击处理函数

private:
    QGridLayout *glay;            // 网格布局
    QLabel *LabTitle;              // 标题标签
    QLabel *LabDate;               // 日期选择标签
    QLabel *LabShowBigVid;         // 显示大视频的标签
    QLabel *LabVidName;            // 视频名称标签
    QLabel *LabVidTime;            // 视频创建时间标签
    QLabel *LabVidPath;            // 视频路径标签
    QLabel *LabVidNameS;           // 视频名称显示标签
    QLabel *LabVidTimeS;           // 视频创建时间显示标签
    QLabel *LabVidPathS;           // 视频路径显示标签
    QToolButton *TlBtnReturn;      // 返回按钮
    QDateEdit *editdatetime;       // 日期选择器
    QListWidget *videowins;        // 视频列表控件
    QPushButton *BtnMore;          // “查看更多”按钮
    QPushButton *BtnReturnList;    // 返回视频列表按钮
    QDate selectedDate;            // 选择的日期
    int page = 1;                  // 当前页面
    QAction *ActReturn;            // 返回按钮的动作
};

#endif // VIDEOCAPTUREWIN_H
