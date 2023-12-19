#ifndef CHANGEUSERNAME_H
#define CHANGEUSERNAME_H

#include <QDialog>

#include "dbmanager.h"

namespace Ui {
class changeUsername;
}

class ChangeUsername : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeUsername(DBManager* dbManager, User *user, QWidget *parent = nullptr);
    ~ChangeUsername();

private slots:
    void on_changeUsernamePB_clicked();

private:
    Ui::changeUsername *ui;
    DBManager* dbManager;
    User *user;
};

#endif // CHANGEUSERNAME_H
