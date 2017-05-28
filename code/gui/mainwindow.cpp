#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, const char *title) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {

    ui->setupUi(this);

	//Set up logger
	Logger::setStream(getLogView());
	m_actuator = std::shared_ptr<Actuator>(new Actuator);
	m_simulator = std::shared_ptr<Simulator>(new Simulator(1, -1));
    m_controller = m_actuator;
    m_controller_type = Controller::Type::ACTUATOR;

    // Setup subwindows
    actuator_setup_window = new ActuatorSetup(m_actuator, this);
    simulator_window = new SimulatorWindow(m_simulator, this);
    m_simulator->setSimulatorScene(simulator_window->getSimulatorScene());
    action_about_window = new ActionAbout();

    // Setup slot connections
    connect(ui->setup_actuator, SIGNAL(triggered()), this, SLOT(openActuatorSetup()));
    connect(ui->switch_to_actuator_mode, SIGNAL(triggered()), this, SLOT(switchToActuator()));
    connect(ui->switch_to_simulator_mode, SIGNAL(triggered()), this, SLOT(switchToSimulator()));
    connect(ui->actionAbout, SIGNAL(triggered()), this, SLOT(openActionAbout()));

    // setup focus and an event filter to capture key events
    this->installEventFilter(this);
    this->setFocus();
}

bool MainWindow::eventFilter(QObject *object, QEvent *event) {
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
    // TODO: Eventually we would want to generalize the key events so that we can configure
    // what keys we want to map. This is okay for now.
#ifndef NDEBUG
    Logger::log("Keypressed " + std::to_string(e->key()), Logger::DEBUG);
#endif
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
            // We don't do anything if the key pressed is not supported
            // We could add a beep sound or something to indicate that, but not a priority.
            break;
    }

}

void MainWindow::mousePressEvent(QMouseEvent *event) {
    // When the user clicks outside a widget,
    // restore focus to the main window
    this->setFocus();
}

MainWindow::~MainWindow() {
    // Destroy all subwindows
    delete actuator_setup_window;
    delete simulator_window;
    delete ui;
}

void MainWindow::on_move_button_clicked() {
    Controller::Dir dir = (Controller::Dir) ui->selected_direction->currentIndex();
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
    switch(type) {
    case Controller::Type::ACTUATOR:
        // Switch to the actuator controller and hide the simulation window
        Logger::log("Switching to ACTUATOR", Logger::INFO);
        m_controller = m_actuator;
        if (simulator_window->isVisible()) simulator_window->hide();
        break;
    case Controller::Type::SIMULATOR:
        // Switch to the simulator controller and show the simulator window
        Logger::log("Switching to SIMULATOR", Logger::INFO);
        m_controller = m_simulator;
        if (!simulator_window->isVisible()) simulator_window->show();
        break;
    default:
        break;
    }
}

void MainWindow::openActuatorSetup() {
    actuator_setup_window->show();
}

void MainWindow::openActionAbout() {
    action_about_window->show();
}
