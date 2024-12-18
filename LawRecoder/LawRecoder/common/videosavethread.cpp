#include "videosavethread.h"
#include <QDate>
#include <QDebug>
#include <QDir>
#include "../service/savevideoandpictureservice.h"
#include "serialgps.h"
VideoSaveThread::VideoSaveThread() {
    qDebug() << "VideoSaveThread initialized.";
}

VideoSaveThread::~VideoSaveThread() {
    stop();
    if (videoWriter.isOpened())
    {
            videoWriter.release();
            qDebug() << "Video writer released in destructor.";
    }
    qDebug() << "VideoSaveThread destroyed.";
}

void VideoSaveThread::startSaving(const QString &filename, int width, int height, int fps) {
    if (filename.isEmpty()) {
        qWarning() << "Filename cannot be empty.";
        return; // 或者抛出异常
    }

    outputFile = QDir::cleanPath(filename);
    frameWidth = width;
    frameHeight = height;
    frameRate = fps;
    stopFlag = false;

    qDebug() << "Starting save thread with file:" << outputFile;
    start();
}

void VideoSaveThread::stopSaving() {
    qDebug() << "Request to stop saving.";
    stop();
}

void VideoSaveThread::stop() {
    stopFlag = true;
    qDebug() << "Stopping save thread...";

    // 先停止视频写入
    if (videoWriter.isOpened()) {
        videoWriter.release();
        qDebug() << "Video writer released.";
    }

    // 插入数据库
    insertVideoInfoToDatabase();

    // 手动退出线程的事件循环
    quit();
    wait(); // 等待线程退出
    qDebug() << "Save thread stopped.";
}

void VideoSaveThread::run() {
    try {
        // 获取视频保存路径
        QString videoPath = Singleton<DeviceService>::getInstance().getVideoPath();

        // 如果路径为空，使用默认路径
        if (videoPath.isEmpty()) {
            videoPath = QDir::homePath() + "/Video"; // 默认路径
            QDir().mkpath(videoPath); // 创建目录
        }

        // 设置输出文件路径
        outputFile = QDir::cleanPath(videoPath + "/" + QFileInfo(outputFile).fileName());

        qDebug() << "Trying to open video writer for file:" << outputFile;

        // 初始化视频写入器
        if (!videoWriter.open(outputFile.toStdString(), cv::VideoWriter::fourcc('H', '2', '6', '4'), frameRate, cv::Size(frameWidth, frameHeight))) {
            qWarning() << "Failed to open video writer for file:" << outputFile;
            //return;
        }

        qDebug() << "Video writer opened successfully.";

        // 启动事件循环，保持线程运行
        exec();
    } catch (const std::exception &e) {
        qWarning() << "Exception in VideoSaveThread::run(): " << e.what();
    } catch (...) {
        qWarning() << "Unknown error occurred in VideoSaveThread::run().";
    }

    // 确保退出时释放资源
    if (videoWriter.isOpened()) {
        videoWriter.release();
        qDebug() << "Video writer released on thread exit.";
    }

    qDebug() << "Save thread exited cleanly.";
}

void VideoSaveThread::insertVideoInfoToDatabase() {
    try {
        // 获取当前时间
        QString currentDateTime = QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss");

        // 初始化 SerialGPS 对象并获取地理位置信息
        SerialGPS gpsReader;
        QString gpsData = "NULL"; // 默认值
        if (gpsReader.open()) {
            gpsData = gpsReader.getLastGPSData();
            gpsReader.close();
        } else {
            qWarning() << "Failed to initialize GPS reader, setting video address to NULL.";
        }

        // 填充视频信息
        VideoDao video;
        video.setVideoName(currentDateTime);
        video.setVideoAddress(gpsData); // 写入 GPS 地理位置信息
        video.setVideoDate(currentDateTime);
        video.setVideoUser("User");
        video.setVideoType(1);
        video.setVideoPath(outputFile);

        // 使用 QScopedPointer 管理 SaveVideoAndPictureService
        QScopedPointer<SaveVideoAndPictureService> saveVideoService(new SaveVideoAndPictureService);
        bool result = saveVideoService->insertVideoInfo(video);

        if (result) {
            qDebug() << "[DEBUG] Video information inserted successfully.";
        } else {
            qWarning() << "[DEBUG] Failed to insert video information.";
        }
    } catch (const std::exception &e) {
        qWarning() << "Exception in insertVideoInfoToDatabase(): " << e.what();
    } catch (...) {
        qWarning() << "Unknown error occurred in insertVideoInfoToDatabase().";
    }
}




void VideoSaveThread::saveFrame(const QImage &image) {
    if (stopFlag || !videoWriter.isOpened()) {
        return; // 安全退出
    }

    cv::Mat frame(image.height(), image.width(), CV_8UC3, (void*)image.bits(), image.bytesPerLine());
    cv::cvtColor(frame, frame, cv::COLOR_RGB2BGR);
    videoWriter.write(frame);
    // qDebug() << "Frame saved.";
}

