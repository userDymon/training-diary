#include <QSqlQuery>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "exercizedialog.h"
#include "ui_exercizedialog.h"
#include "ui_AboutProgram.h"

#include <QDialog>
#include <QSqlTableModel>
#include <QTableView>
#include <QListWidgetItem>
#include <QString>
#include <QDate>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>
#include <QRegularExpression>
#include <QStyledItemDelegate>
#include <QCheckBox>
#include <QCoreApplication>
#include <QFile>
#include <QtUiTools/QUiLoader>

MainWindow::MainWindow(DBManager* dbManager,User *user, QWidget *parent)
    : QMainWindow(parent), user(user), dbManager(dbManager), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowIcon(QIcon("C:/Users/dimag/OneDrive/Документи/ТНТУ/КН/323/Курсова робота/training-diary/img/training-diary.ico"));

    aboutProgram = new AboutProgram();

    this->setupModelHistory("exercises",
                     QStringList()<< tr("id")
                                   << tr("Name")
                                   << tr("Weight")
                                   << tr("Sets")
                                   << tr("Reps")
                                   << tr("Date")
                                   << tr("isGoal")
                     );

    this->createUIforHistory();

    this->setupModelSchedule("scheduleExercises",
                     QStringList()<< tr("id")
                                   << tr("Name")
                                   << tr("Weight")
                                   << tr("Sets")
                                   << tr("Reps")
                                   << tr("DayOfWeek")
                     );

    this->createUIforSchedule();

    this->setupModelGoal("goals",
                            QStringList()<< tr("id")
                                          << tr("Name")
                                          << tr("Weight")
                                          << tr("Sets")
                                          << tr("Reps")
                                          << tr("isGoal")
                            );

    this->createUIforGoal();

    exercizeDialog = new ExercizeDialog(dbManager, user, false);
    exercizeDialog->setModal(true);

    // Отримати поточну дату
    QDate currentDate = QDate::currentDate();

    // Встановити поточну дату в QDateEdit
    ui->dateEdit->setDate(currentDate);
    on_comboBox_currentTextChanged("Monday");

    changeUsernameDialog = new ChangeUsername(dbManager, user);
    changeUsernameDialog->setModal(true);

    changePasswordDialog = new ChangePassword(dbManager, user);
    changePasswordDialog->setModal(true);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (historyModel)
        delete historyModel;
}

void MainWindow::on_pushButton_clicked()
{
    exercizeDialog->deleteLater();
    exercizeDialog = new ExercizeDialog(dbManager, user, false);
    exercizeDialog->setModal(true);
    exercizeDialog->show();
    exercizeDialogUi =  exercizeDialog->getUi();
    int result = exercizeDialog->exec();

    if(result == QDialog::Accepted){
        QString name = exercizeDialogUi->nameLineEdit->text();
        int weight = exercizeDialogUi->weightLineEdit->text().toInt();
        int reps = exercizeDialogUi->repsLineEdit->text().toInt();
        int sets = exercizeDialogUi->setsLineEdit->text().toInt();
        Exercise newExercise(name, weight, sets,reps);
        dbManager->insertIntoTable(newExercise, user->getLogin());
        addNewItemExercise(&newExercise);

        if(dbManager->haveGoalExercise(user->getLogin(), name, weight, sets,reps, true)){
            dbManager->deleteGoalExercise(user->getLogin(), name, weight, sets, reps);
            QMessageBox::information(this, "Successful", "The goal was achieved");
        }

        Exercise progresExercise(dbManager->returnProgresExercise(name));
        addNewItemProgresExercise(&progresExercise);
        goalModel->select();
    }

    historyModel->select();
}

void MainWindow::on_deleteExercisePB_clicked()
{
    QDate currentDate = QDate::currentDate();
    if(dbManager->deleteExercise(user->getLogin(), currentDate)){
        QMessageBox::information(this, "Successful", "Exercise removed");
        ui->listWidget->clear();
        ui->progresListWidget->clear();
    }else{
        QMessageBox::information(this, "Error", "The exercise has not been deleted");
    }
    historyModel->select();
}

