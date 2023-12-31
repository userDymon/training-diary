#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>
#include <QTableView>
#include <QVector>
#include <QSortFilterProxyModel>
#include <QtUiTools/QUiLoader>

#include "exercizedialog.h"
#include "user.h"
#include "changeusername.h"
#include "changepassword.h"
#include "aboutprogram.h"

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
    void addNewItemProgresExercise(Exercise *);

    void on_dateEdit_userDateChanged(const QDate &date);

    void on_addExerciseSchedulePB_clicked();

    void on_comboBox_currentTextChanged(const QString &arg1);

    void on_addGoalPB_clicked();

    void on_actionLog_out_triggered();

    void on_actionChange_login_triggered();

    void on_actionChange_password_triggered();

    void on_actionAbout_program_triggered();

    void on_deleteExerciseSchedulePB_clicked();

    void on_clearGoalPB_clicked();

    void on_deleteExercisePB_clicked();

private:
    Ui::MainWindow *ui;
    ExercizeDialog * exercizeDialog;
    User *user;
    DBManager* dbManager;
    ChangeUsername * changeUsernameDialog;
    ChangePassword * changePasswordDialog;
    AboutProgram * aboutProgram;

    QSqlTableModel* historyModel;
    QSqlTableModel* scheduleModel;
    QSqlTableModel* goalModel;

    QSortFilterProxyModel* historyProxyModel;
    QSortFilterProxyModel* dayOfWeekFilterProxyModel;
    QSortFilterProxyModel* userProxyModel;

    Ui::ExercizeDialog *exercizeDialogUi;

    void setupModelHistory(const QString& tableName, const QStringList& headers);
    void setupModelSchedule(const QString& tableName, const QStringList& headers);
    void setupModelGoal(const QString& tableName, const QStringList& headers);

    void createUIforHistory();
    void createUIforSchedule();
    void createUIforGoal();
};
#endif // MAINWINDOW_H
