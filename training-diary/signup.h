#ifndef SIGNUP_H
#define SIGNUP_H

#include <QDialog>

#include "dbmanager.h"

namespace Ui {
class SignUp;
}

class SignUp : public QDialog
{
    Q_OBJECT

public:
    explicit SignUp(DBManager* dbManager, QWidget *parent = nullptr);
    ~SignUp();

private slots:
    void on_signIn_linkActivated(const QString &link);

    void on_submitPB_clicked();

private:
    Ui::SignUp *ui;
    DBManager* dbManager;
};

#endif // SIGNUP_H
