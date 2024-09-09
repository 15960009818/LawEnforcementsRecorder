#include <QDebug>
#include <QString>
#include "../sqlite3.h"
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
    sqlite3 *db;
};
