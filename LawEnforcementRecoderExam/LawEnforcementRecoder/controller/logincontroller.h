#ifndef LOGINCONTROLLER_H
#define LOGINCONTROLLER_H

#include <QObject>
#include <QThread>
#include <QString>
#include "../common/singleton.h"
class LoginController:public QObject
{
    //单例模式，不要再头文件写构造函数了
    SINGLETON(LoginController)
       Q_OBJECT
signals:
        //开始执行信号
        void startedLoginControllerSignal();

        //执行完成线程停止信号
        void finishedLoginCheckSignal();

        //数据返回给ui信号
        void showFinishedLoginCheckUiSignal();
private:

        //登录标识
      int LoginFlag;

        /**
          * @brief loginCheck 登录验证
          * @param userAccount 用户账号
          * @param userPassword 用户密码
          * @param userVerify 用户验证码
          * @param genVerify 生成验证码
          * @return 登录成功/失败
          */
     bool loginCheck(QString userAccount,QString userPassword, QString userVerify, QString genVerify);

private slots:
     //开始执行槽
     void startLoginControllerSlot();

     //线程结束槽
     void quitCountThreadSlot();

     //数据返回给ui槽
     void finishedCountThreadSlot();

};

#endif // LOGINCONTROLLER_H
