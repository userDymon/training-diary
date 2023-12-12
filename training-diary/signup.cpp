#include "signup.h"
#include "ui_signup.h"
#include "user.h"

#include <QString>
#include <QCryptographicHash>
#include <QMessageBox>

SignUp::SignUp(DBManager* dbManager, QWidget *parent) :
    QDialog(parent),
    dbManager(dbManager),
    ui(new Ui::SignUp)
{
    ui->setupUi(this);
    ui->createPasswordLine->setEchoMode(QLineEdit::Password);
    ui->confirmPasswordLine->setEchoMode(QLineEdit::Password);
    ui->signIn->setText("<a href='#'>Sign In</a>");
    connect(ui->signIn, SIGNAL(linkActivated(const QString&)), this, SLOT(on_signIn_linkActivated()));
}

SignUp::~SignUp()
{
    delete ui;
}

void SignUp::on_signIn_linkActivated(const QString &link)
{
    this->reject();
}


void SignUp::on_submitPB_clicked()
{
    QString login = ui->loginLine->text();

    if(ui->createPasswordLine->text() == ui->confirmPasswordLine->text()){
        QByteArray hashedPassword = QCryptographicHash::hash(ui->createPasswordLine->text().toUtf8(), QCryptographicHash::Sha256);
        User newUser(ui->loginLine->text(), hashedPassword);
        dbManager->insertIntoTable(newUser);
        this->accept();
    }else{
        QMessageBox::critical(this, "Помилка", "Паролі повинні співпадати");
    }
}

