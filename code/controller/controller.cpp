#include "controller.h"

void Controller::move(Dir dir) {
	Vector2i vector_dir;

	switch (dir) {
	case UP:
		vector_dir.y = m_invert_y;
		break;
	case DOWN:
		vector_dir.y = -1 * m_invert_y;
		break;
	case RIGHT:
		vector_dir.x = m_invert_x;
		break;
	case LEFT:
		vector_dir.x = -1 * m_invert_x;
		break;
	default:
		Logger::log("Invalid direction specified for movement: " + dir, Logger::ERROR);
		return;
	}
	move(vector_dir);
}

void Controller::invertAxis(Axis axis) {
	switch (axis) {
	case X:
		m_invert_x *= -1;
		break;
	case Y:
		m_invert_y *= -1;
		break;
	default:
		Logger::log("Invalid axis specified for inversion: " + axis, Logger::ERROR);
		break;
	}
}