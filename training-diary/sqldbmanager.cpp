#include "sqldbmanager.h"

#include <QDate>
#include <QDateTime>
#include <QDebug>
#include <QFile>
#include <QObject>
#include <QSqlError>
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QMessageBox>

SqlDBManager* SqlDBManager::instance = nullptr;

SqlDBManager::SqlDBManager()
{
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOST_NAME);
    db.setDatabaseName(DATABASE_FILE_NAME);
}

SqlDBManager *SqlDBManager::getInstance()
{
    if (instance == nullptr) {
        instance = new SqlDBManager();
    }
    return instance;
}

void SqlDBManager::connectToDataBase()
{
    if (QFile(DATABASE_FILE_NAME).exists()) {
        this->openDataBase();
    } else {
        this->restoreDataBase();
    }
}

QSqlDatabase SqlDBManager::getDB()
{
    return db;
}

bool SqlDBManager::openDataBase()
{
    if (db.open()) {
        return true;
    } else
        return false;
}
bool SqlDBManager::restoreDataBase()
{
    if (this->openDataBase()) {
        if (!this->createTables()) {
            return false;
        } else {
            return true;
        }
    } else {
        qCritical() << "Cannot restore database";
        return false;
    }
}
void SqlDBManager::closeDataBase()
{
    db.close();
}

bool SqlDBManager::createTables()
{
    QSqlQuery query;

    if (!query.exec("\
                    CREATE TABLE users(\
                        id INTEGER PRIMARY KEY AUTOINCREMENT,\
                        login VARCHAR(255) NOT NULL,\
                        password VARCHAR(255) NOT NULL,\
                        UNIQUE(login)\
                        )\
    ")) {
        qDebug() << "DataBase: error of create table users";
        qDebug() << query.lastError().text();
        return false;
    }

    if(!query.exec("\
                    CREATE TABLE exercises (\
                        user_id INTEGER,\
                        name VARCHAR(255) NOT NULL,\
                        weight INTEGER NOT NULL,\
                        sets INTEGER NOT NULL,\
                        reps INTEGER NOT NULL,\
                        date DATE,\
                        FOREIGN KEY (user_id) REFERENCES users(id)\
                        )\
    ")) {
        qDebug() << "DataBase: error of create table exercise";
        qDebug() << query.lastError().text();
        return false;
    }

    if(!query.exec("\
                    CREATE TABLE scheduleExercises (\
                        user_id INTEGER,\
                        name VARCHAR(255) NOT NULL,\
                        weight INTEGER NOT NULL,\
                        sets INTEGER NOT NULL,\
                        reps INTEGER NOT NULL,\
                        dayOfWeek VARCHAR(10) NOT NULL,\
                        FOREIGN KEY (user_id) REFERENCES users(id)\
                        )\
    ")) {
        qDebug() << "DataBase: error of create table exercise";
        qDebug() << query.lastError().text();
        return false;
    }

    if(!query.exec("\
                    CREATE TABLE goals (\
                        user_id INTEGER,\
                        name VARCHAR(255) NOT NULL,\
                        weight INTEGER NOT NULL,\
                        sets INTEGER NOT NULL,\
                        reps INTEGER NOT NULL,\
                        isGoal BOOLEAN,\
                        FOREIGN KEY (user_id) REFERENCES users(id)\
                        )\
    ")) {
        qDebug() << "DataBase: error of create table exercise";
        qDebug() << query.lastError().text();
        return false;
    }
    if (!query.exec("\
                    CREATE TABLE userAutolog(\
                        user_id INTEGER NOT NULL,\
                        login VARCHAR(255) NOT NULL,\
                        password VARCHAR(255) NOT NULL\
                        )\
    ")) {
        qDebug() << "DataBase: error of create table userAutolog";
        qDebug() << query.lastError().text();
        return false;
    }else {
        return true;
    }
}

