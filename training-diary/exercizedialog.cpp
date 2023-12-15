#include "exercizedialog.h"
#include "ui_exercizedialog.h"

#include "exercise.h"

#include <QMessageBox>

ExercizeDialog::ExercizeDialog(DBManager* dbManager, User *user, QWidget *parent) :
    QDialog(parent), user(user), dbManager(dbManager),
    ui(new Ui::ExercizeDialog)
{
    ui->setupUi(this);
}

ExercizeDialog::~ExercizeDialog()
{
    delete ui;
}


void ExercizeDialog::on_addExercizePB_clicked()
{
    /*
    if(ui->nameLineEdit->text().isEmpty() || ui->weightLineEdit->text().isEmpty()
        || ui->repsLineEdit->text().isEmpty() || ui->setsLineEdit->text().isEmpty()){
        QMessageBox::critical(this, "Error", "All fields must be filled");
    }else{
        bool ok;
        int weight = ui->weightLineEdit->text().toInt(&ok);
        int reps = ui->repsLineEdit->text().toInt(&ok);
        int sets = ui->setsLineEdit->text().toInt(&ok);
        if(!ok){
            QMessageBox::critical(this, "Error", "Sets or Reps is not number");
        }else{
            Exercise newExercise(ui->nameLineEdit->text(), weight, sets,reps);
            dbManager->insertIntoTable(newExercise, user->getLogin());

            Exercise * exercise = new Exercise(ui->nameLineEdit->text(), weight, sets,reps);
            emit addExercise(exercise);
            this->accept();
        }
    }
    */
    if(ui->nameLineEdit->text().isEmpty() || ui->weightLineEdit->text().isEmpty()
        || ui->repsLineEdit->text().isEmpty() || ui->setsLineEdit->text().isEmpty()){
        QMessageBox::critical(this, "Error", "All fields must be filled");
    }else{
        bool ok;
        ui->weightLineEdit->text().toInt(&ok);
        ui->repsLineEdit->text().toInt(&ok);
        ui->setsLineEdit->text().toInt(&ok);
        if(!ok){
            QMessageBox::critical(this, "Error", "Weight or Sets or Reps is not number");
        }else{
            this->accept();
        }
    }
}

