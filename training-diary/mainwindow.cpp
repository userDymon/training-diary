#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "exercizedialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    this->setWindowTitle("Training diary");
    exercizeDialog = new ExercizeDialog();
    exercizeDialog->setModal(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_clicked()
{
    exercizeDialog->show();
}

