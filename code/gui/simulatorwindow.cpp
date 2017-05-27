#include "simulatorwindow.h"
#include "ui_simulatorwindow.h"
#include "mainwindow.h"
#include "../graphics/simulatorscene.h"

SimulatorWindow::SimulatorWindow(std::shared_ptr<Simulator> simulator, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimulatorWindow) {
    ui->setupUi(this);

    m_simulator_scene = new SimulatorScene(simulator, ui->simulator_graphics_view);
    ui->simulator_graphics_view->setScene(m_simulator_scene);
}

void SimulatorWindow::keyPressEvent(QKeyEvent *event) {
    QWidget *parent = parentWidget();
    if (!parent) return;
    MainWindow *mainWindow = (MainWindow*) parentWidget();
    mainWindow->keyPressEvent(event);
}

SimulatorWindow::~SimulatorWindow() {
    delete m_simulator_scene;
    delete ui;
}

SimulatorScene *SimulatorWindow::getSimulatorScene() {
    return m_simulator_scene;
}
