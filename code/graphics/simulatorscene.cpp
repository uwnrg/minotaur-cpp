#include <QGraphicsEllipseItem>

#include "simulatorscene.h"
#include "robotgraphicsitem.h"
#include "../utility/logger.h"
#include "../controller/simulator.h"

SimulatorScene::SimulatorScene(std::shared_ptr<Simulator> &simulator_controller, QObject *parent) :
        m_simulator_controller(simulator_controller),
        QGraphicsScene(parent) {
    // Create graphics items
    robot_graphics = new RobotGraphicsItem();
    this->addItem(robot_graphics);
}

SimulatorScene::~SimulatorScene() {
    delete robot_graphics;
}

void SimulatorScene::update() {
	Vector2i robot_pos = *m_simulator_controller->getRobotPos();
#ifndef NDEBUG
	Logger::log("Beginning graphic scene update", Logger::DEBUG);
	Logger::log("Robot position (" + std::to_string(robot_pos.x_comp) + ", " + std::to_string(robot_pos.y_comp) + ")",
	            Logger::DEBUG);
#endif
	robot_graphics->setX(robot_pos.x_comp);
	robot_graphics->setY(robot_pos.y_comp);
	QGraphicsScene::update(QRectF(-800, -800, 1600, 1600));
}