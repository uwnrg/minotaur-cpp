#include "controller.h"

Controller::Controller(int t_invert_x, int t_invert_y) :
        m_invert_x(t_invert_x), m_invert_y(t_invert_y) {}

Vector2i Controller::toVec2(Dir dir) {
    Vector2i vector_dir(0, 0);

    switch (dir) {
        case UP:
            vector_dir.y_comp = 1;
            break;
        case DOWN:
            vector_dir.y_comp = -1;
            break;
        case RIGHT:
            vector_dir.x_comp = 1;
            break;
        case LEFT:
            vector_dir.x_comp = -1;
            break;
        default:
            Logger::log("Invalid direction specified for movement: " + std::to_string(dir), Logger::ERROR);
            return vector_dir;
    }
    return vector_dir;
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
            Logger::log("Invalid axis specified for inversion: " + std::to_string(axis), Logger::ERROR);
            break;
    }
}

void Controller::keyPressed(int key) {
#ifndef NDEBUG
    Logger::log("Keypressed " + std::to_string(key), Logger::DEBUG);
#endif
    auto it = m_keyMap.find(key);
    if (it != m_keyMap.end()) {
        m_keyMap.erase(key);
    }
    m_keyMap.insert(key_press(key, true));
}

void Controller::keyReleased(int key) {
#ifndef NDEBUG
    Logger::log("Keyreleased " + std::to_string(key), Logger::DEBUG);
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
    move(Controller::toVec2(dir), timer);
}
