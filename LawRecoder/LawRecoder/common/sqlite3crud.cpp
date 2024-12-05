#include "sqlite3crud.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

Sqlite3Crud::Sqlite3Crud() {}

Sqlite3Crud::~Sqlite3Crud() {
    close();
}

bool Sqlite3Crud::open(const QString &dbName) {
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(dbName);

    if (!db.open()) {
        qDebug() << "Can't open database: " << db.lastError().text();
        return false;
    }
    return true;
}

void Sqlite3Crud::close() {
    QSqlDatabase db = QSqlDatabase::database();
    if (db.isOpen()) {
        db.close();
    }
}

bool Sqlite3Crud::execute(const QString &sql) {
    QSqlQuery query;
    if (!query.exec(sql)) {
        qDebug() << "SQL error: " << query.lastError().text();
        return false;
    }
    return true;
}

bool Sqlite3Crud::query(const QString &sql, QVector<QVector<QString>> &result) {
    QSqlQuery query;
    if (!query.exec(sql)) {
        qDebug() << "SQL error: " << query.lastError().text();
        return false;
    }

    result.clear();
    while (query.next()) {
        QVector<QString> row;
        for (int i = 0; i < query.record().count(); ++i) {
            row.append(query.value(i).toString());
        }
        result.append(row);
    }
    return true;
}

bool Sqlite3Crud::insert(const QString &tableName, const QMap<QString, QString> &values) {
    QStringList columns, valuePlaceholders;
    QStringList valueList;

    for (auto it = values.begin(); it != values.end(); ++it) {
        columns.append(it.key());
        valuePlaceholders.append("?");
        valueList.append(it.value());
    }

    QString sql = QString("INSERT INTO %1 (%2) VALUES (%3)")
                  .arg(tableName)
                  .arg(columns.join(", "))
                  .arg(valuePlaceholders.join(", "));

    QSqlQuery query;
    query.prepare(sql);

    for (int i = 0; i < valueList.size(); ++i) {
        query.bindValue(i, valueList[i]);
    }

    if (!query.exec()) {
        qDebug() << "SQL execute error: " << query.lastError().text();
        return false;
    }
    return true;
}

bool Sqlite3Crud::update(const QString &tableName, const QMap<QString, QString> &values, const QString &whereClause) {
    QStringList assignments;
    QStringList valueList;

    for (auto it = values.begin(); it != values.end(); ++it) {
        assignments.append(QString("%1 = ?").arg(it.key()));
        valueList.append(it.value());
    }

    QString sql = QString("UPDATE %1 SET %2 WHERE %3")
                  .arg(tableName)
                  .arg(assignments.join(", "))
                  .arg(whereClause);

    QSqlQuery query;
    query.prepare(sql);

    for (int i = 0; i < valueList.size(); ++i) {
        query.bindValue(i, valueList[i]);
    }

    if (!query.exec()) {
        qDebug() << "SQL execute error: " << query.lastError().text();
        return false;
    }
    return true;
}

bool Sqlite3Crud::remove(const QString &tableName, const QString &whereClause) {
    QString sql = QString("DELETE FROM %1 WHERE %2")
                  .arg(tableName)
                  .arg(whereClause);

    return execute(sql);
}
