#include "simulator.h"
#include "../gui/graphics/simulatorscene.h"

Simulator::Simulator(int t_invert_x, int t_invert_y) :
		Controller(t_invert_x, t_invert_y) {
}

void Simulator::move(Vector2i dir, int timer) {
	// Update the robot position
	robot_pos += dir * 10;
	Logger::log("Moved (" + std::to_string(dir.x_comp) + ", " + std::to_string(dir.y_comp) + ")",
	            Logger::INFO);

	// Update the graphics scene
	m_simulator_scene->update();
}

void Simulator::setSimulatorScene(SimulatorScene *simulator_scene) {
	m_simulator_scene = simulator_scene;
}

Vector2i *Simulator::getRobotPos() {
	return &robot_pos;
}

