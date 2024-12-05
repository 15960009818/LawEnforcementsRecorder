#ifndef SQLITE3CRUD_H
#define SQLITE3CRUD_H

#include <QVector>
#include <QMap>
#include <QString>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>

class Sqlite3Crud {
public:
    Sqlite3Crud();
    ~Sqlite3Crud();

    bool open(const QString &dbName);
    void close();
    bool execute(const QString &sql);
    bool query(const QString &sql, QVector<QVector<QString>> &result);
    bool insert(const QString &tableName, const QMap<QString, QString> &values);
    bool update(const QString &tableName, const QMap<QString, QString> &values, const QString &whereClause);
    bool remove(const QString &tableName, const QString &whereClause);

private:
    QSqlDatabase db; // 使用 QSqlDatabase 代替原来的 sqlite3*
};

#endif // SQLITE3CRUD_H
