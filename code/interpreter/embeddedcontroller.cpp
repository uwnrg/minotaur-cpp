#include "embeddedcontroller.h"

EmbeddedController::EmbeddedController() :
        m_controller_ptr(nullptr) {}

void EmbeddedController::bind_controller(std::shared_ptr<Controller> *controller_ptr) {
    this->m_controller_ptr = controller_ptr;
}

bool EmbeddedController::send_movement(Vector2i &move_vector) {
    // Forward movement to the currently selected controller
    if (!m_controller_ptr || !*m_controller_ptr) { return false; }
    (*m_controller_ptr)->move(move_vector);
    return true;
}