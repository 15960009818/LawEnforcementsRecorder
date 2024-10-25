#ifndef PICTURE_H
#define PICTURE_H

#include<QString>
#include<QObject>

class PictureDao
{

public:
    // 构造函数
    PictureDao();
    PictureDao(int id, const QString& name, const QString& address,
            const QString& date, const QString& user,
            int type, const QString& path);

    // Getter 和 Setter 方法
    int getPictureId() const;
    void setPictureId(int id);

    QString getPictureName() const;
    void setPictureName(const QString& name);

    QString getPictureAddress() const;
    void setPictureAddress(const QString& address);

    QString getPictureDate() const;
    void setPictureDate(const QString& date);

    QString getPictureUser() const;
    void setPictureUser(const QString& user);

    int getPictureType() const;
    void setPictureType(int type);

    QString getPicturePath() const;
    void setPicturePath(const QString& path);

    // 重载操作符==
    bool operator==(const PictureDao &other) const {
        return picture_id == other.picture_id &&
               picture_name == other.picture_name &&
               picture_address == other.picture_address &&
               picture_date == other.picture_date &&
               picture_user == other.picture_user &&
               picture_type == other.picture_type &&
               picture_path == other.picture_path;
    }

private:
    int picture_id;
    QString picture_name;
    QString picture_address;
    QString picture_date;
    QString picture_user;
    int picture_type;
    QString picture_path;
};



#endif // PICTURE_H
