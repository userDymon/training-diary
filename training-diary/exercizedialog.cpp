#include "exercizedialog.h"
#include "ui_exercizedialog.h"

ExercizeDialog::ExercizeDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ExercizeDialog)
{
    ui->setupUi(this);
}

ExercizeDialog::~ExercizeDialog()
{
    delete ui;
}

void ExercizeDialog::on_buttonBox_accepted()
{
}

