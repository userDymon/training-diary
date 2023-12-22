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
    virtual bool insertIntoGoals(Exercise &, QString) = 0;
    virtual bool insertIntoTable(Exercise &, QString, QString) = 0;
    virtual bool selectFromTable(User &) = 0;
    virtual bool haveUser(QString &) = 0;
    virtual bool haveGoalExercise(QString, QString, int, int, int, bool) = 0;
    virtual bool deleteGoalExercise(QString, QString, int, int, int) = 0;
    virtual Exercise returnProgresExercise(const QString& exerciseName) = 0;
    virtual bool autoLog(User &) = 0;
    virtual bool hasSavedCredentials() = 0;
    virtual User returnSavedCredentials() = 0;
    virtual int returnId(User *user) = 0;
    virtual bool clearAutologinTable() = 0;
    // Функція для зміни логіна в базі даних
    virtual bool updateLogin(const QString& oldLogin, const QString& newLogin) = 0;
    // Функція для зміни паролю в базі даних
    virtual bool updatePassword(const QString& login, const QString& newPassword) = 0;

};
#define DBMANAGER_H

#endif // DBMANAGER_H
