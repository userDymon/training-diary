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
    bool insertIntoGoals(Exercise &, QString) override;
    bool insertIntoTable(Exercise &, QString, QString) override;
    bool selectFromTable(User &) override;
    bool haveUser(QString &) override;
    bool haveGoalExercise(QString, QString, int, int, int, bool) override;
    bool deleteGoalExercise(QString, QString, int, int, int) override;
    Exercise returnProgresExercise(const QString& exerciseName) override;
    bool autoLog(User &) override;
    bool hasSavedCredentials() override;
    User returnSavedCredentials() override;
    int returnId(User *user) override;
    bool clearAutologinTable() override;
    // Функція для зміни логіна в базі даних
    bool updateLogin(const QString& oldLogin, const QString& newLogin) override;
    // Функція для зміни паролю в базі даних
    bool updatePassword(const QString& login, const QString& newPassword) override;


private:
    QSqlDatabase db;

    static SqlDBManager *instance;

    SqlDBManager();

    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createTables();
    bool createTableUsers();
    bool createTableExercise();
    bool createTableScheduleExercises();
    bool createTableGoal();
    bool createTableAutolog();
};

#endif // SQLDBMANAGER_H
