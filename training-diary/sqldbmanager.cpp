#include "sqldbmanager.h"

#include <QDate>
#include <QDebug>
#include <QFile>
#include <QObject>
#include <QSqlError>
#include <QSqlQuery>
#include <QCryptographicHash>

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
                        date_time TIMESTAMP DEFAULT CURRENT_TIMESTAMP,\
                        FOREIGN KEY (user_id) REFERENCES users(id)\
                        )\
    ")) {
        qDebug() << "DataBase: error of create table exercise";
        qDebug() << query.lastError().text();
        return false;
    } else {
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

    query.prepare(
        "INSERT INTO exercises (user_id, name, weight, sets, reps)\
        VALUES((SELECT id FROM users WHERE login = :login), :name, :weight, :sets, :reps)");
    query.bindValue(":login", login);
    query.bindValue(":name", exercise.getName());
    query.bindValue(":weight", exercise.getWeight());
    query.bindValue(":sets", exercise.getSets());
    query.bindValue(":reps", exercise.getReps());

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


