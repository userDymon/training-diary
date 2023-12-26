#include "exercizedialog.h"
#include "ui_exercizedialog.h"

#include "exercise.h"

#include <QMessageBox>

ExercizeDialog::ExercizeDialog(DBManager* dbManager, User *user, bool isGoal, QWidget *parent) :
    QDialog(parent), user(user), dbManager(dbManager), isGoal(isGoal),
    ui(new Ui::ExercizeDialog)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("C:/Users/dimag/OneDrive/Документи/ТНТУ/КН/323/Курсова робота/training-diary/img/exercise.ico"));
}

ExercizeDialog::~ExercizeDialog()
{
    delete ui;
}

Ui::ExercizeDialog * ExercizeDialog::getUi(){
    return this->ui;
}


void ExercizeDialog::on_addExercizePB_clicked()
{
    bool ok = false;
    QString name = ui->nameLineEdit->text();
    int weight = ui->weightLineEdit->text().toInt(&ok);
    int sets = ui->setsLineEdit->text().toInt(&ok);
    int reps = ui->repsLineEdit->text().toInt(&ok);

    if(name.isEmpty() || ui->weightLineEdit->text().isEmpty()
        || ui->repsLineEdit->text().isEmpty() || ui->setsLineEdit->text().isEmpty()){
        QMessageBox::critical(this, "Error", "All fields must be filled");
    }else{
        if(isGoal){
            if(!ok){
                QMessageBox::critical(this, "Error", "Weight or Sets or Reps is not number");
            }else{
                if(dbManager->haveGoalExercise(user->getLogin(), name, weight, sets, reps, false)){
                    QMessageBox::warning(this, "Error", "The goal has been achieved or already set");
                }else{
                    this->accept();
                }
            }
        }
        else{
            if(!ok){
                QMessageBox::critical(this, "Error", "Weight or Sets or Reps is not number");
            }else{
                this->accept();
            }
        }
    }
}

