#include "aboutprogram.h"
#include "ui_aboutprogram.h"

AboutProgram::AboutProgram(QDialog *parent)
    : QDialog(parent)
    , ui(new Ui::AboutProgram)
{
    ui->setupUi(this);
    this->setWindowIcon(QIcon("C:/Users/dimag/OneDrive/Документи/ТНТУ/КН/323/Курсова робота/training-diary/img/training-diary.ico"));
    ui->label->setText("The programme is a course work of a student\n"
                       "of the group KN-323 commissioned by the head\n"
                       "of the course work, teacher Lisovuy V.M.");

}

AboutProgram::~AboutProgram()
{
    delete ui;
}
