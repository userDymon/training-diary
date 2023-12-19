#include "changeusername.h"
#include "ui_changeusername.h"

#include <QMessageBox>

ChangeUsername::ChangeUsername(DBManager* dbManager, User *user, QWidget *parent)
    : QDialog(parent),
    dbManager(dbManager),
    user(user),
    ui(new Ui::changeUsername)
{
    ui->setupUi(this);
    this->setWindowTitle("Change login");
}

ChangeUsername::~ChangeUsername()
{
    delete ui;
}

void ChangeUsername::on_changeUsernamePB_clicked()
{
    if(ui->newLoginLE->text().isEmpty()){
        QMessageBox::critical(this, "Error", "Login cannot be empty");
    }else{
        dbManager->updateLogin(user->getLogin(),ui->newLoginLE->text());
        dbManager->clearAutologinTable();
        user->setLogin(ui->newLoginLE->text());
        this->accept();
    }
}

