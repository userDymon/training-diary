#include "mainwindow.h"
#include "login.h"
#include "dbmanager.h"
#include "sqldbmanager.h"
#include "user.h"

#include <QCryptographicHash>
#include <QApplication>
#include <QCoreApplication>
#include <QFile>
#include <QTextStream>
#include <QtDebug>
#include <QMessageBox>

void customMessageHandler(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QFile logFile("log.txt");
    if (logFile.open(QIODevice::WriteOnly | QIODevice::Append)) {
        QTextStream stream(&logFile);
        switch (type) {
        case QtInfoMsg:
            stream << "Info: ";
            break;
        case QtDebugMsg:
            stream << "Debug: ";
            break;
        case QtWarningMsg:
            stream << "Warning: ";
            break;
        case QtCriticalMsg:
            stream << "Critical: ";
            break;
        case QtFatalMsg:
            stream << "Fatal: ";
            break;
        }

        stream << msg << " (" << context.file << ":" << context.line << ", " << context.function
               << ")\n";
        logFile.close();
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    qInstallMessageHandler(customMessageHandler);

    DBManager *dbManager = SqlDBManager::getInstance();
    dbManager->connectToDataBase();

    MainWindow w(dbManager, new User(dbManager->returnSavedCredentials()));

    Login l(dbManager);

    if (dbManager->hasSavedCredentials()) {
        w.show();
    } else {
        l.show();
    }

    return a.exec();
}

