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
                        login VARCHAR(255) NOT NULL,\
                        password VARACHAR(255) NOT NULL\
                        )\
    ")) {
        qDebug() << "DataBase: error of create table users";
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


