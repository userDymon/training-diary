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


    this->setupModel("exercises",
                     QStringList()<< tr("id")
                                   << tr("Name")
                                   << tr("Weight")
                                   << tr("Sets")
                                   << tr("Reps")
                                   << tr("Date")
                     );

    this->createUI(ui->historyTableView);

    exercizeDialog = new ExercizeDialog(dbManager, user);
    exercizeDialog->setModal(true);

    // Отримати поточну дату
    QDate currentDate = QDate::currentDate();

    // Встановити поточну дату в QDateEdit
    ui->dateEdit->setDate(currentDate);

    connect(exercizeDialog, ExercizeDialog::addExercise, this, MainWindow::addNewItemExercise);
}

MainWindow::~MainWindow()
{
    delete ui;
    if (model)
        delete model;
}

void MainWindow::on_pushButton_clicked()
{
    exercizeDialog->show();
    exercizeDialog->exec();

    model->select();
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


void MainWindow::setupModel(const QString& tableName, const QStringList& headers) {
    model = new QSqlTableModel(this, dbManager->getDB());
    model->setTable(tableName);
    for (int i = 0, j = 0; i < model->columnCount(); i++, j++) {
        model->setHeaderData(i, Qt::Horizontal, headers[j]);
    }
    //model->setSort(0, Qt::AscendingOrder);

}

void MainWindow::createUI(QTableView *tableView) {
    // Ensure that the model is defined before using it
    if (model) {

        proxyModel = new QSortFilterProxyModel(this);
        proxyModel->setSourceModel(model);

        // Set the model for the tableView
        tableView->setModel(proxyModel);

        // Hide the column with ID (if it exists)
        tableView->setColumnHidden(0, true);

        // Set selection behavior to select entire rows
        tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

        // Set selection mode to allow only single selection
        tableView->setSelectionMode(QAbstractItemView::SingleSelection);

        // Automatically resize column widths
        tableView->resizeColumnsToContents();

        // Make columns editable on double-click
        //tableView->setEditTriggers(QAbstractItemView::DoubleClicked);

        // Stretch columns in the horizontal header
        tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

        model->select();
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

