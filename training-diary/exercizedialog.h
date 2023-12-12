#ifndef EXERCIZEDIALOG_H
#define EXERCIZEDIALOG_H

#include <QDialog>

namespace Ui {
class ExercizeDialog;
}

class ExercizeDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ExercizeDialog(QWidget *parent = nullptr);
    ~ExercizeDialog();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::ExercizeDialog *ui;
};

#endif // EXERCIZEDIALOG_H
