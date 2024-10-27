#include <QApplication>
#include "win/imagecapturewin.h"
#include "win/settingwin.h"
#include <iostream>
#include "../common/jsonparser.h"
#include "../common/jsonbuilder.h"
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

     SettingWin player;
     player.show();



    return app.exec();
}
