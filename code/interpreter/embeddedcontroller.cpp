#include "embeddedcontroller.h"

EmbeddedController::EmbeddedController() :
    m_controller_ptr(nullptr) {}

void EmbeddedController::bind_controller(std::shared_ptr<Controller> *controller_ptr) {
    this->m_controller_ptr = controller_ptr;
}

bool EmbeddedController::send_movement(vector2i &move_vector) {
    // Forward movement to the currently selected controller
    if (!m_controller_ptr || !*m_controller_ptr) { return false; }
    (*m_controller_ptr)->move(move_vector);
    return true;
}

bool EmbeddedController::move_up() {
    if (!m_controller_ptr || !*m_controller_ptr) { return false; }
    (*m_controller_ptr)->move(Controller::Dir::UP);
    return true;
}

bool EmbeddedController::move_down() {
    if (!m_controller_ptr || !*m_controller_ptr) { return false; }
    (*m_controller_ptr)->move(Controller::Dir::DOWN);
    return true;
}

bool EmbeddedController::move_right() {
    if (!m_controller_ptr || !*m_controller_ptr) { return false; }
    (*m_controller_ptr)->move(Controller::Dir::RIGHT);
    return true;
}

bool EmbeddedController::move_left() {
    if (!m_controller_ptr || !*m_controller_ptr) { return false; }
    (*m_controller_ptr)->move(Controller::Dir::LEFT);
    return true;
}
