#ifndef DBMANAGER_H

#include <QSqlDatabase>
#include <QString>
#include <QDate>

#include "user.h"
#include "exercise.h"

class DBManager
{
public:
    virtual void connectToDataBase() = 0;
    virtual QSqlDatabase getDB() = 0;
    virtual bool insertIntoTable(User &) = 0;
    virtual bool insertIntoTable(Exercise &, QString) = 0;
    virtual bool insertIntoTable(Exercise &, QString, bool) = 0;
    virtual bool insertIntoTable(Exercise &, QString, QString) = 0;
    virtual bool selectFromTable(User &) = 0;
    virtual bool haveUser(QString &) = 0;
    //virtual bool haveGoalExercise(QString, int, int, int);
    //virtual bool deleteGoalExercise(QString, QString, int, int, int);
    virtual Exercise returnProgresExercise(const QString& exerciseName) = 0;
};
#define DBMANAGER_H

#endif // DBMANAGER_H
