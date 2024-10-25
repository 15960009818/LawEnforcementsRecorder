#include "videocapturecontroller.h"
#include "../common/commondefs.h"
#include <QMetaType>
#include <QDebug>

VideoCaptureController::VideoCaptureController() {
    initController();
}

VideoCaptureController::~VideoCaptureController() {
    // 确保线程被正确清理
    if (threadGetVideoMessage) {
        threadGetVideoMessage->quit();
        threadGetVideoMessage->wait();
    }
}

void VideoCaptureController::initController() {
    qRegisterMetaType<QList<VideoDao>>("QList<VideoDao>");

    connect(this,SIGNAL(getSignal(QDate,int)),this,SLOT(getVideoMessageSlot(QDate,int)));
    connect(this, SIGNAL(finishedVideoQuerySignal(int, const QList<VideoDao>&)),
            this, SLOT(sendVideoMessageSlot(int, const QList<VideoDao>&)));
}

void VideoCaptureController::getVideoMessageSlot(QDate videoDate, int row) {
    qDebug() << "[DEBUG] getVideoMessage called.";
    qDebug() << "[DEBUG] VideoDate:" << videoDate;

    // 首先判断日期是否有效
    if (!videoDate.isValid()) {
        qDebug() << "[ERROR] 输入的日期无效";
        emit finishedVideoQuerySignal(SQLERROR, QList<VideoDao>()); // 发送错误状态和空列表
        return;
    }

    // 初始化线程池
    threadGetVideoMessage = new QThread;
    videoCaptureService = new VideoCaptureService();

    // 将 VideoCaptureService 移动到线程中
    videoCaptureService->moveToThread(threadGetVideoMessage);

    // 线程启动后调用 VideoCaptureService 的槽函数来处理获取视频信息
    connect(threadGetVideoMessage, &QThread::started, videoCaptureService, [=]() {
        videoCaptureService->GetDateVideoSlots(videoDate, row);
        qDebug() << "[DEBUG] VideoCaptureService started with videoDate:" << videoDate;
    });

    // 处理获取到的视频信息并发出信号
    connect(videoCaptureService, &VideoCaptureService::videoListReady, this, [=](const QList<VideoDao> &videoList) {
        emit finishedVideoQuerySignal(SQLSUCCESS, videoList); // 发送成功状态和视频列表
        threadGetVideoMessage->quit(); // 停止线程
    });

    // 处理服务类出错的情况
    connect(videoCaptureService, &VideoCaptureService::errorOccurred, this, [=](int errorCode) {
        emit finishedVideoQuerySignal(errorCode, QList<VideoDao>()); // 发送错误状态和空列表
        threadGetVideoMessage->quit(); // 停止线程
    });

    // 线程完成时销毁，先销毁线程再通知 UI
    connect(threadGetVideoMessage, &QThread::finished, threadGetVideoMessage, &QObject::deleteLater);
    connect(threadGetVideoMessage, &QThread::finished, videoCaptureService, &QObject::deleteLater);

    // 启动线程
    qDebug() << "[DEBUG] Starting threadGetVideoMessage...";
    threadGetVideoMessage->start();
}

void VideoCaptureController::sendVideoMessageSlot(int SQLRESULT, const QList<VideoDao> &videoMessage) {
    qDebug() << "[DEBUG] Video query process finished, SQL result:" << SQLRESULT;

    QString uiMessage;

    // 根据 SQLRESULT 解析不同的消息
    switch (SQLRESULT) {
        case SQLERROR:
            uiMessage = "数据库错误，无法获取视频信息。";
            break;
        case SQLSUCCESS:
            if (videoMessage.isEmpty()) {
                uiMessage = "没有找到任何视频记录。";
            } else {
                uiMessage = "视频信息获取成功，共找到 " + QString::number(videoMessage.size()) + " 条记录。";
            }
            break;
        case SQLGETERROR:
            uiMessage = "查询失败，可能是查询条件错误。";
            break;
        default:
            uiMessage = "发生未知错误。";
            break;
    }

    // 发送信号给UI，通知视频查询结果
    emit finishVideoSignal(uiMessage, videoMessage);
    qDebug() << "[DEBUG] emit finishedVideoQuerySignal:" << uiMessage;
}

QDate VideoCaptureController::getCurrentDate() {
    return QDate::currentDate(); // 获取并返回当天的日期
}
