#include "videodao.h"

VideoDao::VideoDao() : videoId(0), videoType(0) {}

int VideoDao::getVideoId() const {
    return videoId;
}

void VideoDao::setVideoId(int id) {
    videoId = id;
}

QString VideoDao::getVideoName() const {
    return videoName;
}

void VideoDao::setVideoName(const QString &name) {
    videoName = name;
}

QString VideoDao::getVideoAddress() const {
    return videoAddress;
}

void VideoDao::setVideoAddress(const QString &address) {
    videoAddress = address;
}

QString VideoDao::getVideoDate() const {
    return videoDate;
}

void VideoDao::setVideoDate(const QString &date) {
    videoDate = date;
}

QString VideoDao::getVideoUser() const {
    return videoUser;
}

void VideoDao::setVideoUser(const QString &user) {
    videoUser = user;
}

int VideoDao::getVideoType() const {
    return videoType;
}

void VideoDao::setVideoType(int type) {
    videoType = type;
}

QString VideoDao::getVideoPath() const {
    return videoPath;
}

void VideoDao::setVideoPath(const QString &path) {
    videoPath = path;
}
