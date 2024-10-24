#ifndef PICTURE_H
#define PICTURE_H

#include<QString>

class PictureDao {
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
