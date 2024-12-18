#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsProxyWidget>
#include "win/videocapturewin.h"  // 视频捕获窗口

class RotatedWindow : public QGraphicsView {
public:
    RotatedWindow(QWidget *widgetToEmbed, int rotationAngle = 90, QWidget *parent = nullptr)
        : QGraphicsView(parent) {
        // 创建场景
        QGraphicsScene *scene = new QGraphicsScene(this);

        // 将目标窗口嵌入场景
        QGraphicsProxyWidget *proxyWidget = scene->addWidget(widgetToEmbed);
        proxyWidget->setTransformOriginPoint(proxyWidget->boundingRect().center());
        proxyWidget->setRotation(rotationAngle);

        // 设置场景到视图
        setScene(scene);

        // 调整视图的大小以适配旋转后的内容
        QRectF rotatedRect = proxyWidget->sceneTransform().mapRect(proxyWidget->boundingRect());
        setFixedSize(rotatedRect.size().toSize() + QSize(10, 10));  // 添加额外边距
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // 创建视频捕获窗口
    VideoCaptureWin *videoWindow = new VideoCaptureWin();
    videoWindow->setWindowTitle("视频捕获窗口");

    // 使用 RotatedWindow 将窗口旋转 90 度
    RotatedWindow rotatedView(videoWindow, 90);
    rotatedView.setWindowTitle("旋转窗口");
    rotatedView.show();

    return app.exec();
}
