#ifndef MINOTAUR_CPP_EMBEDDEDCONTROLLER_H
#define MINOTAUR_CPP_EMBEDDEDCONTROLLER_H

#include "python.h"

#include <memory>

#include "../controller/controller.h"
#include "../gui/mainwindow.h"

// Singleton controller instance
class EmbeddedController {
public:
    static EmbeddedController &getInstance() {
        static EmbeddedController controller;
        return controller;
    }

private:
    // The shared pointer that indicates which controller is active
    // so that commands issued from the engine can be forwarded
    std::shared_ptr<Controller> *m_controller_ptr;

    EmbeddedController();

public:
    // Singleton
    EmbeddedController(EmbeddedController const &) = delete;

    void operator=(EmbeddedController const &) = delete;

    // Set a controller as the active controller
    void bind_controller(std::shared_ptr<Controller> *controller_ptr);

    // Send a movement to the active controller
    bool send_movement(Vector2i &move_vector);

    bool move_right();

    bool move_left();

    bool move_up();

    bool move_down();
};

namespace Embedded {
    // Embedded move function exposed to Python API
    static PyObject *emb_move(PyObject *, PyObject *args) {
        int x = 0;
        int y = 0;
        if (!PyArg_ParseTuple(args, "ii", &x, &y)) {
            return PyLong_FromLong(-1);
        }
        Vector2i move_vector(x, y);
        bool res = EmbeddedController::getInstance().send_movement(move_vector);
        return PyLong_FromLong(res);
    }

    static PyObject *emb_right(PyObject *, PyObject *) {
        bool res = EmbeddedController::getInstance().move_right();
        return PyLong_FromLong(res);
    }

    static PyObject *emb_left(PyObject *, PyObject *) {
        bool res = EmbeddedController::getInstance().move_left();
        return PyLong_FromLong(res);
    }

    static PyObject *emb_up(PyObject *, PyObject *) {
        bool res = EmbeddedController::getInstance().move_up();
        return PyLong_FromLong(res);
    }

    static PyObject *emb_down(PyObject *, PyObject *) {
        bool res = EmbeddedController::getInstance().move_down();
        return PyLong_FromLong(res);
    }

    static PyObject *emb_robot_rect(PyObject *, PyObject *) {
        cv::Rect2d robot_rect = Main::get()->state().get_robot_box(false);
        PyObject *rect_tuple = PyTuple_New(4);
        PyTuple_SetItem(rect_tuple, 0, PyFloat_FromDouble(robot_rect.x));
        PyTuple_SetItem(rect_tuple, 1, PyFloat_FromDouble(robot_rect.y));
        PyTuple_SetItem(rect_tuple, 2, PyFloat_FromDouble(robot_rect.width));
        PyTuple_SetItem(rect_tuple, 3, PyFloat_FromDouble(robot_rect.height));
        return rect_tuple;
    }

    static PyObject *emb_object_rect(PyObject *, PyObject *) {
        cv::Rect object_rect = Main::get()->state().get_object_box(false);
        PyObject *rect_tuple = PyTuple_New(4);
        PyTuple_SetItem(rect_tuple, 0, PyFloat_FromDouble(object_rect.x));
        PyTuple_SetItem(rect_tuple, 1, PyFloat_FromDouble(object_rect.y));
        PyTuple_SetItem(rect_tuple, 2, PyFloat_FromDouble(object_rect.width));
        PyTuple_SetItem(rect_tuple, 3, PyFloat_FromDouble(object_rect.height));
        return rect_tuple;
    }

    static PyObject *emb_robot_pos(PyObject *, PyObject *) {
        cv::Rect2d robot_rect = Main::get()->state().get_robot_box(false);
        PyObject *pos_tuple = PyTuple_New(2);
        PyTuple_SetItem(pos_tuple, 0, PyFloat_FromDouble(robot_rect.x + robot_rect.width / 2));
        PyTuple_SetItem(pos_tuple, 1, PyFloat_FromDouble(robot_rect.y + robot_rect.height / 2));
        return pos_tuple;
    }

    static PyObject *emb_object_pos(PyObject *, PyObject *) {
        cv::Rect2d object_rect = Main::get()->state().get_robot_box(false);
        PyObject *pos_tuple = PyTuple_New(2);
        PyTuple_SetItem(pos_tuple, 0, PyFloat_FromDouble(object_rect.x + object_rect.width / 2));
        PyTuple_SetItem(pos_tuple, 1, PyFloat_FromDouble(object_rect.y + object_rect.height / 2));
        return pos_tuple;
    }

    static PyObject *sim_reset(PyObject *, PyObject *) {
        Main::get()->global_sim().lock()->robot_reset();
        return PyLong_FromLong(true);
    }

    static PyMethodDef emb_methods[]{
        {"move",        emb_move,        METH_VARARGS, "Send move command vector"},
        {"right",       emb_right,       METH_VARARGS, "Send move right"},
        {"left",        emb_left,        METH_VARARGS, "Send move left"},
        {"down",        emb_down,        METH_VARARGS, "Send move down"},
        {"up",          emb_up,          METH_VARARGS, "Send move up"},
        {"robot_rect",  emb_robot_rect,  METH_VARARGS, "Get the current robot rectangle"},
        {"robot_pos",   emb_robot_pos,   METH_VARARGS, "Get the current robot position"},
        {"object_rect", emb_object_rect, METH_VARARGS, "Get the current object rectangle"},
        {"object_pos",  emb_object_pos,  METH_VARARGS, "Get the current object position"},
        {nullptr,       nullptr, 0,                    nullptr}
    };

    static PyMethodDef sim_methods[]{
        {"reset", sim_reset, METH_VARARGS, "Reset simulator"},
        {nullptr, nullptr, 0,              nullptr}
    };

    static PyModuleDef emb_module{
        PyModuleDef_HEAD_INIT, "emb", nullptr, -1, emb_methods,
        nullptr, nullptr, nullptr, nullptr
    };

    static PyModuleDef sim_module{
        PyModuleDef_HEAD_INIT, "sim", nullptr, -1, sim_methods,
        nullptr, nullptr, nullptr, nullptr
    };

    static PyObject *PyInit_emb() {
        return PyModule_Create(&emb_module);
    }


    static PyObject *PyInit_sim() {
        return PyModule_Create(&sim_module);
    }
}

#endif //MINOTAUR_CPP_EMBEDDEDCONTROLLER_H
