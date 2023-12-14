#ifndef SQLDBMANAGER_H
#define SQLDBMANAGER_H


#include <QSqlDatabase>
#include <QString>
#include <QDate>

#include "dbmanager.h"
#include "user.h"

#define DATABASE_HOST_NAME "TrainingDataBase"
#define DATABASE_FILE_NAME "DataBase.sqlite"

class SqlDBManager : public DBManager
{
public:
    static SqlDBManager *getInstance();

    void connectToDataBase() override;
    QSqlDatabase getDB() override;
    bool insertIntoTable(User &) override;
    bool insertIntoTable(Exercise &, QString) override;
    bool insertIntoTable(Exercise &, QString, QString) override;
    bool selectFromTable(User &) override;
    bool haveUser(QString &) override;

private:
    QSqlDatabase db;

    static SqlDBManager *instance;

    SqlDBManager();

    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createTables();
};

#endif // SQLDBMANAGER_H
