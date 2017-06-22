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
    void keyPressEvent(QKeyEvent*);
    void reject();
    SimulatorScene *getSimulatorScene();
    ~SimulatorWindow();

private:
    Ui::SimulatorWindow *ui;
    SimulatorScene *m_simulator_scene;
};

#endif // SIMULATORWINDOW_H
