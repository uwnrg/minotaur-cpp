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
    // Get the robot position from the simulator controller
    // and set the position of the robot graphics on the scene
    Vector2i robot_pos = *m_simulator_controller->getRobotPos();
#ifndef NDEBUG
    Logger::log("Beginning graphic scene update", Logger::DEBUG);
    log(Logger::DEBUG) << "Robot position " << robot_pos;
#endif
    robot_graphics->setX(robot_pos.y());
    robot_graphics->setY(robot_pos.x());
    QGraphicsScene::update(QRectF(-800, -800, 1600, 1600));
}