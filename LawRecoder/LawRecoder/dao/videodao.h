#ifndef VIDEODAO_H
#define VIDEODAO_H

#include <QString>
#include <QDate>

class VideoDao {
public:
    VideoDao();

    // Getter 和 Setter
    int getVideoId() const;
    void setVideoId(int id);

    QString getVideoName() const;
    void setVideoName(const QString &name);

    QString getVideoAddress() const;
    void setVideoAddress(const QString &address);

    QString getVideoDate() const;
    void setVideoDate(const QString &date);

    QString getVideoUser() const;
    void setVideoUser(const QString &user);

    int getVideoType() const;
    void setVideoType(int type);

    QString getVideoPath() const;
    void setVideoPath(const QString &path);

    // 重载操作符==
    bool operator==(const VideoDao &other) const {
        return videoId == other.videoId &&
               videoName == other.videoName &&
               videoAddress == other.videoAddress &&
               videoDate == other.videoDate &&
               videoUser == other.videoUser &&
               videoType == other.videoType &&
               videoPath == other.videoPath;
    }
private:
    int videoId;            // 视频ID
    QString videoName;      // 视频名称
    QString videoAddress;   // 视频地址
    QString videoDate;        // 视频日期
    QString videoUser;      // 用户名
    int videoType;          // 视频类型
    QString videoPath;      // 视频文件路径
};

#endif // VIDEODAO_H
