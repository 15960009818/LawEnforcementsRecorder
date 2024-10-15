#ifndef IMAGECAPTUREWIN_H
#define IMAGECAPTUREWIN_H

#include <QObject>
#include <QWidget>
#include <QGridLayout>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QAction>
#include <QDateEdit>
#include <QListWidget>
#include <QListView>

class ImageCaptureWin : public QWidget
{
    Q_OBJECT
public:
    explicit ImageCaptureWin(QWidget *parent = nullptr);

    // 设置UI界面
    void setUi();
    void connectSignals();
    void paintEvent(QPaintEvent *event) override;

private slots:
    void BtnClicked();

private:
    QGridLayout *glay;           // 网格布局
    QLabel *LabTitle;            // 标题标签
    QLabel *LabDate;             // 日期选择标签
    QLabel *LabShowBigPic;       // 显示大图片
    QLabel *LabShowDetlPic;      // 显示详细图片
    QLabel *LabptName;           // 图片名称标签
    QLabel *LabptTime;           // 图片创建时间标签
    QLabel *LabptPath;           // 图片路径标签
    QLabel *LabptNameS;          // 显示的图片名称
    QLabel *LabptTimeS;          // 显示的图片创建时间
    QLabel *LabptPathS;          // 显示的图片路径

    QPushButton *BtnReturnList;  // 返回图片列表按钮
    QPushButton *BtnMore;        // 查看更多按钮
    QToolButton *TlBtnReturn;    // 返回按钮
    QToolButton *TlBtnCalendar;  // 日历工具按钮
    QAction *ActReturn;          // 返回动作
    QDateEdit *editdatetime;     // 日期选择器
    QListWidget *videowins;      // 显示图片列表的控件
};

#endif // IMAGECAPTUREWIN_H
