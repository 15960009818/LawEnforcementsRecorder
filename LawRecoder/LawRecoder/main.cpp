#include <QApplication>
#include "win/imagecapturewin.h"
int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    ImageCaptureWin player;
    player.show();

    return app.exec();
}
