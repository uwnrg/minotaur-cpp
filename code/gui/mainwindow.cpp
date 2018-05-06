#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "ui_serialbox.h"

#include "../interpreter/embeddedcontroller.h"
#include "../compstate/parammanager.h"

param_manager *g_pm = nullptr;

MainWindow::MainWindow() :
    ui(std::make_unique<Ui::MainWindow>()),

    m_status_box(std::make_shared<StatusBox>(this)),
    m_global_sim(std::make_shared<GlobalSim>()),
    m_parameter_box(std::make_shared<ParameterBox>(this)),

    m_solenoid(std::make_shared<Solenoid>()),
    m_simulator(std::make_shared<Simulator>(m_global_sim.get())),
    m_controller(m_solenoid),

    m_about_window(std::make_unique<ActionAbout>(this)),
    m_camera_display(std::make_unique<CameraDisplay>(this)),
    m_script_window(std::make_unique<ScriptWindow>(this)),

    m_serial_box(std::make_unique<SerialBox>(m_solenoid, this)),
    m_simulator_window(std::make_unique<SimulatorWindow>(m_simulator, this)),

    m_controller_type(Controller::SOLENOID),

    m_compstate(this) {

    ui->setupUi(this);

    // Set up logger
    Logger::setStream(getLogView());

    // Bind controller to Python Engine
    EmbeddedController::getInstance().bind_controller(&m_controller);
    PythonEngine::getInstance().append_module("emb", &Embedded::PyInit_emb);
    PythonEngine::getInstance().append_module("sim", &Embedded::PyInit_sim);

    // Connect solenoid serial port to the monitor
    connect(m_solenoid.get(), &Solenoid::serialRead, m_serial_box.get(), &SerialBox::append_text);

    // Simulator and controls
    connect(ui->switch_to_simulator_mode, &QAction::triggered, this, &MainWindow::switchToSimulator);
    connect(ui->move_button, &QPushButton::clicked, this, &MainWindow::moveButtonClicked);

    // Opening sub windows
    connect(ui->start_python_interpreter, &QAction::triggered, m_script_window.get(), &QDialog::show);
    connect(ui->open_about, &QAction::triggered, m_about_window.get(), &QDialog::show);
    connect(ui->open_camera_display, &QAction::triggered, m_camera_display.get(), &QDialog::show);
    connect(ui->open_serial_box, &QAction::triggered, m_serial_box.get(), &QDialog::show);
    connect(ui->open_status_box, &QAction::triggered, m_status_box.get(), &QDialog::show);
    connect(ui->open_parameter_box, &QAction::triggered, m_parameter_box.get(), &QDialog::show);

    // Drop-down actions
    connect(ui->action_clear_log, &QAction::triggered, this, &MainWindow::clearLogOutput);
    connect(ui->action_invert_x_axis, &QAction::triggered, this, &MainWindow::invertControllerX);
    connect(ui->action_invert_y_axis, &QAction::triggered, this, &MainWindow::invertControllerY);

    // setup focus and an event filter to capture key events
    this->installEventFilter(this);
    this->setFocus();
    this->setFixedSize(this->size());

    // Initialize global parameter manager
    g_pm = new param_manager(this);
}

MainWindow::~MainWindow() {
    // Delete global parameter manager
    delete g_pm;
}

bool MainWindow::eventFilter(QObject *, QEvent *event) {
    // When the GUI gets focused, we assign the focus to this object, necessary for correctly
    // receiving key events
    if (event->type() == QEvent::FocusIn) {
        this->setFocus();
    }
    return false;
}

QTextEdit *MainWindow::getLogView() {
    return ui->log_viewer;
}

void MainWindow::keyPressEvent(QKeyEvent *e) {
    if (e->isAutoRepeat()) {
        return;
    }
    m_controller->keyPressed(e->key());
    switch (e->key()) {
        case Qt::Key_Up:
            m_controller->move(Controller::Dir::UP);
            break;

        case Qt::Key_Down:
            m_controller->move(Controller::Dir::DOWN);
            break;

        case Qt::Key_Right:
            m_controller->move(Controller::Dir::RIGHT);
            break;

        case Qt::Key_Left:
            m_controller->move(Controller::Dir::LEFT);
            break;

        default:
            break;
    }
}

void MainWindow::keyReleaseEvent(QKeyEvent *e) {
    if (e->isAutoRepeat()) {
        return;
    }
    m_controller->keyReleased(e->key());
}

void MainWindow::mousePressEvent(QMouseEvent *) {
    // When the user clicks outside a widget,
    // restore focus to the main window
    this->setFocus();
}

void MainWindow::moveButtonClicked() {
    auto dir = (Controller::Dir) ui->selected_direction->currentIndex();
    m_controller->move(dir);
}

void MainWindow::switchControllerTo(Controller::Type const type) {
#ifndef NDEBUG
    debug() << "Switch controller button clicked";
#endif
    // Do nothing if the same controller type is selected
    if (m_controller_type == type) {
#ifndef NDEBUG
        debug() << "No controller change";
#endif
        return;
    }
    m_controller_type = type;
    switch (type) {
        case Controller::Type::SOLENOID:
            // Switch to the solenoid controller and hide the simulation window
            log() << "Switching to SOLENOID";
            m_controller = m_solenoid;
            if (m_simulator_window->isVisible()) { m_simulator_window->hide(); }
            break;
        case Controller::Type::SIMULATOR:
            // Switch to the simulator controller and show the simulator window
            log() << "Switching to SIMULATOR";
            m_controller = m_simulator;
            if (!m_simulator_window->isVisible()) { m_simulator_window->show(); }
            break;
        default:
            break;
    }
}

void MainWindow::clearLogOutput() {
    Logger::clear_log();
}

void MainWindow::invertControllerX() {
    log() << "Inverting X-axis";
    m_controller->invert_x_axis();
}

void MainWindow::invertControllerY() {
    log() << "Inverting Y-axis";
    m_controller->invert_y_axis();
}

std::weak_ptr<Controller> MainWindow::controller() const {
    return m_controller;
}

std::weak_ptr<Solenoid> MainWindow::solenoid() const {
    return m_solenoid;
}

std::weak_ptr<StatusBox> MainWindow::status_box() const {
    return m_status_box;
}

std::weak_ptr<ParameterBox> MainWindow::param_box() const {
    return m_parameter_box;
}

std::weak_ptr<GlobalSim> MainWindow::global_sim() const {
    return m_global_sim;
}

CompetitionState &MainWindow::state() {
    return m_compstate;
}
