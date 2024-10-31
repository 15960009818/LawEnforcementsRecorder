#include <QApplication>
#include "win/imagecapturewin.h"
#include "win/settingwin.h"
#include <iostream>
#include "../common/jsonparser.h"
#include "../common/jsonbuilder.h"
#include "../common/singleton.h"
#include "../win/videocapturewin.h"
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

     Singleton<SettingWin>::getInstance().show();
//    VideoCaptureWin video;
//    video.show();


    return app.exec();
}
