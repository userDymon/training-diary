#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>
#include <QLabel>

#include "dbmanager.h"
#include "signup.h"
#include "user.h"

namespace Ui {
class Login;
}

class Login : public QDialog
{
    Q_OBJECT

public:
    explicit Login(DBManager* dbManager, QWidget *parent = nullptr);
    ~Login();

signals:
    void signInSuccess();
    void signInFailed(const QString& error);

private slots:
    void attemptSignIn(const QString& login, const QString& password);
    void on_submitPB_clicked();

    void on_signUp_linkActivated(const QString &link);

    void onSignInSuccess();
    void onSignInFailed();

private:
    Ui::Login *ui;
    DBManager* dbManager;
    SignUp *signUp;
    User *user;
};

#endif // LOGIN_H
