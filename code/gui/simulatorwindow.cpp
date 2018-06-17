#include "../utility/utility.h"
#include "../simulator/renderscene.h"

#include "ui_simulatorwindow.h"
#include "mainwindow.h"
#include "simulatorwindow.h"

#include <QGridLayout>

#ifndef NDEBUG
#include <QDebug>
#endif

SimulatorWindow::SimulatorWindow(std::shared_ptr<Simulator> simulator, QWidget *parent) :
    QDialog(parent),
    ui(std::make_unique<Ui::SimulatorWindow>()) {
    ui->setupUi(this);
    setWindowTitle("Simulator");

    m_simulator_scene = std::make_unique<RenderScene>(simulator, this);
    ui->renderLayout->addWidget(m_simulator_scene.get());

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

MainWindow *get_main_window_parent(SimulatorWindow *sim) {
    QWidget *parent = sim->parentWidget();
    auto main_window = reinterpret_cast<MainWindow *>(parent);
#ifndef NDEBUG
    if (!main_window) {
        qDebug() << "FATAL: SimulatorWindow parent is not MainWindow";
    }
#endif
    return main_window;
}

void SimulatorWindow::keyPressEvent(QKeyEvent *event) {
    get_main_window_parent(this)->keyPressEvent(event);
}

void SimulatorWindow::keyReleaseEvent(QKeyEvent *event) {
    get_main_window_parent(this)->keyReleaseEvent(event);
}

void SimulatorWindow::reject() {
    // When the user clicks exit, switch back to Solenoid
    m_simulator_scene->stopRender();
    get_main_window_parent(this)->switchToSolenoid();
    QDialog::reject();
}

SimulatorWindow::~SimulatorWindow() = default;