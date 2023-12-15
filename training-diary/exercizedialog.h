#ifndef EXERCIZEDIALOG_H
#define EXERCIZEDIALOG_H

#include <QDialog>

#include "user.h"
#include "dbmanager.h"

namespace Ui {
class ExercizeDialog;
}

class ExercizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExercizeDialog(DBManager* dbManager, User *user, QWidget *parent = nullptr);
    ~ExercizeDialog();
    Ui::ExercizeDialog *ui;

private slots:   
    void on_addExercizePB_clicked();


private:
    User *user;
    DBManager* dbManager;
};

#endif // EXERCIZEDIALOG_H
