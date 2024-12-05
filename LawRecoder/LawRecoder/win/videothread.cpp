#include "videothread.h"
#include <QDate>
#include <QDebug>
#include <QDir>
#include <QImage>
#include <QSettings>
#include <QTimer>

/**
 * @brief VideoThread 构造函数
 *
 * 构造函数在初始化时打开视频文件并获取总帧数和帧率。
 *
 * @param videoPath 视频文件路径
 * @param parent 父对象
 */
VideoThread::VideoThread(const QString &videoPath, QObject *parent)
    : QThread(parent), currentFrame(0), speedFactor(1.0), playing(false), paused(false), stopped(false) {
    // 初始化代码
    qDebug() << "Initializing VideoThread with video path:" << videoPath;

    // 尝试打开视频文件
    cap.open(videoPath.toStdString());
    if (!cap.isOpened()) {
        qDebug() << "Error opening video file!";
        return;
    }

    // 获取视频的总帧数和帧率
    totalFrames = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));
    fps = cap.get(cv::CAP_PROP_FPS);

    qDebug() << "Video opened successfully. Total frames:" << totalFrames << "FPS:" << fps;
}

/**
 * @brief VideoThread 析构函数
 *
 * 析构函数在销毁时停止视频线程。
 */
VideoThread::~VideoThread() {
    stop();
    //wait();
    qDebug() << "VideoThread is being destroyed.";
}

/**
 * @brief 判断视频文件是否成功打开
 *
 * @return 如果视频文件成功打开返回 true，否则返回 false
 */
bool VideoThread::isOpened() const {
    return cap.isOpened();
}

/**
 * @brief 停止视频线程
 *
 * 停止视频播放并退出线程。
 */
void VideoThread::stop() {
    QMutexLocker locker(&mutex);  // 加锁确保线程安全
    stopped = true;
    waitCondition.wakeAll();  // 唤醒所有等待的线程
    qDebug() << "Stopping VideoThread...";
}

/**
 * @brief 暂停视频播放
 *
 * 将视频播放状态设置为暂停，暂停时视频线程会等待。
 */
void VideoThread::pause() {
    QMutexLocker locker(&mutex);  // 加锁确保线程安全
    paused = true;
    qDebug() << "Video paused at frame:" << currentFrame;
}

/**
 * @brief 恢复视频播放
 *
 * 将视频播放状态设置为恢复，唤醒视频线程继续播放。
 */
void VideoThread::resume() {
    QMutexLocker locker(&mutex);  // 加锁确保线程安全
    paused = false;
    waitCondition.wakeAll();  // 唤醒等待的线程
    qDebug() << "Resuming video from frame:" << currentFrame;
}

/**
 * @brief 跳转到指定帧
 *
 * 设置当前帧为指定帧并更新视频文件的播放位置。
 *
 * @param frame 目标帧数
 */
void VideoThread::seekToFrame(int frame) {
    QMutexLocker locker(&mutex);  // 加锁确保线程安全
    currentFrame = frame;
    cap.set(cv::CAP_PROP_POS_FRAMES, frame);  // 设置视频播放位置为指定帧
    waitCondition.wakeAll();  // 唤醒等待的线程
    qDebug() << "Seeking to frame:" << currentFrame;
}

/**
 * @brief 设置视频播放速度
 *
 * 修改播放速度的倍数。
 *
 * @param factor 播放速度倍数
 */
void VideoThread::setSpeedFactor(double factor) {
    QMutexLocker locker(&mutex);  // 加锁确保线程安全
    speedFactor = factor;
    qDebug() << "Speed factor set to:" << speedFactor;
}

/**
 * @brief 截图按钮点击事件的槽函数
 *
 * 该函数在点击截图按钮时触发，通知视频线程进行截图操作。
 */
void VideoThread::captureScreenshot() {
    // QMutexLocker locker(&mutex); // 可加锁以保护视频帧，当前暂时不需要加锁

    // 读取当前帧
    cv::Mat frame;
    cap.set(cv::CAP_PROP_POS_FRAMES, currentFrame);  // 设置视频的当前帧位置
    cap >> frame;  // 读取帧
    if (frame.empty()) {
        qWarning("Failed to capture screenshot - empty frame.");
        return;
    }

    // 转换颜色格式并保存图像
    cv::Mat rgbFrame;
    cv::cvtColor(frame, rgbFrame, cv::COLOR_BGR2RGB);  // 转换为RGB格式
    QImage img((uchar*)rgbFrame.data, rgbFrame.cols, rgbFrame.rows, rgbFrame.step, QImage::Format_RGB888);  // 创建QImage

    // 获取当前时间戳作为文件名
    QString timestamp = QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss");

    // 设置保存路径，支持 Windows 和 Ubuntu
    QString defaultPath;
#ifdef Q_OS_WIN
    // Windows 使用用户的 "Pictures" 文件夹
    defaultPath = QStandardPaths::writableLocation(QStandardPaths::PicturesLocation);
#elif defined(Q_OS_UNIX)
    // Ubuntu 使用 "/home/用户名/Pictures" 文件夹
    defaultPath = QDir::homePath() + "/Pictures";
#else
    // 其他操作系统，使用当前工作目录
    defaultPath = QDir::currentPath();
#endif

    QDir().mkpath(defaultPath);  // 如果目录不存在则创建
    QString outfile = QDir::cleanPath(defaultPath + "/" + timestamp + ".jpg");  // 设置截图保存路径

    // 保存截图
    if (img.save(outfile, "JPEG")) {
//        qDebug() << "Screenshot saved at:" << outfile;  // 输出保存路径
//        QSettings settings("../settings.ini", QSettings::IniFormat);
//        QString userAccount = settings.value("User/userAccount").toString();
//         QThreadPool* pool = QThreadPool::globalInstance();
//        fileUploader = new FileUploader(outfile,userAccount,pool);
////        // 上传截图文件
//        fileUploader->startUpload();
    } else {
        qDebug() << "Failed to save screenshot.";  // 截图保存失败时的输出
    }
}

/**
 * @brief 线程运行函数
 *
 * 该函数在线程启动时执行，处理视频播放、暂停、截图请求等操作。
 */
void VideoThread::run() {
    qDebug() << "VideoThread started. Playing video...";
    while (!stopped) {
        QMutexLocker locker(&mutex);  // 加锁确保线程安全
        if (paused) {
            qDebug() << "Video paused at frame:" << currentFrame;
            waitCondition.wait(&mutex);  // 如果暂停，等待唤醒
        }

        if (stopped) break;

        cv::Mat frame;
        if (cap.read(frame)) {
            emit dataSend2UI(currentFrame, frame);  // 将当前帧数据发送到UI
            currentFrame++;

            if (currentFrame >= totalFrames) {
                emit finishedPlaying();  // 如果播放完所有帧，发出播放完成信号
                break;
            }

            int delay = static_cast<int>(1000 / (fps * speedFactor));  // 根据速度因子计算帧间延迟
            msleep(delay);  // 等待延迟时间

            // 检查是否请求截图
            if (screenshotRequested) {
                captureScreenshot();  // 执行截图
                screenshotRequested = false;  // 重置标志位
            }
        } else {
            emit finishedPlaying();  // 如果无法读取帧，发出播放完成信号
            break;
        }
    }
}
