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
#include "../camera/cameradisplay.h"

#include "simulatorwindow.h"
#include "actionabout.h"
#include "scriptwindow.h"
#include "serialmonitor.h"

#define DEFAULT_TITLE "minotaur"

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(
        int argc,
        char *argv[],
        QWidget *parent = nullptr,
        const char *title = DEFAULT_TITLE
    );

    ~MainWindow() override;

    QTextEdit *getLogView();

    void keyPressEvent(QKeyEvent *) override;

    void keyReleaseEvent(QKeyEvent *) override;


public Q_SLOTS:

    void openPythonInterpreter();

    void openActionAbout();

    void openCameraDisplay();

    void openSerialMonitor();

    /**
     * Clear the logging output, if the logging
     * has been set to the output field.
     */
    void clearLogOutput();

    /**
     * Invert the x-axis of the currently active controller.
     */
    void invertControllerX();

    /**
     * Invert the y-axis of the currently active controller;
     */
    void invertControllerY();

    inline void switchToSolenoid() { switchControllerTo(Controller::Type::SOLENOID); }

    inline void switchToSimulator() { switchControllerTo(Controller::Type::SIMULATOR); }


private Q_SLOTS:

    // Button click events
    void onMoveButtonClicked();

    // Mouse events
    void mousePressEvent(QMouseEvent *event) override;

private:
    std::unique_ptr<Ui::MainWindow> ui;

    std::unique_ptr<ScriptWindow> m_script_window;
    std::unique_ptr<SimulatorWindow> m_simulator_window;
    std::unique_ptr<ActionAbout> m_about_window;
    std::unique_ptr<CameraDisplay> m_camera_display;
    std::unique_ptr<SerialMonitor> m_serial_monitor;

    Controller::Type m_controller_type;

    std::shared_ptr<Controller> m_controller;
    std::shared_ptr<Solenoid> m_solenoid;
    std::shared_ptr<Simulator> m_simulator;

    bool eventFilter(QObject *, QEvent *) override;

    void switchControllerTo(Controller::Type type);
};

#endif // MAINWINDOW_H
