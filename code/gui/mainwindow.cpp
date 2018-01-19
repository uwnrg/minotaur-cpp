#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../interpreter/embeddedcontroller.h"

MainWindow::MainWindow(QWidget *parent, const char *) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {

    ui->setupUi(this);

    //Set up logger
    Logger::setStream(getLogView());
    m_actuator = std::shared_ptr<Actuator>(new Actuator);
    m_solenoid = std::shared_ptr<Solenoid>(new Solenoid);
    m_simulator = std::shared_ptr<Simulator>(new Simulator(1, -1));
    m_controller = m_solenoid;
    m_controller_type = Controller::Type::SOLENOID;

    // Bind controller to Python Engine
    EmbeddedController::getInstance().bind_controller(&m_controller);
    PythonEngine::getInstance().append_module("emb", &Embedded::PyInit_emb);

    // Setup subwindows
    actuator_setup_window = new ActuatorSetup(m_actuator, this);
    simulator_window = new SimulatorWindow(m_simulator, this);
    script_window = new ScriptWindow(this);
    //m_simulator->setSimulatorScene(simulator_window->getSimulatorScene());
    action_about_window = new ActionAbout();

    // Setup slot connections
    connect(ui->setup_actuator, SIGNAL(triggered()), this, SLOT(openActuatorSetup()));
    connect(ui->switch_to_actuator_mode, SIGNAL(triggered()), this, SLOT(switchToActuator()));
    connect(ui->switch_to_simulator_mode, SIGNAL(triggered()), this, SLOT(switchToSimulator()));
    connect(ui->start_python_interpreter, SIGNAL(triggered()), this, SLOT(openPythonInterpreter()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(openActionAbout()));

    // setup focus and an event filter to capture key events
    this->installEventFilter(this);
    this->setFocus();
    this->setFixedSize(this->size());
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

MainWindow::~MainWindow() {
    // Destroy all subwindows
    delete actuator_setup_window;
    delete simulator_window;
    delete script_window;
    delete ui;
}

void MainWindow::onMoveButtonClicked() {
    auto dir = (Controller::Dir) ui->selected_direction->currentIndex();
    m_controller->move(dir);
}

void MainWindow::switchControllerTo(Controller::Type const type) {
#ifndef NDEBUG
    Logger::log("Switch controller button clicked", Logger::DEBUG);
#endif
    // Do nothing if the same controller type is selected
    if (m_controller_type == type) {
#ifndef NDEBUG
        Logger::log("No controller change", Logger::DEBUG);
#endif
        return;
    }
    m_controller_type = type;
    switch (type) {
        case Controller::Type::ACTUATOR:
            // Switch to the actuator controller and hide the simulation window
            Logger::log("Switching to ACTUATOR", Logger::INFO);
            m_controller = m_actuator;
            if (simulator_window->isVisible()) { simulator_window->hide(); }
            break;
        case Controller::Type::SIMULATOR:
            // Switch to the simulator controller and show the simulator window
            Logger::log("Switching to SIMULATOR", Logger::INFO);
            m_controller = m_simulator;
            if (!simulator_window->isVisible()) { simulator_window->show(); }
            break;
        default:
            break;
    }
}

void MainWindow::openActuatorSetup() {
    actuator_setup_window->show();
}

void MainWindow::openPythonInterpreter() {
    script_window->show();
}

void MainWindow::openActionAbout() {
    action_about_window->show();
}
