#include "scheduleexercise.h"
#include "ui_scheduleexercise.h"

#include <QMessageBox>

ScheduleExercise::ScheduleExercise(DBManager* dbManager, User *user, QWidget *parent)
    : QDialog(parent), user(user), dbManager(dbManager),
    ui(new Ui::ScheduleExercise)
{
    ui->setupUi(this);

}

ScheduleExercise::~ScheduleExercise()
{
    delete ui;
}


void ScheduleExercise::on_addExercizePB_clicked()
{
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
            dbManager->insertIntoTable(newExercise, user->getLogin(), ui->comboBox->currentText());
            this->accept();
        }
    }
}

