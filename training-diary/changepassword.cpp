#include "changepassword.h"
#include "ui_changepassword.h"

#include <QMessageBox>
#include <QCryptographicHash>

ChangePassword::ChangePassword(DBManager* dbManager, User *user, QWidget *parent)
    : QDialog(parent),
    dbManager(dbManager),
    user(user),
    ui(new Ui::ChangePassword)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("C:/Users/dimag/OneDrive/Документи/ТНТУ/КН/323/Курсова робота/training-diary/img/password.ico"));
    ui->oldPasswordLE->setEchoMode(QLineEdit::Password);
    ui->newPasswordLE->setEchoMode(QLineEdit::Password);
    ui->newPasswordConfirmLE->setEchoMode(QLineEdit::Password);
}

ChangePassword::~ChangePassword()
{
    delete ui;
}

void ChangePassword::on_changePasswordPB_clicked()
{
    if(ui->oldPasswordLE->text().isEmpty() || ui->newPasswordLE->text().isEmpty() || ui->newPasswordConfirmLE->text().isEmpty()){
        QMessageBox::critical(this, "Error", "All fields must be filled");
    }else{
        if(ui->newPasswordLE->text() == ui->newPasswordConfirmLE->text()){
            QByteArray oldPasswordHash = QCryptographicHash::hash(ui->oldPasswordLE->text().toUtf8(), QCryptographicHash::Sha256);
            User oldPasswordUser(user->getLogin(), oldPasswordHash);
            if(dbManager->selectFromTable(oldPasswordUser)){
                QByteArray hashedPassword = QCryptographicHash::hash(ui->newPasswordConfirmLE->text().toUtf8(), QCryptographicHash::Sha256);
                dbManager->updatePassword(user->getLogin(), hashedPassword);
                dbManager->clearAutologinTable();
                user->setPassword(hashedPassword);
                this->accept();
            }else{
                QMessageBox::critical(this, "Error", "Old password don't match");
            }
        }else{
            QMessageBox::warning(this, "Error", "Passwords must match");
        }
    }
}

