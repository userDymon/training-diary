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
    bool insertIntoTable(Exercise &, QString, bool) override;
    bool insertIntoTable(Exercise &, QString, QString) override;
    bool selectFromTable(User &) override;
    bool haveUser(QString &) override;
    //bool haveGoalExercise(QString, int, int, int) override;
    //bool deleteGoalExercise(QString, QString, int, int, int) override;
    Exercise returnProgresExercise(const QString& exerciseName) override;

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
