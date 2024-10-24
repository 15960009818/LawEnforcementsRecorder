#include "picturedao.h"

// 构造函数实现
PictureDao::PictureDao() : picture_id(0), picture_type(0) {}

PictureDao::PictureDao(int id, const QString& name, const QString& address,
                 const QString& date, const QString& user,
                 int type, const QString& path)
    : picture_id(id), picture_name(name), picture_address(address),
      picture_date(date), picture_user(user), picture_type(type), picture_path(path) {}

// Getter 和 Setter 实现
int PictureDao::getPictureId() const { return picture_id; }
void PictureDao::setPictureId(int id) { picture_id = id; }

QString PictureDao::getPictureName() const { return picture_name; }
void PictureDao::setPictureName(const QString& name) { picture_name = name; }

QString PictureDao::getPictureAddress() const { return picture_address; }
void PictureDao::setPictureAddress(const QString& address) { picture_address = address; }

QString PictureDao::getPictureDate() const { return picture_date; }
void PictureDao::setPictureDate(const QString& date) { picture_date = date; }

QString PictureDao::getPictureUser() const { return picture_user; }
void PictureDao::setPictureUser(const QString& user) { picture_user = user; }

int PictureDao::getPictureType() const { return picture_type; }
void PictureDao::setPictureType(int type) { picture_type = type; }

QString PictureDao::getPicturePath() const { return picture_path; }
void PictureDao::setPicturePath(const QString& path) { picture_path = path; }
