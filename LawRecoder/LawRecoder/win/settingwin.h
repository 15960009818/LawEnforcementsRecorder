#ifndef SETTINGWIN_H
#define SETTINGWIN_H

#include <QComboBox>
#include <QGridLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QSlider>
#include <QWidget>
#include <QString>
#include <QMessageBox>
#include "../common/singleton.h"
/**
 * @class SettingWin
 * @brief 系统设置窗口类，负责显示和处理系统设置界面
 */
class SettingWin : public QWidget
{
    Q_OBJECT
    SINGLETON(SettingWin)
public:
//    /**
//     * @brief 构造函数，初始化设置窗口
//     * @param parent 父窗口指针，默认为 nullptr
//     */
//    explicit SettingWin(QWidget *parent = nullptr);

    /**
     * @brief 设置 UI 界面布局
     */
    void setUi();

    /**
     * @brief 连接信号与槽函数
     */
    void connectSignals();

private:
    // 界面控件
    QLineEdit *lineEdit;
    QSlider *slider;
    QGridLayout *glay;

    QLabel *LabTitle;
    QLabel *LabDvid;
    QLabel *LabMemSpc;
    QLabel *LabMemRemain;
    QLabel *LabVdPath;
    QLabel *LabPixPath;

    QLineEdit *editVdPath;      ///< 录像存储路径编辑框
    QLineEdit *editPixPath;     ///< 图片存储路径编辑框
    QLineEdit *editMemSpc;      ///< 内存空间编辑框
    QLineEdit *editMemRemain;   ///< 内存剩余编辑框

    QPushButton *BtnVdPath;     ///< 设置视频路径按钮
    QPushButton *BtnPixPath;    ///< 设置图片路径按钮
    QPushButton *BtnOk;         ///< 确定按钮
    QPushButton *BtnCancel;     ///< 取消按钮

    QComboBox *CombDvid;        ///< 设备编号下拉框

    QString PathName;           ///< 用户选择的路径
    int resettingFlag = 0;      ///< 判断是否是第一次进入设置页面

    void setWidgetSizes();
    void addWidgetsToLayout();
private slots:
    /**
     * @brief 视频路径选择按钮槽函数
     */
    void VideoPathClickedSlots();

    /**
     * @brief 图片路径选择按钮槽函数
     */
    void PicturePathClickedSlots();

    /**
     * @brief 确定按钮槽函数
     */
    void BtnOkSlot();

    /**
     * @brief 取消按钮槽函数
     */
    void BtnCancelSlot();


    /**
     * @brief 处理空间不足信号的槽函数
     * @param errorMsg 错误信息
     */
    void handleSpaceInsufficient(QString errorMsg);

    /**
     * @brief 处理空间充足信号的槽函数
     * @param availableSpaceMB 可用空间大小 (MB)
     */
    void handleSpaceSufficient(qint64 availableSpaceMB);
};

#endif // SETTINGWIN_H
