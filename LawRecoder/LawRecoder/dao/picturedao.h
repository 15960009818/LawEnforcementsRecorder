#ifndef PICTURE_H
#define PICTURE_H

#include <string>

class PictureDao {
public:
    // 构造函数
    PictureDao();
    PictureDao(int id, const std::string& name, const std::string& address,
            const std::string& date, const std::string& user,
            int type, const std::string& path);

    // Getter 和 Setter 方法
    int getPictureId() const;
    void setPictureId(int id);

    std::string getPictureName() const;
    void setPictureName(const std::string& name);

    std::string getPictureAddress() const;
    void setPictureAddress(const std::string& address);

    std::string getPictureDate() const;
    void setPictureDate(const std::string& date);

    std::string getPictureUser() const;
    void setPictureUser(const std::string& user);

    int getPictureType() const;
    void setPictureType(int type);

    std::string getPicturePath() const;
    void setPicturePath(const std::string& path);

private:
    int picture_id;
    std::string picture_name;
    std::string picture_address;
    std::string picture_date;
    std::string picture_user;
    int picture_type;
    std::string picture_path;
};

#endif // PICTURE_H
