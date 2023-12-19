#ifndef CHANGEPASSWORD_H
#define CHANGEPASSWORD_H

#include <QDialog>

#include "dbmanager.h"

namespace Ui {
class ChangePassword;
}

class ChangePassword : public QDialog
{
    Q_OBJECT

public:
    explicit ChangePassword(DBManager* dbManager, User *user, QWidget *parent = nullptr);
    ~ChangePassword();

private slots:
    void on_changePasswordPB_clicked();

private:
    Ui::ChangePassword *ui;
    DBManager* dbManager;
    User *user;
};

#endif // CHANGEPASSWORD_H
