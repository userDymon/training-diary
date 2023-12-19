#include "user.h"

User::User(const QString& login, const QString& password)
    : login(login), password(password) {}

QString User::getLogin() const {
    return login;
}

QString User::getPassword() const {
    return password;
}

void User::setLogin(QString login){
    this->login = login;
}

void User::setPassword(QString password){
    this->password = password;
}
