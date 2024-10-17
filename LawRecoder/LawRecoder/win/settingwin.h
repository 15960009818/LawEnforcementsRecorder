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
class SettingWin :public QWidget
{
    Q_OBJECT
signals:

public:
    /**
     * @函数名: LoginWin
     * @描述: 构造函数，初始化登录窗口
     * @参数: parent 父窗口指针，默认为nullptr
     * @作者: 翔君
     * @创建日期: 2024/9/19
     * @是否完成: 未完成
     */
    explicit SettingWin(QWidget *parent = nullptr);

    /**
     * @函数名: setUi
     * @描述: 设置登录窗口的UI界面布局
     * @作者: 翔君
     * @创建日期: 2024/9/19
     * @是否完成: 完成
     */
    void setUi();

    /**
     * @函数名: connectSignals
     * @描述: 连接登录窗口的信号与槽函数
     * @作者: 翔君
     * @创建日期: 2024/9/19
     * @是否完成: 未完成
     */
    void connectSignals();
private:
    QLineEdit *lineEdit;
       QSlider *slider;
       QGridLayout *glay;
       QLabel *LabTitle;
       QLabel *LabDvid;
       QLabel *LabMemSpc;
       QLabel *LabMemSpc1;
       QLabel *LabMemRemain;
       QLabel *LabVdPath;
       QLabel *LabPixPath;
       QLabel *LabMemRemain2;

       QLineEdit *editVdPath;
       QLineEdit *editPixPath;
       QLineEdit *editMemSpc;
       QLineEdit *editMemRemain;

       QPushButton *BtnVdPath;
       QPushButton *BtnPixPath;
       QPushButton *BtnOk;
       QPushButton *BtnCancel;
       QComboBox *CombDvid;

       QString PathName;

private slots:
    /**
     * @函数名: BtnClicked
     * @描述: 处理按钮点击事件的槽函数
     * @作者: 翔君
     * @创建日期: 2024/9/19
     * @是否完成: 未完成
     */
//       void BtnClicked();
       void VideoPathClickedSlots();
       void PicturePathClickedSlots();
       void BtnOkSlot();
       void BtnCancelSlot();
       void checkVideoPathSlots();
       void handleSpaceInsufficient(QString errorMsg);
       void handleSpaceSufficient(qint64 availableSpaceMB);
};

#endif // SETTINGWIN_H
