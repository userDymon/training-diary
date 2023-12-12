#ifndef LOGIN_H
#define LOGIN_H

#include <QDialog>

#include "dbmanager.h"

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

private:
    Ui::Login *ui;
    DBManager* dbManager;
};

#endif // LOGIN_H
