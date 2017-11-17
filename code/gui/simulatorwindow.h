#ifndef SIMULATORWINDOW_H
#define SIMULATORWINDOW_H

#include <QDialog>
#include <memory>

class Simulator;
class SimulatorScene;
class MainWindow;

namespace Ui {
class SimulatorWindow;
}

class SimulatorWindow : public QDialog {
    Q_OBJECT

public:
    explicit SimulatorWindow(std::shared_ptr<Simulator> simulator, QWidget *parent = 0);
    void keyPressEvent(QKeyEvent*) override;
    void reject() override;
    SimulatorScene *getSimulatorScene();
    ~SimulatorWindow() override;

private:
    Ui::SimulatorWindow *ui;
    // Graphics scene for simulator
    SimulatorScene *m_simulator_scene;
};

#endif // SIMULATORWINDOW_H
