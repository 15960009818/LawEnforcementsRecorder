#include"sqlite3crud.h"
Sqlite3Crud::Sqlite3Crud() : db(nullptr) {}

Sqlite3Crud::~Sqlite3Crud() {
    close();
}

bool Sqlite3Crud::open(const QString &dbName) {
    if (db) {
        qDebug() << "Database already open.";
        return false;
    }
    int rc = sqlite3_open(dbName.toStdString().c_str(), &db);
    if (rc) {
        qDebug() << "Can't open database: " << sqlite3_errmsg(db);
        return false;
    }
    return true;
}

void Sqlite3Crud::close() {
    if (db) {
        sqlite3_close(db);
        db = nullptr;
    }
}

bool Sqlite3Crud::execute(const QString &sql) {
    char *errMsg = nullptr;
    int rc = sqlite3_exec(db, sql.toStdString().c_str(), nullptr, nullptr, &errMsg);
    if (rc != SQLITE_OK) {
        qDebug() << "SQL error: " << errMsg;
        sqlite3_free(errMsg);
        return false;
    }
    return true;
}

bool Sqlite3Crud::query(const QString &sql, QVector<QVector<QString>> &result) {
    char *errMsg = nullptr;
    char **dbResult;
    int rows, cols;

    int rc = sqlite3_get_table(db, sql.toStdString().c_str(), &dbResult, &rows, &cols, &errMsg);
    if (rc != SQLITE_OK) {
        qDebug() << "SQL error: " << errMsg;
        sqlite3_free(errMsg);
        return false;
    }

    result.clear();
    for (int i = 0; i < rows; ++i) {
        QVector<QString> row;
        for (int j = 0; j < cols; ++j) {
            row.append(QString(dbResult[(i + 1) * cols + j]));
        }
        result.append(row);
    }

    sqlite3_free_table(dbResult);
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

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql.toStdString().c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        qDebug() << "SQL prepare error: " << sqlite3_errmsg(db);
        return false;
    }

    for (int i = 0; i < valueList.size(); ++i) {
        sqlite3_bind_text(stmt, i + 1, valueList[i].toStdString().c_str(), -1, SQLITE_STATIC);
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        qDebug() << "SQL execute error: " << sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
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

    sqlite3_stmt *stmt;
    int rc = sqlite3_prepare_v2(db, sql.toStdString().c_str(), -1, &stmt, nullptr);
    if (rc != SQLITE_OK) {
        qDebug() << "SQL prepare error: " << sqlite3_errmsg(db);
        return false;
    }

    for (int i = 0; i < valueList.size(); ++i) {
        sqlite3_bind_text(stmt, i + 1, valueList[i].toStdString().c_str(), -1, SQLITE_STATIC);
    }

    rc = sqlite3_step(stmt);
    if (rc != SQLITE_DONE) {
        qDebug() << "SQL execute error: " << sqlite3_errmsg(db);
        sqlite3_finalize(stmt);
        return false;
    }

    sqlite3_finalize(stmt);
    return true;
}

bool Sqlite3Crud::remove(const QString &tableName, const QString &whereClause) {
    QString sql = QString("DELETE FROM %1 WHERE %2")
                  .arg(tableName)
                  .arg(whereClause);

    return execute(sql);
}
