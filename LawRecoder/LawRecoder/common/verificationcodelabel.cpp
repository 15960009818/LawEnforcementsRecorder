#include "verificationcodelabel.h"
#include <QPaintEvent>
#include <QPainter>
#include <QTime>
#include <QDebug>
/**
 * @类名: VerificationCodeLabel
 * @描述: 用于生成和显示验证码的 QLabel 子类，通过重写 paintEvent 绘制验证码和噪点。
 * @作者: 翔君
 * @创建日期: 2024/10/21
 * @是否完成: 完成
 */
VerificationCodeLabel::VerificationCodeLabel(QWidget *parent)
    : QLabel(parent)
{
    // 生成随机种子，确保每次运行生成不同的验证码
    qsrand(QTime::currentTime().second() * 1000 + QTime::currentTime().msec());
    colorArray = new QColor[letter_number]; // 初始化用于保存每个字母颜色的数组
    verificationCode = new QChar[letter_number]; // 初始化用于保存验证码字符的数组
    noice_point_number = this->width(); // 将噪点数量设置为窗口宽度
}

/**
 * @函数名: paintEvent
 * @描述: 重写的绘制事件，生成验证码并绘制在控件上，包括随机颜色、字符和噪点。
 * @参数: QPaintEvent *event - 传递的绘制事件参数
 * @返回值: 无
 * @作者: 翔君
 * @创建日期: 2024/10/21
 * @是否完成: 完成
 */
void VerificationCodeLabel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this); // 创建画笔
    QPoint p; // 用于保存绘制字符或噪点的坐标

    // 将背景设置为白色
    painter.fillRect(this->rect(), Qt::white);

    // 生成 4 个不同的验证码字符
    produceVerificationCode();

    // 生成 4 个随机颜色
    produceRandomColor();

    // 绘制验证码字符
    for (int i = 0; i < letter_number; ++i)
    {
        p.setX(i * (this->width() / letter_number) + this->width() / 8); // 计算字符 X 坐标
        p.setY(this->height() / 2); // 计算字符 Y 坐标
        painter.setPen(colorArray[i]); // 设置画笔颜色
        painter.drawText(p, QString(verificationCode[i])); // 绘制字符
    }

    // 绘制随机噪点
    for (int j = 0; j < noice_point_number; ++j)
    {
        p.setX(qrand() % this->width()); // 随机生成噪点的 X 坐标
        p.setY(qrand() % this->height()); // 随机生成噪点的 Y 坐标
        painter.setPen(colorArray[j % 4]); // 随机选择颜色
        painter.drawPoint(p); // 绘制噪点
    }

    // 将验证码字符拼接为字符串并保存到成员变量 v
    QString a0 = (QString)verificationCode[0];
    QString a1 = (QString)verificationCode[1];
    QString a2 = (QString)verificationCode[2];
    QString a3 = (QString)verificationCode[3];
    this->v = QString("%1%2%3%4").arg(a0).arg(a1).arg(a2).arg(a3); // 拼接验证码为字符串

    // 输出调试信息
    qDebug() << "验证码拼接: " << v << endl;
    qDebug() << "验证码第一个字符: " << verificationCode[0] << endl;

    return;
}

/**
 * @函数名: produceVerificationCode
 * @描述: 生成验证码字符，调用 produceRandomLetter 生成随机字符并存储到 verificationCode 数组中。
 * @参数: 无
 * @返回值: 无
 * @作者: 翔君
 * @创建日期: 2024/10/21
 * @是否完成: 完成
 */
void VerificationCodeLabel::produceVerificationCode() const
{
    for (int i = 0; i < letter_number; ++i)
        verificationCode[i] = produceRandomLetter(); // 调用函数生成随机字符
    return;
}

/**
 * @函数名: produceRandomLetter
 * @描述: 生成一个随机的字符，可能是数字、大写字母或小写字母。
 * @参数: 无
 * @返回值: QChar - 生成的随机字符
 * @作者: 翔君
 * @创建日期: 2024/10/21
 * @是否完成: 完成
 */
QChar VerificationCodeLabel::produceRandomLetter() const
{
    QChar c;
    int flag = qrand() % letter_number; // 随机决定字符类型
    switch (flag)
    {
    case NUMBER_FLAG: c = '0' + qrand() % 10; break; // 生成数字
    case UPLETTER_FLAG: c = 'A' + qrand() % 26; break; // 生成大写字母
    case LOWLETTER_FLAG: c = 'a' + qrand() % 26; break; // 生成小写字母
    default: c = qrand() % 2 ? 'W' : 'D'; // 默认字符
    }
    return c;
}

/**
 * @函数名: produceRandomColor
 * @描述: 生成随机颜色并存储到 colorArray 数组中。
 * @参数: 无
 * @返回值: 无
 * @作者: 翔君
 * @创建日期: 2024/10/21
 * @是否完成: 完成
 */
void VerificationCodeLabel::produceRandomColor() const
{
    for (int i = 0; i < letter_number; ++i)
        colorArray[i] = QColor(qrand() % 255, qrand() % 255, qrand() % 255); // 随机生成颜色
    return;
}

/**
 * @函数名: getVerificationCode
 * @描述: 返回验证码字符串，所有字符转换为大写。
 * @参数: 无
 * @返回值: QString - 验证码字符串
 * @作者: 翔君
 * @创建日期: 2024/10/21
 * @是否完成: 完成
 */
QString VerificationCodeLabel::getVerificationCode() const
{
    QString s;
    QChar cTemp;
    for (int i = 0; i < letter_number; ++i)
    {
        cTemp = verificationCode[i];
        s += cTemp > 97 ? cTemp.toUpper() : cTemp; // 转换为大写字母
    }
    qDebug() << "验证码: " << s << endl;
    return s;
}

/**
 * @函数名: mousePressEvent
 * @描述: 重写鼠标点击事件，当鼠标左键点击时刷新验证码。
 * @参数: QMouseEvent *event - 鼠标事件参数
 * @返回值: 无
 * @作者: 翔君
 * @创建日期: 2024/10/21
 * @是否完成: 完成
 */
void VerificationCodeLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        slt_reflushVerification(); // 刷新验证码
    }
    QLabel::mousePressEvent(event); // 传递事件给基类处理
}

/**
 * @函数名: slt_reflushVerification
 * @描述: 刷新验证码，调用 repaint() 重绘。
 * @参数: 无
 * @返回值: 无
 * @作者: 翔君
 * @创建日期: 2024/10/21
 * @是否完成: 完成
 */
void VerificationCodeLabel::slt_reflushVerification()
{
    repaint(); // 重新绘制验证码
}

/**
 * @析构函数: ~VerificationCodeLabel
 * @描述: 析构函数，销毁对象时释放资源。
 * @作者: 翔君
 * @创建日期: 2024/10/21
 * @是否完成: 完成
 */
VerificationCodeLabel::~VerificationCodeLabel()
{
}

