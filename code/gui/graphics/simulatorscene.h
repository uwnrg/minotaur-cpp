#ifndef SIMULATORSCENE_H
#define SIMULATORSCENE_H

#include <QGraphicsScene>
#include <QGraphicsItem>
#include <memory>

class Simulator;

class SimulatorScene : public QGraphicsScene
{
public:
    SimulatorScene(std::shared_ptr<Simulator>& simulator_controller, QObject *parent);
	void update();
	~SimulatorScene();

private:
    std::shared_ptr<Simulator> m_simulator_controller;
	QGraphicsItem *robot_graphics;
};

#endif // SIMULATORSCENE_H
