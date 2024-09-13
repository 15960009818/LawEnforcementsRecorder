#include <QApplication>
#include "win/loginwin.h"
#include "service/loginservice.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    LoginService l;
    l.GetLoginCheckSlots("dsada","dsadas");
//    LoginWin w;
//    w.show();

    return a.exec();
}
