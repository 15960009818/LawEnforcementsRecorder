#include "picturedao.h"

// 构造函数实现
PictureDao::PictureDao() : picture_id(0), picture_type(0) {}

PictureDao::PictureDao(int id, const std::string& name, const std::string& address,
                 const std::string& date, const std::string& user,
                 int type, const std::string& path)
    : picture_id(id), picture_name(name), picture_address(address),
      picture_date(date), picture_user(user), picture_type(type), picture_path(path) {}

// Getter 和 Setter 实现
int PictureDao::getPictureId() const { return picture_id; }
void PictureDao::setPictureId(int id) { picture_id = id; }

std::string PictureDao::getPictureName() const { return picture_name; }
void PictureDao::setPictureName(const std::string& name) { picture_name = name; }

std::string PictureDao::getPictureAddress() const { return picture_address; }
void PictureDao::setPictureAddress(const std::string& address) { picture_address = address; }

std::string PictureDao::getPictureDate() const { return picture_date; }
void PictureDao::setPictureDate(const std::string& date) { picture_date = date; }

std::string PictureDao::getPictureUser() const { return picture_user; }
void PictureDao::setPictureUser(const std::string& user) { picture_user = user; }

int PictureDao::getPictureType() const { return picture_type; }
void PictureDao::setPictureType(int type) { picture_type = type; }

std::string PictureDao::getPicturePath() const { return picture_path; }
void PictureDao::setPicturePath(const std::string& path) { picture_path = path; }
