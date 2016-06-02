#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "../controller/actuator.h"

MainWindow::MainWindow(QWidget *parent, const char *title) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Setup subwindows
    actuator_setup_window = new ActuatorSetup(m_controller);


    // Setup slot connections
    connect(ui->setup_actuator, SIGNAL(triggered()), this, SLOT(OpenActuatorSetup()));
}

QTextEdit* MainWindow::GetLogView()
{
    return ui->log_viewer;
}

MainWindow::~MainWindow()
{
    // Destroy all subwindows
    delete actuator_setup_window;

    delete ui;
}

void MainWindow::on_move_button_clicked()
{
	Controller::Dir dir = (Controller::Dir)ui->selected_direction->itemData(ui->selected_direction->currentIndex()).toInt();
	m_controller->move(Controller::move(dir));
}

void MainWindow::OpenActuatorSetup()
{
    actuator_setup_window->show();
}
