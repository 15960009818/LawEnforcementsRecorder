#include "logincontroller.h"

LoginController::LoginController()
{
    initController();
}
LoginController::~LoginController()
{

}

int LoginController::getLoginFlag() const {
    return loginflag;
}


void LoginController::setLoginFlag(int flag) {
    loginflag = flag;
}

/**
 * @brief LoginController::initController 初始化函数
 */
void LoginController::initController()
{
     connect(this, SIGNAL(startedLoginSignal()), this, SLOT(startLoginSlot()));
}

/**
 * @brief LoginController::LoginCheckSlots 登录验证槽
 * @param userAccount 用户账号
 * @param userPassword 用户密码
 * @param inputVerificationCode 用户输入的验证码
 * @param generatedVerificationCode 生成的验证码
 */
void LoginController::LoginCheckSlots(const QString &userAccount, const QString &userPassword, const QString &inputVerificationCode, const QString &generatedVerificationCode)
{
    // 首先判断用户输入内容是否为空
    if (userAccount.isEmpty() || userPassword.isEmpty() || inputVerificationCode.isEmpty()) {
        qDebug() << "账号、密码或验证码不能为空";
        emit finishedLoginServiceSignal();  // 假设此处会通过信号通知UI或其他部分
        return;
    }

    // 检查输入的验证码和生成的验证码是否一致
    if (inputVerificationCode != generatedVerificationCode) {
        qDebug() << "验证码错误";
        emit finishedLoginServiceSignal();  // 假设此处会通过信号通知UI或其他部分
        return;
    }

    // 验证通过，开始执行登录逻辑，通过线程池处理登录服务
    threadLoginCheck = new QThread;
    loginService = new LoginService();

    // 将 LoginService 移动到线程中
    loginService->moveToThread(threadLoginCheck);

    // 线程启动后调用 LoginService 的槽函数来处理登录
    connect(threadLoginCheck, &QThread::started, loginService, [=]() {
        loginService->GetLoginCheckSlots(userAccount, userPassword);  // 传入账号和密码
    });

    // 线程完成时销毁
    connect(threadLoginCheck, &QThread::finished, loginService, &QObject::deleteLater);

    // 确保线程结束后通知 UI，避免卡死
    connect(threadLoginCheck, &QThread::finished, this, &LoginController::finishedLoginServiceThreadSlot);
    connect(this, &LoginController::finishedLoginServiceSignal, this, &LoginController::quitLoginServiceThreadSlot);

    // 启动线程
    threadLoginCheck->start();
}


/**
 * @brief LoginController::startLoginSlot 初始化槽函数
 */
void LoginController::startLoginSlot()
{

    setLoginFlag(0);
}

/**
 * @brief LoginController::quitLoginServiceThreadSlot 退出loginservice执行槽
 */
void LoginController::quitLoginServiceThreadSlot()
{

}

/**
 * @brief LoginController::finishedLoginServiceThreadSlot 返回给ui的槽
 */
void LoginController::finishedLoginServiceThreadSlot()
{

}