bool SqlDBManager::insertIntoTable(User &user)
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO users(login, password)\
        VALUES(:login, :password)");
    query.bindValue(":login", user.getLogin());
    query.bindValue(":password", user.getPassword());

    if (!query.exec()) {
        qDebug() << "error insert into table users";
        qDebug() << query.lastError().text();
        qDebug() << query.lastQuery();

        return false;
    } else
        return true;
}

bool SqlDBManager::insertIntoTable(Exercise &exercise, QString login)
{
    QSqlQuery query;
    QDate currentDate = QDate::currentDate();

    query.prepare(
        "INSERT INTO exercises (user_id, name, weight, sets, reps, date)\
        VALUES((SELECT id FROM users WHERE login = :login), :name, :weight, :sets, :reps, :date)");
    query.bindValue(":login", login);
    query.bindValue(":name", exercise.getName());
    query.bindValue(":weight", exercise.getWeight());
    query.bindValue(":sets", exercise.getSets());
    query.bindValue(":reps", exercise.getReps());
    query.bindValue(":date", currentDate);

    if (!query.exec()) {
        qDebug() << "error insert into table exercises";
        qDebug() << query.lastError().text();
        qDebug() << query.lastQuery();

        return false;
    } else
        return true;
}

bool SqlDBManager::insertIntoTable(Exercise &exercise, QString login, bool isGoal)
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO goals (user_id, name, weight, sets, reps, isGoal)\
        VALUES((SELECT id FROM users WHERE login = :login), :name, :weight, :sets, :reps, :isGoal)");
    query.bindValue(":login", login);
    query.bindValue(":name", exercise.getName());
    query.bindValue(":weight", exercise.getWeight());
    query.bindValue(":sets", exercise.getSets());
    query.bindValue(":reps", exercise.getReps());
    query.bindValue(":isGoal", isGoal);

    if (!query.exec()) {
        qDebug() << "error insert into table exercises";
        qDebug() << query.lastError().text();
        qDebug() << query.lastQuery();

        return false;
    } else
        return true;
}

bool SqlDBManager::insertIntoTable(Exercise &exercise, QString login, QString dayOfWeek)
{
    QSqlQuery query;

    query.prepare(
        "INSERT INTO scheduleExercises (user_id, name, weight, sets, reps, dayOfWeek)\
        VALUES((SELECT id FROM users WHERE login = :login), :name, :weight, :sets, :reps, :dayOfWeek)");
    query.bindValue(":login", login);
    query.bindValue(":name", exercise.getName());
    query.bindValue(":weight", exercise.getWeight());
    query.bindValue(":sets", exercise.getSets());
    query.bindValue(":reps", exercise.getReps());
    query.bindValue(":dayOfWeek", dayOfWeek);

    if (!query.exec()) {
        qDebug() << "error insert into table exercises";
        qDebug() << query.lastError().text();
        qDebug() << query.lastQuery();

        return false;
    } else
        return true;
}

bool SqlDBManager::selectFromTable(User &user) {
    QSqlQuery query;

    query.prepare("SELECT * FROM users WHERE LOWER(login) = LOWER(:login) AND password = :password;");
    query.bindValue(":login", user.getLogin());
    query.bindValue(":password", user.getPassword());

    if (!query.exec()) {
        qDebug() << "Error selecting from the users table:";
        qDebug() << query.lastError().text();
        qDebug() << query.lastQuery();
        return false;
    }

    if (query.next()) {
        QString storedLogin = query.value("login").toString();
        QString storedPasswordHash = query.value("password").toString();

        if (user.getLogin() == storedLogin && user.getPassword() == storedPasswordHash) {
            qDebug() << "User exists and credentials match.";
            return true;
        } else {
            qDebug() << "User exists, but credentials do not match.";
            return false;
        }
    }

    qDebug() << "User not found.";
    return false;
}

