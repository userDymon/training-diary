#ifndef USER_H
#define USER_H

#include <QString>

class User {
public:
    User(const QString& login, const QString& password);

    QString getLogin() const;
    QString getPassword() const;

private:
    QString login;
    QString password;
};

#endif // USER_H
