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

    static PyObject *sim_reset(PyObject *, PyObject *) {
        Main::get()->global_sim().lock()->robot_reset();
        return PyLong_FromLong(true);
    }

    // Embedded python configuration which describes which methods
    // should be exposed in which module
    static PyMethodDef emb_methods[]{
        {"move",  emb_move,  METH_VARARGS, "Send move command vector"},
        {"right", emb_right, METH_VARARGS, "Send move right"},
        {"left",  emb_left,  METH_VARARGS, "Send move left"},
        {"down",  emb_down,  METH_VARARGS, "Send move down"},
        {"up",    emb_up,    METH_VARARGS, "Send move up"},
        {nullptr, nullptr, 0,              nullptr}
    };
    // Method 'move' is exposed in module 'emb' as 'emb.move'
    static PyModuleDef emb_module{
        PyModuleDef_HEAD_INIT, "emb", nullptr, -1, emb_methods,
        nullptr, nullptr, nullptr, nullptr
    };

    static PyObject *PyInit_emb() {
        return PyModule_Create(&emb_module);
    }

    static PyMethodDef sim_methods[]{
        {"reset", sim_reset, METH_VARARGS, "Reset simulator"},
        {nullptr, nullptr, 0,              nullptr}
    };
    static PyModuleDef sim_module{
        PyModuleDef_HEAD_INIT, "sim", nullptr, -1, sim_methods,
        nullptr, nullptr, nullptr, nullptr
    };

    static PyObject *PyInit_sim() {
        return PyModule_Create(&sim_module);
    }
}

#endif //MINOTAUR_CPP_EMBEDDEDCONTROLLER_H