bool SqlDBManager::haveUser(QString & login){
    QSqlQuery query;

    query.prepare("SELECT login FROM users WHERE LOWER(login) = LOWER(:login)");
    query.bindValue(":login", login);

    if (!query.exec()) {
        qDebug() << "Error selecting from the users table:";
        qDebug() << query.lastError().text();
        qDebug() << query.lastQuery();
        return false;
    }

    if (query.next()) {
        QString storedLogin = query.value("login").toString();

        if (login == storedLogin) {
            qDebug() << "User exists and credentials match.";
            return true;
        } else {
            qDebug() << "User exists, but credentials do not match.";
            return false;
        }
    }

    qDebug() << "User not found.";
    return false;
}


bool SqlDBManager::haveGoalExercise(QString login, QString name, int weight, int sets, int reps, bool isBigger) {
    QSqlQuery query;

    if (isBigger) {
        query.prepare("SELECT * FROM goals WHERE user_id = (SELECT id FROM users WHERE login = :login) AND name = :name AND weight <= :weight AND sets <= :sets AND reps <= :reps ");
    } else {
        query.prepare("SELECT * FROM goals WHERE user_id = (SELECT id FROM users WHERE login = :login) AND name = :name AND weight >= :weight AND sets >= :sets AND reps >= :reps ");
    }

    query.bindValue(":login", login);
    query.bindValue(":name", name);
    query.bindValue(":weight", weight);
    query.bindValue(":sets", sets);
    query.bindValue(":reps", reps);

    if (!query.exec()) {
        qDebug() << "Error selecting from the goals table:";
        qDebug() << query.lastError().text();
        qDebug() << query.lastQuery();
        return false;
    }

    if(!isBigger){
        if(query.next()){
            return true;
        }
        QSqlQuery query2;
        query2.prepare("SELECT * FROM exercises WHERE user_id = (SELECT id FROM users WHERE login = :login) AND name = :name AND weight >= :weight AND sets >= :sets AND reps >= :reps ");
        query2.bindValue(":login", login);
        query2.bindValue(":name", name);
        query2.bindValue(":weight", weight);
        query2.bindValue(":sets", sets);
        query2.bindValue(":reps", reps);

        if (query2.exec() && query2.next()) {
            return true;
        } else {
            qDebug() << "Record not found or error executing the query.";
            return false;
        }
    }

    return query.next();

}


bool SqlDBManager::deleteGoalExercise(QString login, QString name, int weight, int sets, int reps) {
    QSqlQuery query;

    query.prepare("DELETE FROM goals WHERE user_id = (SELECT id FROM users WHERE login = :login) AND name = :name AND weight <= :weight AND sets <= :sets AND reps <= :reps ");
    query.bindValue(":login", login);
    query.bindValue(":name", name);
    query.bindValue(":weight", weight);
    query.bindValue(":sets", sets);
    query.bindValue(":reps", reps);

    if (!query.exec()) {
        qDebug() << "Error deleting from the goals table:";
        qDebug() << query.lastError().text();
        qDebug() << query.lastQuery();
        return false;
    }

    return true;
}

