#ifndef SIMULATORSCENE_H
#define SIMULATORSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <memory>

class Simulator;

// Graphics scene where the simulation is drawn
class SimulatorScene : public QGraphicsScene {
public:
    SimulatorScene(std::shared_ptr<Simulator> &simulator_controller, QObject *parent);
    // Redraw the scene with new positions
    void update();
    ~SimulatorScene();

private:
    // Pointer to robot graphics
    QGraphicsItem *robot_graphics;
    std::shared_ptr<Simulator> m_simulator_controller;
};

#endif // SIMULATORSCENE_H
