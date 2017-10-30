#ifndef MINOTAUR_CPP_EMBEDDEDCONTROLLER_H
#define MINOTAUR_CPP_EMBEDDEDCONTROLLER_H

#undef slots
#define _hypot hypot
#include <Python.h>

#include <memory>

#include "../controller/controller.h"

class EmbeddedController {
public:
    static EmbeddedController &getInstance() {
        static EmbeddedController controller;
        return controller;
    }
private:
    // The shared pointer that indicates which controller is active
    std::shared_ptr<Controller> *m_controller_ptr;

    EmbeddedController();
    ~EmbeddedController();

public:
    EmbeddedController(EmbeddedController const &) = delete;
    void operator=(EmbeddedController const &) = delete;

    // Set a controller as the active controller
    void bind_controller(std::shared_ptr<Controller> *controller_ptr);
    // Send a movement to the active controller
    bool send_movement(Vector2i &move_vector);
};

namespace Embedded {
    // Embedded move function exposed to Python API
    static PyObject *emb_move(PyObject *self, PyObject *args) {
        int x = 0;
        int y = 0;
        if (!PyArg_ParseTuple(args, "ii", &x, &y))
            return PyLong_FromLong(-1);
        Vector2i move_vector(x, y);
        bool res = EmbeddedController::getInstance().send_movement(move_vector);
        return PyLong_FromLong(res);
    }

    static PyMethodDef emb_methods[]{
            {"move",  emb_move, METH_VARARGS, "Send move command to active controller."},
            {nullptr, nullptr, 0,                                 nullptr}
    };
    static PyModuleDef emb_module{
            PyModuleDef_HEAD_INIT, "emb", nullptr, -1, emb_methods,
            nullptr, nullptr, nullptr, nullptr
    };
    static PyObject *PyInit_emb() {
        return PyModule_Create(&emb_module);
    }
}

#endif //MINOTAUR_CPP_EMBEDDEDCONTROLLER_H
