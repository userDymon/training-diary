#include <QSqlQuery>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "exercizedialog.h"
#include "ui_exercizedialog.h"

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

MainWindow::MainWindow(DBManager* dbManager,User *user, QWidget *parent)
    : QMainWindow(parent), user(user), dbManager(dbManager), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowTitle("Training diary");


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
    int result = exercizeDialog->exec();

    if(result == QDialog::Accepted){
        QString name = exercizeDialog->ui->nameLineEdit->text();
        int weight = exercizeDialog->ui->weightLineEdit->text().toInt();
        int reps = exercizeDialog->ui->repsLineEdit->text().toInt();
        int sets = exercizeDialog->ui->setsLineEdit->text().toInt();
        Exercise newExercise(name, weight, sets,reps);
        dbManager->insertIntoTable(newExercise, user->getLogin());
        addNewItemExercise(&newExercise);

        if(dbManager->haveGoalExercise(user->getLogin(), name, weight, sets,reps, true)){
            dbManager->deleteGoalExercise(user->getLogin(), name, weight, sets, reps);
        }

        Exercise progresExercise(dbManager->returnProgresExercise(name));
        addNewItemProgresExercise(&progresExercise);
        goalModel->select();
    }

    historyModel->select();
}

void MainWindow::on_addExerciseSchedulePB_clicked()
{
    exercizeDialog->deleteLater();
    exercizeDialog = new ExercizeDialog(dbManager, user, false);
    exercizeDialog->setModal(true);
    exercizeDialog->show();
    int result = exercizeDialog->exec();

    if(result == QDialog::Accepted){
        QString name = exercizeDialog->ui->nameLineEdit->text();
        int weight = exercizeDialog->ui->weightLineEdit->text().toInt();
        int reps = exercizeDialog->ui->repsLineEdit->text().toInt();
        int sets = exercizeDialog->ui->setsLineEdit->text().toInt();
        Exercise newExercise(name, weight, sets,reps);
        dbManager->insertIntoTable(newExercise, user->getLogin(), ui->comboBox->currentText());
    }

    scheduleModel->select();
}

void MainWindow::on_addGoalPB_clicked()
{
    exercizeDialog->deleteLater();
    exercizeDialog = new ExercizeDialog(dbManager, user, true);
    exercizeDialog->setModal(false);
    exercizeDialog->show();
    int result = exercizeDialog->exec();

    if(result == QDialog::Accepted){
        QString name = exercizeDialog->ui->nameLineEdit->text();
        int weight = exercizeDialog->ui->weightLineEdit->text().toInt();
        int reps = exercizeDialog->ui->repsLineEdit->text().toInt();
        int sets = exercizeDialog->ui->setsLineEdit->text().toInt();
        Exercise newExercise(name, weight, sets,reps);
        dbManager->insertIntoTable(newExercise, user->getLogin(), true);
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
    //model->setSort(0, Qt::AscendingOrder);

}

void MainWindow::setupModelSchedule(const QString& tableName, const QStringList& headers) {
    scheduleModel = new QSqlTableModel(this, dbManager->getDB());
    scheduleModel->setTable(tableName);
    for (int i = 0, j = 0; i < scheduleModel->columnCount(); i++, j++) {
        scheduleModel->setHeaderData(i, Qt::Horizontal, headers[j]);
    }
    //model->setSort(0, Qt::AscendingOrder);

}

void MainWindow::setupModelGoal(const QString& tableName, const QStringList& headers) {
    goalModel = new QSqlTableModel(this, dbManager->getDB());
    goalModel->setTable(tableName);
    for (int i = 0, j = 0; i < goalModel->columnCount(); i++, j++) {
        goalModel->setHeaderData(i, Qt::Horizontal, headers[j]);
    }
    //model->setSort(0, Qt::AscendingOrder);

}


void MainWindow::createUIforHistory() {
    // Ensure that the model is defined before using it
    if (historyModel) {

        historyProxyModel = new QSortFilterProxyModel(this);
        historyProxyModel->setSourceModel(historyModel);

        // Set the model for the tableView
        ui->historyTableView->setModel(historyProxyModel);

        // Hide the column with ID (if it exists)
        ui->historyTableView->setColumnHidden(0, true);
        ui->historyTableView->setColumnHidden(5, true);
        ui->historyTableView->setColumnHidden(6, true);

        // Set selection behavior to select entire rows
        ui->historyTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

        // Set selection mode to allow only single selection
        ui->historyTableView->setSelectionMode(QAbstractItemView::SingleSelection);

        // Automatically resize column widths
        ui->historyTableView->resizeColumnsToContents();

        // Make columns editable on double-click
        //tableView->setEditTriggers(QAbstractItemView::DoubleClicked);

        // Stretch columns in the horizontal header
        ui->historyTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        historyModel->select();
    } else {
        // If the model is not defined, print a message to the console
        qDebug() << "Model is not defined.";
    }
}

void MainWindow::createUIforSchedule() {
    // Ensure that the model is defined before using it
    if (scheduleModel) {

        dayOfWeekFilterProxyModel = new QSortFilterProxyModel(this);
        dayOfWeekFilterProxyModel->setSourceModel(scheduleModel);

        // Set the model for the tableView
        ui->scheduleTableView->setModel(dayOfWeekFilterProxyModel);

        // Hide the column with ID (if it exists)
        ui->scheduleTableView->setColumnHidden(0, true);

        ui->scheduleTableView->setColumnHidden(5, true);

        // Set selection behavior to select entire rows
        ui->scheduleTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

        // Set selection mode to allow only single selection
        ui->scheduleTableView->setSelectionMode(QAbstractItemView::SingleSelection);

        // Automatically resize column widths
        ui->scheduleTableView->resizeColumnsToContents();

        // Make columns editable on double-click
        //tableView->setEditTriggers(QAbstractItemView::DoubleClicked);

        // Stretch columns in the horizontal header
        ui->scheduleTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        scheduleModel->select();
    } else {
        // If the model is not defined, print a message to the console
        qDebug() << "Model is not defined.";
    }
}

void MainWindow::createUIforGoal() {
    // Ensure that the model is defined before using it
    if (goalModel) {
        // Set the model for the tableView
        ui->goalsTableView->setModel(goalModel);

        // Hide the column with ID (if it exists)
        ui->goalsTableView->setColumnHidden(0, true);
        ui->goalsTableView->setColumnHidden(5, true);

        // Set selection behavior to select entire rows
        ui->goalsTableView->setSelectionBehavior(QAbstractItemView::SelectRows);

        // Set selection mode to allow only single selection
        ui->goalsTableView->setSelectionMode(QAbstractItemView::SingleSelection);

        // Automatically resize column widths
        ui->goalsTableView->resizeColumnsToContents();

        // Make columns editable on double-click
        //tableView->setEditTriggers(QAbstractItemView::DoubleClicked);

        // Stretch columns in the horizontal header
        ui->goalsTableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        goalModel->select();
    } else {
        // If the model is not defined, print a message to the console
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
    dayOfWeekFilterProxyModel->setFilterKeyColumn(5); // Assuming the "dayOfWeek" column is at index 5
    dayOfWeekFilterProxyModel->setFilterFixedString(dayOfWeek);
}