void MainWindow::on_addExerciseSchedulePB_clicked()
{
    exercizeDialog->deleteLater();
    exercizeDialog = new ExercizeDialog(dbManager, user, false);
    exercizeDialog->setModal(true);
    exercizeDialog->show();
    exercizeDialogUi =  exercizeDialog->getUi();
    int result = exercizeDialog->exec();

    if(result == QDialog::Accepted){
        QString name = exercizeDialogUi->nameLineEdit->text();
        int weight = exercizeDialogUi->weightLineEdit->text().toInt();
        int reps = exercizeDialogUi->repsLineEdit->text().toInt();
        int sets = exercizeDialogUi->setsLineEdit->text().toInt();
        Exercise newExercise(name, weight, sets,reps);
        dbManager->insertIntoTable(newExercise, user->getLogin(), ui->comboBox->currentText());
    }

    scheduleModel->select();
}

void MainWindow::on_deleteExerciseSchedulePB_clicked()
{
    QString dayOfWeek = ui->comboBox->currentText();
    if(dbManager->deleteScheduleExercise(user->getLogin(), dayOfWeek)){
        QMessageBox::information(this, "Successful", "Exercise removed");
    }else{
        QMessageBox::information(this, "Error", "The exercise has not been deleted");
    }
    scheduleModel->select();
}

void MainWindow::on_addGoalPB_clicked()
{
    exercizeDialog->deleteLater();
    exercizeDialog = new ExercizeDialog(dbManager, user, true);
    exercizeDialog->setModal(false);
    exercizeDialog->show();
    exercizeDialogUi =  exercizeDialog->getUi();
    int result = exercizeDialog->exec();

    if(result == QDialog::Accepted){
        QString name = exercizeDialogUi->nameLineEdit->text();
        int weight = exercizeDialogUi->weightLineEdit->text().toInt();
        int reps = exercizeDialogUi->repsLineEdit->text().toInt();
        int sets = exercizeDialogUi->setsLineEdit->text().toInt();
        Exercise newExercise(name, weight, sets,reps);
        dbManager->insertIntoGoals(newExercise, user->getLogin());
    }

    goalModel->select();
}

void MainWindow::on_clearGoalPB_clicked()
{
    if(dbManager->deleteGoalExercise(user->getLogin())){
        QMessageBox::information(this, "Successful", "Exercises removed");
    }else{
        QMessageBox::information(this, "Error", "The exercises has not been deleted");
    }
    goalModel->select();
}

void  MainWindow::addNewItemExercise(Exercise *exercise){
    QListWidgetItem* item = new QListWidgetItem();
    item->setText(QString("Name: %1, Weight: %2, Sets: %3, Reps: %4")
                      .arg(exercise->getName())
                      .arg(exercise->getWeight())
                      .arg(exercise->getSets())
                      .arg(exercise->getReps())
                  );
    ui->listWidget->addItem(item);
}

void  MainWindow::addNewItemProgresExercise(Exercise *exercise){
    QListWidgetItem* item = new QListWidgetItem();
    item->setText(QString("Name: %1, Weight: %2, Sets: %3, Reps: %4")
                      .arg(exercise->getName())
                      .arg(exercise->getWeight() > 0 ? "+" + QString::number(exercise->getWeight()) : QString::number(exercise->getWeight()))
                      .arg(exercise->getSets() > 0 ? "+" + QString::number(exercise->getSets()) : QString::number(exercise->getSets()))
                      .arg(exercise->getReps() > 0 ? "+" + QString::number(exercise->getReps()) : QString::number(exercise->getReps()))
                  );
    ui->progresListWidget->addItem(item);
}


void MainWindow::setupModelHistory(const QString& tableName, const QStringList& headers) {
    historyModel = new QSqlTableModel(this, dbManager->getDB());
    historyModel->setTable(tableName);
    for (int i = 0, j = 0; i < historyModel->columnCount(); i++, j++) {
        historyModel->setHeaderData(i, Qt::Horizontal, headers[j]);
    }
}

void MainWindow::setupModelSchedule(const QString& tableName, const QStringList& headers) {
    scheduleModel = new QSqlTableModel(this, dbManager->getDB());
    scheduleModel->setTable(tableName);
    for (int i = 0, j = 0; i < scheduleModel->columnCount(); i++, j++) {
        scheduleModel->setHeaderData(i, Qt::Horizontal, headers[j]);
    }
}

