#include "simulatorwindow.h"
#include "ui_simulatorwindow.h"
#include "mainwindow.h"
#include "../graphics/simulatorscene.h"

SimulatorWindow::SimulatorWindow(std::shared_ptr<Simulator>& simulator, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimulatorWindow),
    m_simulator(simulator) {
    ui->setupUi(this);

    m_simulator_scene = new SimulatorScene(m_simulator, ui->simulator_graphics_view);
    m_simulator->setSimulatorScene(m_simulator_scene);
    ui->simulator_graphics_view->setScene(m_simulator_scene);
}

void SimulatorWindow::keyPressEvent(QKeyEvent *event) {
    MainWindow *parent = (MainWindow*) parentWidget();
    parent->keyPressEvent(event);
}

SimulatorWindow::~SimulatorWindow() {
    delete m_simulator_scene;
    delete ui;
}
