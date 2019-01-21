#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <memory>
#include <QMainWindow>

// UI forward declaration
namespace Ui {
    class MainWindow;
}
// Qt forward declarations
class QKeyEvent;
class QTextEdit;
// Forward declarations
class ActionAbout;
class CameraDisplay;
class CompetitionState;
class Controller;
class GlobalSim;
class ParameterBox;
class ScriptWindow;
class SerialBox;
class Simulator;
class SimulatorWindow;
class Solenoid;
class StatusBox;

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    MainWindow();

    ~MainWindow() override;

    QTextEdit *getLogView();

    void keyPressEvent(QKeyEvent *) override;

    void keyReleaseEvent(QKeyEvent *) override;

public:
    std::weak_ptr<Controller> controller() const;
    std::weak_ptr<Solenoid> solenoid() const;
    std::weak_ptr<StatusBox> status_box() const;
    std::weak_ptr<ParameterBox> param_box() const;
    std::weak_ptr<GlobalSim> global_sim() const;

    CompetitionState &state();

public Q_SLOTS:

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

    void switchToSolenoid();
    void switchToSimulator();


private Q_SLOTS:

    // Mouse events
    void mousePressEvent(QMouseEvent *event) override;

private:
    bool eventFilter(QObject *, QEvent *) override;

    void switchControllerTo(int type);

private:
    std::unique_ptr<Ui::MainWindow> ui;

    std::shared_ptr<StatusBox> m_status_box;
    std::shared_ptr<GlobalSim> m_global_sim;
    std::shared_ptr<ParameterBox> m_parameter_box;

    std::shared_ptr<Solenoid> m_solenoid;
    std::shared_ptr<Simulator> m_simulator;
    std::shared_ptr<Controller> m_controller;

    std::unique_ptr<ActionAbout> m_about_window;
    std::unique_ptr<CameraDisplay> m_camera_display;
    std::unique_ptr<ScriptWindow> m_script_window;

    std::unique_ptr<SerialBox> m_serial_box;
    std::unique_ptr<SimulatorWindow> m_simulator_window;

    std::unique_ptr<CompetitionState> m_compstate;

    int m_controller_type;
};

#endif // MAINWINDOW_H
