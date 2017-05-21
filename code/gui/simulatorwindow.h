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

class SimulatorWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SimulatorWindow(std::shared_ptr<Simulator>& controller, QWidget *parent = 0);
    void keyPressEvent(QKeyEvent*);
    ~SimulatorWindow();

private:
    Ui::SimulatorWindow *ui;
    SimulatorScene *m_simulator_scene;
    std::shared_ptr<Simulator> m_simulator;
};

#endif // SIMULATORWINDOW_H
