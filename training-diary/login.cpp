#include "login.h"
#include "ui_login.h"
#include "user.h"
#include "mainwindow.h"
#include "signup.h"

#include <QCryptographicHash>
#include <QMessageBox>

Login::Login(DBManager* dbManager, QWidget *parent)
    : QDialog(parent), dbManager(dbManager),  ui(new Ui::Login)
{
    ui->setupUi(this);
    this->setWindowTitle("Training diary");
    ui->passwordLine->setEchoMode(QLineEdit::Password);
    ui->signUp->setText("<a href='#'>Sign Up</a>");

    connect(ui->signUp, SIGNAL(linkActivated(const QString&)), this, SLOT(on_signUp_linkActivated()));
    connect(this, &Login::signInSuccess, this, &Login::onSignInSuccess);
    connect(this, &Login::signInFailed, this, &Login::onSignInFailed);

    signUp = new SignUp(dbManager);
    signUp->setModal(true);
}


Login::~Login()
{
    delete ui;
}

void Login::attemptSignIn(const QString& login, const QString& password) {
    // Хешуємо введене користувачем пароль
    QByteArray hashedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);

    // Створюємо об'єкт користувача зі введеними даними
    user = new User(login, QString(hashedPassword));

    // Перевірка наявності користувача в базі даних
    if (dbManager->selectFromTable(*user)) {
        emit signInSuccess();
        if(ui->autoLogCB){
            dbManager->autoLog(*user);
        }
        MainWindow* mainWindow = new MainWindow(dbManager, user);
        mainWindow->show();
    } else {
        emit signInFailed("Incorrect login or password.");
    }
}

void Login::on_submitPB_clicked()
{
    if(ui->loginLine->text().isEmpty() || ui->passwordLine->text().isEmpty()){
        QMessageBox::critical(this, "Error", "All fields must be filled");
    }else{
        attemptSignIn(ui->loginLine->text(), ui->passwordLine->text());
    }
}


void Login::on_signUp_linkActivated(const QString &link)
{
    signUp->show();
}

void Login::onSignInSuccess() {
    QMessageBox::information(this, "Success", "Login successful");
    this->accept();
}

void Login::onSignInFailed() {
    QMessageBox::critical(this, "Error", "Login failed. Check your credentials.");
}

