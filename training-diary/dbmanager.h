#ifndef DBMANAGER_H

#include <QSqlDatabase>

#include "user.h"

class DBManager
{
public:
    virtual void connectToDataBase() = 0;
    virtual QSqlDatabase getDB() = 0;
    virtual bool insertIntoTable(User &) = 0;
    virtual bool selectFromTable(User &) = 0;
};
#define DBMANAGER_H

#endif // DBMANAGER_H
