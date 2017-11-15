#include <QGridLayout>
#include "simulatorwindow.h"
#include "ui_simulatorwindow.h"
#include "mainwindow.h"
#include "../graphics/simulatorscene.h"

SimulatorWindow::SimulatorWindow(std::shared_ptr<Simulator> simulator, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimulatorWindow) {
    ui->setupUi(this);
    setWindowTitle("Simulator");

    m_simulator_scene = new RenderScene(simulator, this);
    ui->renderLayout->addWidget(m_simulator_scene);

    // Prevent resizing
    this->setFixedSize(this->size());
}

void SimulatorWindow::setVisible(bool visible) {
    QDialog::setVisible(visible);
    if (visible) {
        m_simulator_scene->startRender();
    } else {
        m_simulator_scene->stopRender();
    }
}

void SimulatorWindow::keyPressEvent(QKeyEvent *event) {
    // Forward key press events to the main window
    QWidget *parent = parentWidget();
    if (!parent) return;
    MainWindow *main_window = (MainWindow*) parent;
    main_window->keyPressEvent(event);
}

void SimulatorWindow::keyReleaseEvent(QKeyEvent *event) {
    QWidget *parent = parentWidget();
    if (!parent) return;
    MainWindow *mainWindow = (MainWindow*) parent;
    mainWindow->keyReleaseEvent(event);
}

void SimulatorWindow::reject() {
    // When the user clicks exit, switch back to ACTUATOR
    m_simulator_scene->stopRender();
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