Exercise SqlDBManager::returnProgresExercise(const QString& exerciseName) {
    Exercise defaultExercise(exerciseName, 0, 0, 0);
    // Знайдіть останній запис за іменем вправи
    QString findLastValueQuery = "SELECT weight, sets, reps FROM exercises "
                                 "WHERE name = :exerciseName "
                                 "ORDER BY date DESC "
                                 "LIMIT 1;";

    QSqlQuery findLastValue;
    findLastValue.prepare(findLastValueQuery);
    findLastValue.bindValue(":exerciseName", exerciseName);

    if (findLastValue.exec() && findLastValue.next()) {
        // Отримайте останнє значення
        int lastWeight = findLastValue.value("weight").toInt();
        int lastSets = findLastValue.value("sets").toInt();
        int lastReps = findLastValue.value("reps").toInt();

        // Знайдіть передостанній запис за іменем вправи по даті
        QString findSecondLastValueQuery = "SELECT weight, sets, reps FROM exercises "
                                           "WHERE name = :exerciseName "
                                           "ORDER BY date DESC "
                                           "LIMIT 1 OFFSET 1;";

        QSqlQuery findSecondLastValue;
        findSecondLastValue.prepare(findSecondLastValueQuery);
        findSecondLastValue.bindValue(":exerciseName", exerciseName);

        if (findSecondLastValue.exec() && findSecondLastValue.next()) {
            // Отримайте передостаннє значення
            int secondLastWeight = findSecondLastValue.value("weight").toInt();
            int secondLastSets = findSecondLastValue.value("sets").toInt();
            int secondLastReps = findSecondLastValue.value("reps").toInt();

            // Збільште передостанні значення на 1 та вставте новий запис
            int progresWeight = lastWeight - secondLastWeight;
            int progresSets = lastSets - secondLastSets;
            int progresReps = lastReps - secondLastReps;

            Exercise resultExercise(exerciseName, progresWeight, progresSets, progresReps);
            return resultExercise;

        } else {
            qDebug() << "Failed to find second last value:" << findSecondLastValue.lastError().text();
        }
    } else {
        qDebug() << "Failed to find last value:" << findLastValue.lastError().text();
    }

    return defaultExercise;
}

bool SqlDBManager::autoLog(User &user){
    QSqlQuery query;

    query.prepare(
        "INSERT INTO userAutolog(user_id, login, password)\
        VALUES((SELECT id FROM users WHERE login = :login), :login, :password)");
        query.bindValue(":login", user.getLogin());
        query.bindValue(":password", user.getPassword());

        if (!query.exec()) {
            qDebug() << "error insert into table userAutolog";
            qDebug() << query.lastError().text();
            qDebug() << query.lastQuery();

            return false;
        } else
        return true;
}

bool SqlDBManager::hasSavedCredentials() {
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM userAutolog");
    if (query.exec() && query.next()) {
        int rowCount = query.value(0).toInt();
        return rowCount > 0;
    } else {
        qDebug() << "Error checking saved credentials:";
        qDebug() << query.lastError().text();
        qDebug() << query.lastQuery();
        return false;
    }
}

User SqlDBManager::returnSavedCredentials() {
    QSqlQuery query;
    query.prepare("SELECT login, password FROM userAutolog");

    if (query.exec() && query.next()) {
        QString savedLogin = query.value("login").toString();
        QString savedPassword = query.value("password").toString();
        // Створюємо та повертаємо об'єкт User
        return User(savedLogin, savedPassword);
    }
}

bool SqlDBManager::clearAutologinTable() {
    QSqlQuery query;
    query.prepare("DELETE FROM userAutolog");

    if (query.exec()) {
        qDebug() << "Autologin data cleared successfully.";
        return true;
    } else {
        qDebug() << "Error clearing autologin data:";
        qDebug() << query.lastError().text();
        qDebug() << query.lastQuery();
        return false;
    }
}

bool SqlDBManager::updateLogin(const QString& oldLogin, const QString& newLogin) {
    QSqlQuery query;
    query.prepare("UPDATE users SET login = :newLogin WHERE login = :oldLogin");
    query.bindValue(":newLogin", newLogin);
    query.bindValue(":oldLogin", oldLogin);

    if (query.exec()) {
        qDebug() << "Login updated successfully.";
        return true;
    } else {
        qDebug() << "Error updating login:";
        qDebug() << query.lastError().text();
        qDebug() << query.lastQuery();
        return false;
    }
}

bool SqlDBManager::updatePassword(const QString& login, const QString& newPassword) {
    QSqlQuery query;
    query.prepare("UPDATE users SET password = :newPassword WHERE login = :login");
    query.bindValue(":newPassword", newPassword);
    query.bindValue(":login", login);

    if (query.exec()) {
        qDebug() << "Password updated successfully.";
        return true;
    } else {
        qDebug() << "Error updating password:";
        qDebug() << query.lastError().text();
        qDebug() << query.lastQuery();
        return false;
    }
}



