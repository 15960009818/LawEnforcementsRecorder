#include <QApplication>
#include "win/loginwin.h"
#include "service/loginservice.h"
#include "win/settingwin.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    SettingWin w;
    w.show();

    return a.exec();
}
