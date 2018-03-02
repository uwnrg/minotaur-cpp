#include "mainwindow.h"
#include "ui_mainwindow.h"

#include "../interpreter/embeddedcontroller.h"

MainWindow::MainWindow(
    int argc,
    char *argv[],
    QWidget *parent,
    const char *
) :
    QMainWindow(parent),
    m_serial_monitor(new SerialMonitor(this)),
    ui(new Ui::MainWindow) {

    ui->setupUi(this);

    // Set up logger
    Logger::setStream(getLogView());

    // Setup the controllers and solenoid connection
    m_simulator = std::make_shared<Simulator>();
    // If a port is passed use it TODO: QCommandLineParser
    if (argc >= 2) {
        m_solenoid = std::make_shared<Solenoid>(argv[1]);
    } else {
        m_solenoid = std::make_shared<Solenoid>();
    }
    m_controller = m_solenoid;
    m_controller_type = Controller::Type::SOLENOID;

    // Bind controller to Python Engine
    EmbeddedController::getInstance().bind_controller(&m_controller);
    PythonEngine::getInstance().append_module("emb", &Embedded::PyInit_emb);

    // Setup sub-windows
    m_simulator_window = new SimulatorWindow(m_simulator, this);
    m_script_window = new ScriptWindow(this);
    m_about_window = new ActionAbout(this);
    m_camera_display = new CameraDisplay(this);

    // Connect solenoid serial port to the monitor
    connect(m_solenoid.get(), &Solenoid::serialRead, m_serial_monitor.get(), &SerialMonitor::append_text);

    // Setup slot connections
    connect(ui->switch_to_simulator_mode, SIGNAL(triggered()), this, SLOT(switchToSimulator()));
    connect(ui->start_python_interpreter, SIGNAL(triggered()), this, SLOT(openPythonInterpreter()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(openActionAbout()));
    connect(ui->actionWebcam_View, SIGNAL(triggered()), this, SLOT(openCameraDisplay()));

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
    delete m_about_window;
    delete m_simulator_window;
    delete m_script_window;
    delete m_camera_display;
    delete ui;
}

void MainWindow::onMoveButtonClicked() {
    auto dir = (Controller::Dir) ui->selected_direction->currentIndex();
    m_controller->move(dir);
}

void MainWindow::switchControllerTo(Controller::Type const type) {
#ifndef NDEBUG
    debug()<< "Switch controller button clicked";
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

void MainWindow::openSerialMonitor() {
    m_serial_monitor->show();
}

void MainWindow::openPythonInterpreter() {
    m_script_window->show();
}

void MainWindow::openActionAbout() {
    m_about_window->show();
}

void MainWindow::openCameraDisplay() {
    m_camera_display->show();
}
