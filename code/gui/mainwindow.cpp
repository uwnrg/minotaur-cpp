#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, const char *title) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

QTextBrowser* MainWindow::GetLogView() {
    return ui->log_viewer;
}

MainWindow::~MainWindow()
{
    delete ui;
}
