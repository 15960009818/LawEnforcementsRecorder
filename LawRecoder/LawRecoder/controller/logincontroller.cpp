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
 * @brief LoginController::initController
 */
void LoginController::initController()
{
     connect(this, SIGNAL(startedLoginSignal()), this, SLOT(startLoginSlot()));
}

/**
 * @brief LoginController::LoginCheckSlots 登录验证槽
 * @param userAccount
 * @param userPassword
 * @param inputVerificationCode
 * @param generatedVerificationCode
 */
void LoginController::LoginCheckSlots(const QString &userAccount, const QString &userPassword, const QString &inputVerificationCode, const QString &generatedVerificationCode)
{

}

/**
 * @brief LoginController::startLoginSlot 初始化槽函数
 */
void LoginController::startLoginSlot()
{

    setLoginFlag(0);
}
