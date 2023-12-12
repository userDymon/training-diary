#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);
    // Додайте ініціалізацію, яка викликається при створенні головного вікна
}

MainWindow::MainWindow(bool isLoggedIn, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    if (!isLoggedIn) {
        // Якщо користувач не увійшов, то можна показати віджет для входу або інше відповідне вікно
    } else {
        // Якщо користувач увійшов, викликаємо ініціалізацію головного вікна
        // Додайте ініціалізацію, яка викликається при успішному вході
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}

