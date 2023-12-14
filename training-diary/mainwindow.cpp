#include <QSqlQuery>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "exercizedialog.h"

#include <QSqlTableModel>
#include <QTableView>
#include <QListWidgetItem>
#include <QString>
#include <QDate>
#include <QMessageBox>
#include <QSqlQueryModel>
#include <QSqlError>
#include <QDebug>

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

    exercizeDialog = new ExercizeDialog(dbManager, user);
    exercizeDialog->setModal(true);

    scheduleExerciseDialog = new ScheduleExercise(dbManager, user);
    scheduleExerciseDialog->setModal(true);

    // Отримати поточну дату
    QDate currentDate = QDate::currentDate();

    // Встановити поточну дату в QDateEdit
    ui->dateEdit->setDate(currentDate);
    on_comboBox_currentTextChanged("Monday");

    connect(exercizeDialog, ExercizeDialog::addExercise, this, MainWindow::addNewItemExercise);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (historyModel)
        delete historyModel;
}

void MainWindow::on_pushButton_clicked()
{
    exercizeDialog->show();
    exercizeDialog->exec();

    historyModel->select();
}

void  MainWindow::addNewItemExercise(Exercise *exercise){
    QListWidgetItem* item = new QListWidgetItem();
    item->setText(QString("name: %1, weight: %2, sets: %3, reps: %4")
                      .arg(exercise->getName())
                      .arg(exercise->getWeight())
                      .arg(exercise->getSets())
                      .arg(exercise->getReps())
                  );
    ui->listWidget->addItem(item);
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


void MainWindow::createUIforHistory() {
    // Ensure that the model is defined before using it
    if (historyModel) {

        proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(historyModel);

        // Set the model for the tableView
        ui->historyTableView->setModel(proxyModel);

        // Hide the column with ID (if it exists)
        ui->historyTableView->setColumnHidden(0, true);
        ui->historyTableView->setColumnHidden(5, true);

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


void MainWindow::on_dateEdit_userDateChanged(const QDate &date)
{
    proxyModel->setFilterKeyColumn(5); // Assuming the "Date" column is at index 5
    proxyModel->setFilterFixedString(date.toString(Qt::ISODate));
}


void MainWindow::on_addExerciseSchedulePB_clicked()
{
    scheduleExerciseDialog->show();
    scheduleExerciseDialog->exec();

    scheduleModel->select();
}


void MainWindow::on_comboBox_currentTextChanged(const QString &dayOfWeek)
{
    dayOfWeekFilterProxyModel->setFilterKeyColumn(5); // Assuming the "dayOfWeek" column is at index 5
    dayOfWeekFilterProxyModel->setFilterFixedString(dayOfWeek);
}

