#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>

#include <QMainWindow>
#include <QTextBrowser>
#include <QKeyEvent>
#include <unordered_map>

#include "../controller/controller.h"
#include "../controller/solenoid.h"
#include "../controller/simulator.h"

#include "actuatorsetup.h"
#include "simulatorwindow.h"
#include "actionabout.h"
#include "scriptwindow.h"

#define DEFAULT_TITLE "minotaur"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, const char *title = DEFAULT_TITLE);

    QTextEdit *getLogView();

    void keyPressEvent(QKeyEvent *) override;

    void keyReleaseEvent(QKeyEvent *) override;

    ~MainWindow() override;

public Q_SLOTS:

    void openActuatorSetup();

    void openPythonInterpreter();

    inline void switchToActuator() { switchControllerTo(Controller::Type::ACTUATOR); }

    inline void switchToSimulator() { switchControllerTo(Controller::Type::SIMULATOR); }

    void openActionAbout();

private Q_SLOTS:

    // Button click events
    void onMoveButtonClicked();

    // Mouse events
    void mousePressEvent(QMouseEvent *event) override;

private:
    Ui::MainWindow *ui;
    ActuatorSetup *actuator_setup_window;
    ScriptWindow *script_window;
    SimulatorWindow *simulator_window;
    Controller::Type m_controller_type;
    ActionAbout *action_about_window;
    std::shared_ptr<Controller> m_controller;
    std::shared_ptr<Actuator> m_actuator;
    std::shared_ptr<Solenoid> m_solenoid;
    std::shared_ptr<Simulator> m_simulator;

    bool eventFilter(QObject *, QEvent *) override;

    void switchControllerTo(Controller::Type type);
};

#endif // MAINWINDOW_H
