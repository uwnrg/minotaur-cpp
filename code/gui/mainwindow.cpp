#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent, const char *title) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {

    ui->setupUi(this);

    //Set up logger
    Logger::setStream(getLogView());

    // Set up controller
    m_control_delegator = std::shared_ptr<ControlDelegator>(new ControlDelegator);

    // Setup subwindows
    std::shared_ptr<Actuator> actuator = m_control_delegator->getActuator();
    actuator_setup_window = new ActuatorSetup(actuator, this);

    // Setup slot connections
    connect(ui->setup_actuator, SIGNAL(triggered()), this, SLOT(openActuatorSetup()));

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
    Logger::log("Keypressed " + std::to_string(e->key()), Logger::DEBUG);
    switch (e->key()) {
        case Qt::Key_Up:
            m_control_delegator->move(Controller::Dir::UP);
            break;

        case Qt::Key_Down:
            m_control_delegator->move(Controller::Dir::DOWN);
            break;

        case Qt::Key_Right:
            m_control_delegator->move(Controller::Dir::RIGHT);
            break;

        case Qt::Key_Left:
            m_control_delegator->move(Controller::Dir::LEFT);
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
    delete ui;
}

void MainWindow::on_move_button_clicked() {
    Controller::Dir dir = (Controller::Dir) ui->selected_direction->currentIndex();
    m_control_delegator->move(dir);
}

void MainWindow::on_controller_button_clicked() {
    int index = ui->controller_type->currentIndex();
    m_control_delegator->switchTo((ControlDelegator::Delegate) index);
}

void MainWindow::openActuatorSetup() {
    actuator_setup_window->show();
}