void MainWindow::setupModelGoal(const QString& tableName, const QStringList& headers) {
    goalModel = new QSqlTableModel(this, dbManager->getDB());
    goalModel->setTable(tableName);
    for (int i = 0, j = 0; i < goalModel->columnCount(); i++, j++) {
        goalModel->setHeaderData(i, Qt::Horizontal, headers[j]);
    }
}


void MainWindow::createUIforHistory() {
    if (historyModel) {

        userProxyModel = new QSortFilterProxyModel(this);
        userProxyModel->setFilterKeyColumn(0);
        userProxyModel->setFilterFixedString(QString::number(dbManager->returnId(user)));

        historyProxyModel = new QSortFilterProxyModel(this);
        historyProxyModel->setSourceModel(historyModel);

        userProxyModel->setSourceModel(historyProxyModel);

        ui->historyTableView->setModel(userProxyModel);

        ui->historyTableView->setColumnHidden(0, true);
        ui->historyTableView->setColumnHidden(5, true);

        ui->historyTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

        ui->historyTableView->setSelectionMode(QAbstractItemView::SingleSelection);

        ui->historyTableView->resizeColumnsToContents();

        ui->historyTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

        ui->historyTableView->verticalHeader()->setVisible(false);

        historyModel->select();
    } else {
        qDebug() << "Model is not defined.";
    }
}

void MainWindow::createUIforSchedule() {
    if (scheduleModel) {
        userProxyModel = new QSortFilterProxyModel(this);
        userProxyModel->setFilterKeyColumn(0);
        userProxyModel->setFilterFixedString(QString::number(dbManager->returnId(user)));

        dayOfWeekFilterProxyModel = new QSortFilterProxyModel(this);
        dayOfWeekFilterProxyModel->setSourceModel(scheduleModel);

        userProxyModel->setSourceModel(dayOfWeekFilterProxyModel);

        ui->scheduleTableView->setModel(userProxyModel);


        ui->scheduleTableView->setColumnHidden(0, true);
        ui->scheduleTableView->setColumnHidden(5, true);


        ui->scheduleTableView->setSelectionBehavior(QAbstractItemView::SelectRows);


        ui->scheduleTableView->setSelectionMode(QAbstractItemView::SingleSelection);


        ui->scheduleTableView->resizeColumnsToContents();

        ui->scheduleTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

        ui->scheduleTableView->verticalHeader()->setVisible(false);

        scheduleModel->select();
    } else {
        qDebug() << "Model is not defined.";
    }
}

void MainWindow::createUIforGoal() {
    if (goalModel) {
        userProxyModel = new QSortFilterProxyModel(this);
        userProxyModel->setFilterKeyColumn(0);
        userProxyModel->setFilterFixedString(QString::number(dbManager->returnId(user)));

        userProxyModel->setSourceModel(goalModel);

        ui->goalsTableView->setModel(userProxyModel);


        ui->goalsTableView->setColumnHidden(0, true);
        ui->goalsTableView->setColumnHidden(5, true);


        ui->goalsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);


        ui->goalsTableView->setSelectionMode(QAbstractItemView::SingleSelection);


        ui->goalsTableView->resizeColumnsToContents();

        ui->goalsTableView->horizontalHeader()->setSectionResizeMode(1, QHeaderView::Stretch);

        ui->goalsTableView->verticalHeader()->setVisible(false);

        goalModel->select();
    } else {
        qDebug() << "Model is not defined.";
    }
}


void MainWindow::on_dateEdit_userDateChanged(const QDate &date)
{
    historyProxyModel->setFilterKeyColumn(5);
    historyProxyModel->setFilterFixedString(date.toString(Qt::ISODate));
}


void MainWindow::on_comboBox_currentTextChanged(const QString &dayOfWeek)
{
    dayOfWeekFilterProxyModel->setFilterKeyColumn(5);
    dayOfWeekFilterProxyModel->setFilterFixedString(dayOfWeek);
}



void MainWindow::on_actionLog_out_triggered()
{
    dbManager->clearAutologinTable();
    QApplication::quit();
}


void MainWindow::on_actionChange_login_triggered()
{
    changeUsernameDialog->show();
}


void MainWindow::on_actionChange_password_triggered()
{
    changePasswordDialog->show();
}


void MainWindow::on_actionAbout_program_triggered()
{
    aboutProgram->show();
}

