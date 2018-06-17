#include "../compstate/compstate.h"
#include "../controller/controller.h"
#include "../gui/global.h"
#include "../simulator/globalsim.h"

#include "embeddedcontroller.h"
#include "python.h"

EmbeddedController &EmbeddedController::getInstance() {
    static EmbeddedController controller;
    return controller;
}

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

PyObject *Embedded::emb_move(PyObject *, PyObject *args) {
    int x = 0;
    int y = 0;
    if (!PyArg_ParseTuple(args, "ii", &x, &y)) {
        return PyLong_FromLong(-1);
    }
    vector2i move_vector(x, y);
    bool res = EmbeddedController::getInstance().send_movement(move_vector);
    return PyLong_FromLong(res);
}

PyObject *Embedded::emb_right(PyObject *, PyObject *) {
    bool res = EmbeddedController::getInstance().move_right();
    return PyLong_FromLong(res);
}

PyObject *Embedded::emb_left(PyObject *, PyObject *) {
    bool res = EmbeddedController::getInstance().move_left();
    return PyLong_FromLong(res);
}

PyObject *Embedded::emb_up(PyObject *, PyObject *) {
    bool res = EmbeddedController::getInstance().move_up();
    return PyLong_FromLong(res);
}

PyObject *Embedded::emb_down(PyObject *, PyObject *) {
    bool res = EmbeddedController::getInstance().move_down();
    return PyLong_FromLong(res);
}

PyObject *Embedded::emb_robot_rect(PyObject *, PyObject *) {
    cv::Rect2d robot_rect = Main::get()->state().get_robot_box(false);
    PyObject *rect_tuple = PyTuple_New(4);
    PyTuple_SetItem(rect_tuple, 0, PyFloat_FromDouble(robot_rect.x));
    PyTuple_SetItem(rect_tuple, 1, PyFloat_FromDouble(robot_rect.y));
    PyTuple_SetItem(rect_tuple, 2, PyFloat_FromDouble(robot_rect.width));
    PyTuple_SetItem(rect_tuple, 3, PyFloat_FromDouble(robot_rect.height));
    return rect_tuple;
}

PyObject *Embedded::emb_object_rect(PyObject *, PyObject *) {
    cv::Rect object_rect = Main::get()->state().get_object_box(false);
    PyObject *rect_tuple = PyTuple_New(4);
    PyTuple_SetItem(rect_tuple, 0, PyFloat_FromDouble(object_rect.x));
    PyTuple_SetItem(rect_tuple, 1, PyFloat_FromDouble(object_rect.y));
    PyTuple_SetItem(rect_tuple, 2, PyFloat_FromDouble(object_rect.width));
    PyTuple_SetItem(rect_tuple, 3, PyFloat_FromDouble(object_rect.height));
    return rect_tuple;
}

PyObject *Embedded::emb_robot_pos(PyObject *, PyObject *) {
    cv::Rect2d robot_rect = Main::get()->state().get_robot_box(false);
    PyObject *pos_tuple = PyTuple_New(2);
    PyTuple_SetItem(pos_tuple, 0, PyFloat_FromDouble(robot_rect.x + robot_rect.width / 2));
    PyTuple_SetItem(pos_tuple, 1, PyFloat_FromDouble(robot_rect.y + robot_rect.height / 2));
    return pos_tuple;
}


PyObject *Embedded::emb_object_pos(PyObject *, PyObject *) {
    cv::Rect2d object_rect = Main::get()->state().get_robot_box(false);
    PyObject *pos_tuple = PyTuple_New(2);
    PyTuple_SetItem(pos_tuple, 0, PyFloat_FromDouble(object_rect.x + object_rect.width / 2));
    PyTuple_SetItem(pos_tuple, 1, PyFloat_FromDouble(object_rect.y + object_rect.height / 2));
    return pos_tuple;
}

PyObject *Embedded::sim_reset(PyObject *, PyObject *) {
    Main::get()->global_sim().lock()->robot_reset();
    return PyLong_FromLong(true);
}


PyMethodDef Embedded::emb_methods[]{
    {"move",        Embedded::emb_move,        METH_VARARGS, "Send move command vector"},
    {"right",       Embedded::emb_right,       METH_VARARGS, "Send move right"},
    {"left",        Embedded::emb_left,        METH_VARARGS, "Send move left"},
    {"down",        Embedded::emb_down,        METH_VARARGS, "Send move down"},
    {"up",          Embedded::emb_up,          METH_VARARGS, "Send move up"},
    {"robot_rect",  Embedded::emb_robot_rect,  METH_VARARGS, "Get the current robot rectangle"},
    {"robot_pos",   Embedded::emb_robot_pos,   METH_VARARGS, "Get the current robot position"},
    {"object_rect", Embedded::emb_object_rect, METH_VARARGS, "Get the current object rectangle"},
    {"object_pos",  Embedded::emb_object_pos,  METH_VARARGS, "Get the current object position"},
    {nullptr,       nullptr, 0,                    nullptr}
};

PyMethodDef Embedded::sim_methods[]{
    {"reset", Embedded::sim_reset, METH_VARARGS, "Reset simulator"},
    {nullptr, nullptr, 0,              nullptr}
};

PyModuleDef Embedded::emb_module{
    PyModuleDef_HEAD_INIT, "emb", nullptr, -1, Embedded::emb_methods,
    nullptr, nullptr, nullptr, nullptr
};

PyModuleDef Embedded::sim_module{
    PyModuleDef_HEAD_INIT, "sim", nullptr, -1, Embedded::sim_methods,
    nullptr, nullptr, nullptr, nullptr
};

PyObject *Embedded::PyInit_emb() {
    return PyModule_Create(&Embedded::emb_module);
}

PyObject *Embedded::PyInit_sim() {
    return PyModule_Create(&Embedded::sim_module);
}
