#include <QApplication>
#include "win/imagecapturewin.h"
#include "win/settingwin.h"
#include <iostream>
#include "win/imagecapturewin.h"
#include "win/videocapturewin.h"
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    qRegisterMetaType<cv::Mat>("cv::Mat");
     Singleton<SettingWin>::getInstance().show();


    return app.exec();
}



