#include "controller.h"
#include "../utility/logger.h"

Controller::Controller(bool invert_x, bool invert_y) :
    m_invert_x(invert_x),
    m_invert_y(invert_y) {}

vector2i Controller::to_vector2i(Dir dir) {
    vector2i vector_dir(0, 0);
    switch (dir) {
        case UP:
            vector_dir.y() = -1;
            break;
        case DOWN:
            vector_dir.y() = 1;
            break;
        case RIGHT:
            vector_dir.x() = 1;
            break;
        case LEFT:
            vector_dir.x() = -1;
            break;
        default:
#ifndef NDEBUG
            fatal() << "Invalid direction for movement: " << dir;
#endif
            return vector_dir;
    }
    return vector_dir;
}

void Controller::invertAxis(Axis axis) {
    switch (axis) {
        case X:
            m_invert_x = !m_invert_x;
            break;
        case Y:
            m_invert_y = !m_invert_y;
            break;
        default:
#ifndef NDEBUG
            fatal() << "Invalid axis for inversion: " << axis;
#endif
            break;
    }
}

void Controller::keyPressed(int key) {
#ifndef NDEBUG
    debug() << "Keypressed " << key;
#endif
    auto it = m_keyMap.find(key);
    if (it != m_keyMap.end()) {
        m_keyMap.erase(key);
    }
    m_keyMap.insert(key_press(key, true));
}

void Controller::keyReleased(int key) {
#ifndef NDEBUG
    debug() << "Keyreleased " << key;
#endif
    auto it = m_keyMap.find(key);
    if (it != m_keyMap.end()) {
        m_keyMap.erase(key);
    }
    m_keyMap.insert(key_press(key, false));
}

bool Controller::isKeyDown(int key) {
    auto it = m_keyMap.find(key);
    if (it == m_keyMap.end()) {
        return false;
    }
    return it->second;
}

void Controller::move(Dir dir, int timer) {
    move(Controller::to_vector2i(dir), timer);
}

void Controller::move(vector2i dir, int step_time) {
    __move_delegate({dir.x() * (m_invert_x ? -1 : 1), dir.y() * (m_invert_y ? -1 : 1)}, step_time);
}

void Controller::invert_x_axis() {
    m_invert_x = !m_invert_x;
}

void Controller::invert_y_axis() {
    m_invert_y = !m_invert_y;
}
