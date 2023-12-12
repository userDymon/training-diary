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
    ui->passwordLine->setEchoMode(QLineEdit::Password);
    ui->signUp->setText("<a href='#'>Sign Up</a>");
    connect(ui->signUp, SIGNAL(linkActivated(const QString&)), this, SLOT(on_signUp_linkActivated()));

    signUp = new SignUp(dbManager);
    signUp->setModal(true);
}

void Login::attemptSignIn(const QString& login, const QString& password) {
    // Хешуємо введене користувачем пароль
    QByteArray hashedPassword = QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256);

    // Створюємо об'єкт користувача зі введеними даними
    User user(login, QString(hashedPassword));

    // Перевірка наявності користувача в базі даних
    if (dbManager->selectFromTable(user)) {
        emit signInSuccess();

        // При успішному вході, створіть головне вікно та відобразіть його
        MainWindow* mainWindow = new MainWindow(true);
        mainWindow->show();
    } else {
        emit signInFailed("Incorrect login or password.");
    }
}

Login::~Login()
{
    delete ui;
}

void Login::on_submitPB_clicked()
{
    attemptSignIn(ui->loginLine->text(), ui->passwordLine->text());
}


void Login::on_signUp_linkActivated(const QString &link)
{
    signUp->show();
}

