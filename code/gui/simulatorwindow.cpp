#include "simulatorwindow.h"
#include "ui_simulatorwindow.h"
#include "mainwindow.h"
#include "../graphics/simulatorscene.h"

SimulatorWindow::SimulatorWindow(std::shared_ptr<Simulator> simulator, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimulatorWindow) {
    ui->setupUi(this);
    setWindowTitle("Simulator");

    m_simulator_scene = new SimulatorScene(simulator, ui->simulator_graphics_view);
    ui->simulator_graphics_view->setScene(m_simulator_scene);
    // Prevent resizing
    this->setFixedSize(this->size());
}

void SimulatorWindow::keyPressEvent(QKeyEvent *event) {
    // Forward key press events to the main window
    QWidget *parent = parentWidget();
    if (!parent) return;
    MainWindow *main_window = (MainWindow*) parent;
    main_window->keyPressEvent(event);
}

void SimulatorWindow::reject() {
    // When the user clicks exit, switch back to ACTUATOR
    QWidget *parent = parentWidget();
    if (!parent) return;
    MainWindow *main_window = (MainWindow*) parent;
    main_window->switchToActuator();
    QDialog::reject();
}

SimulatorWindow::~SimulatorWindow() {
    delete m_simulator_scene;
    delete ui;
}

SimulatorScene *SimulatorWindow::getSimulatorScene() {
    return m_simulator_scene;
}
