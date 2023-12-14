#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QTableView>
#include <QVector>
#include <QSortFilterProxyModel>

#include "exercizedialog.h"
#include "scheduleexercise.h"
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

    void on_addExerciseSchedulePB_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

private:
    Ui::MainWindow *ui;
    ExercizeDialog * exercizeDialog;
    ScheduleExercise * scheduleExerciseDialog;
    User *user;
    DBManager* dbManager;

    QSqlTableModel* historyModel;
    QSqlTableModel* scheduleModel;

    QSortFilterProxyModel* proxyModel;
    QSortFilterProxyModel* dayOfWeekFilterProxyModel;

    void setupModelHistory(const QString& tableName, const QStringList& headers);
    void setupModelSchedule(const QString& tableName, const QStringList& headers);

    void createUIforHistory();
    void createUIforSchedule();
};
#endif // MAINWINDOW_H
