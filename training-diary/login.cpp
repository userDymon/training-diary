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
    this->setWindowIcon(QIcon("C:/Users/dimag/OneDrive/Документи/ТНТУ/КН/323/Курсова робота/training-diary/img/account.ico"));
    ui->passwordLine->setEchoMode(QLineEdit::Password);
    ui->signUp->setText("<a href='#'>Sign Up</a>");

    connect(ui->signUp, SIGNAL(linkActivated(const QString&)), this, SLOT(on_signUp_linkActivated()));
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
        if(ui->autoLogCB->isChecked()){
            dbManager->autoLog(*user);
        }
        this->accept();
        MainWindow* mainWindow = new MainWindow(dbManager, user);
        mainWindow->show();
    } else {
        emit signInFailed("Incorrect login or password.");
    }
}

void Login::on_submitPB_clicked()
{
    if(ui->loginLine->text().isEmpty() || ui->passwordLine->text().isEmpty()){
        QMessageBox::warning(this, "Error", "All fields must be filled");
    }else{
        attemptSignIn(ui->loginLine->text(), ui->passwordLine->text());
    }
}


void Login::on_signUp_linkActivated(const QString &link)
{
    signUp->show();
}


void Login::onSignInFailed() {
    QMessageBox::critical(this, "Error", "Login failed. Check your credentials.");
}

