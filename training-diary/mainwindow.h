#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QTableView>
#include <QVector>
#include <QSortFilterProxyModel>

#include "exercizedialog.h"
#include "user.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class QSqlTableModel;

class DBManager;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(DBManager* dbManager, User *user, QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();
    void addNewItemExercise(Exercise *);

    void on_dateEdit_userDateChanged(const QDate &date);

private:
    Ui::MainWindow *ui;
    ExercizeDialog * exercizeDialog;
    User *user;
    DBManager* dbManager;

    QSqlTableModel* model;

    QSortFilterProxyModel* proxyModel;

    void setupModel(const QString& tableName, const QStringList& headers);

    void createUI(QTableView *);
};
#endif // MAINWINDOW_H
