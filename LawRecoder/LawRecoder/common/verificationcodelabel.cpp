#include "verificationcodelabel.h"
#include <QPaintEvent>
#include <QPainter>
#include <QTime>
#include <QDebug>
VerificationCodeLabel::VerificationCodeLabel(QWidget *parent)
    : QLabel(parent)
{
    //生成随机种子
    qsrand(QTime::currentTime().second() * 1000 + QTime::currentTime().msec());
    colorArray = new QColor[letter_number];
    verificationCode = new QChar[letter_number];
    noice_point_number = this->width();
}
//重写绘制事件,以此来生成验证码
void VerificationCodeLabel::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);
    QPoint p;
    //背景设为白色
    painter.fillRect(this->rect(), Qt::white);
    //产生4个不同的字符
    produceVerificationCode();
    //产生4个不同的颜色
    produceRandomColor();
    //绘制验证码
    for (int i = 0; i < letter_number; ++i)
    {
        p.setX(i*(this->width() / letter_number)+this->width()/8);
        p.setY(this->height() / 2);
        painter.setPen(colorArray[i]);
        painter.drawText(p, QString(verificationCode[i]));
    }
    //绘制噪点
    for (int j = 0; j < noice_point_number; ++j) //noice_point_number噪声点数
    {
        p.setX(qrand() % this->width());
        p.setY(qrand() % this->height());
        painter.setPen(colorArray[j % 4]);
        painter.drawPoint(p);
    }

    QString a0=(QString)verificationCode[0];
    QString a1=(QString)verificationCode[1];
    QString a2=(QString)verificationCode[2];
    QString a3=(QString)verificationCode[3];
//    v.sprintf("%s%s%s%s",a0,a1,a2,a3);
    this->v=QString("%1%2%3%4").arg(a0).arg(a1).arg(a2).arg(a3);

    qDebug()<<"11111:"<<v<<endl;
    qDebug()<<"yzm:"<<verificationCode[0]<<endl;
    return;
}
//这是一个用来生成验证码的函数
void VerificationCodeLabel::produceVerificationCode() const
{
    for (int i = 0; i < letter_number; ++i)
        verificationCode[i] = produceRandomLetter();
    return;
}
//产生一个随机的字符
QChar VerificationCodeLabel::produceRandomLetter() const
{
    QChar c;
    int flag = qrand() % letter_number;
    switch (flag)
    {
    case NUMBER_FLAG:c='0' + qrand() % 10; break;
    case UPLETTER_FLAG:c='A' + qrand() % 26; break;
    case LOWLETTER_FLAG:c='a' + qrand() % 26; break;
    default:c=qrand() % 2 ? 'W' : 'D';
    }
    return c;
}
//产生随机的颜色
void VerificationCodeLabel::produceRandomColor() const
{
    for (int i = 0; i < letter_number; ++i)
        colorArray[i] = QColor(qrand() % 255, qrand() % 255, qrand() % 255);
    return;
}
//返回一个字符串（字母一律都按照大写返回）
QString VerificationCodeLabel::getVerificationCode() const
{
    QString s;
    QChar cTemp;
    for (int i = 0; i < letter_number; ++i)
    {
        cTemp = verificationCode[i];
        s += cTemp>97?cTemp.toUpper():cTemp;
    }
    qDebug()<<"验证码："<<s<<endl;
    return s;
}
// 重写鼠标点击事件以刷新验证码
void VerificationCodeLabel::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton)
    {
        slt_reflushVerification();
    }
    QLabel::mousePressEvent(event); // 传递事件给基类处理
}
//刷新验证码，在用户不确定的时候进行相应刷新
void VerificationCodeLabel::slt_reflushVerification()
{
    repaint();
}
VerificationCodeLabel::~VerificationCodeLabel()
{

}
