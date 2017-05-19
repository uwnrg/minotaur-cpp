#include "simulatorwindow.h"
#include "ui_simulatorwindow.h"
#include "mainwindow.h"

SimulatorWindow::SimulatorWindow(std::shared_ptr<Simulator>& simulator, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimulatorWindow),
    m_simulator(simulator) {
    ui->setupUi(this);
}

void SimulatorWindow::keyPressEvent(QKeyEvent *event) {
    MainWindow *parent = (MainWindow*) parentWidget();
    parent->keyPressEvent(event);
}

SimulatorWindow::~SimulatorWindow() {
    delete ui;
}
