#ifndef SCHEDULEEXERCISE_H
#define SCHEDULEEXERCISE_H

#include <QDialog>

#include "user.h"
#include "dbmanager.h"

namespace Ui {
class ScheduleExercise;
}

class ScheduleExercise : public QDialog
{
    Q_OBJECT

public:
    explicit ScheduleExercise(DBManager* dbManager, User *user, QWidget *parent = nullptr);
    ~ScheduleExercise();

private slots:

    void on_addExercizePB_clicked();

private:
    Ui::ScheduleExercise *ui;

    User *user;
    DBManager* dbManager;
};

#endif // SCHEDULEEXERCISE_H
