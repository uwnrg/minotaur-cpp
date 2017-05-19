#include "simulatorwindow.h"
#include "ui_simulatorwindow.h"

SimulatorWindow::SimulatorWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimulatorWindow)
{
    ui->setupUi(this);
}

SimulatorWindow::~SimulatorWindow()
{
    delete ui;
}
