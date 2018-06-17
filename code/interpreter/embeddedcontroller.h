#ifndef MINOTAUR_CPP_EMBEDDEDCONTROLLER_H
#define MINOTAUR_CPP_EMBEDDEDCONTROLLER_H

#include <memory>

// Forward declarations
namespace nrg {
    template<typename val_t> class vector;
}
class Controller;
typedef nrg::vector<int> vector2i;
// Python forward declarations
struct _object;
typedef _object PyObject;
typedef struct PyMethodDef PyMethodDef;
typedef struct PyModuleDef PyModuleDef;

// Singleton controller instance
class EmbeddedController {
public:
    static EmbeddedController &getInstance();

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
    bool send_movement(vector2i &move_vector);

    bool move_right();
    bool move_left();
    bool move_up();
    bool move_down();
};

namespace Embedded {
    // Embedded move function exposed to Python API
    extern PyObject *emb_move(PyObject *, PyObject *args);
    extern PyObject *emb_right(PyObject *, PyObject *);
    extern PyObject *emb_left(PyObject *, PyObject *);
    extern PyObject *emb_up(PyObject *, PyObject *);
    extern PyObject *emb_down(PyObject *, PyObject *);
    extern PyObject *emb_robot_rect(PyObject *, PyObject *);
    extern PyObject *emb_object_rect(PyObject *, PyObject *);
    extern PyObject *emb_robot_pos(PyObject *, PyObject *);
    extern PyObject *emb_object_pos(PyObject *, PyObject *);
    extern PyObject *sim_reset(PyObject *, PyObject *);

    extern PyMethodDef emb_methods[];
    extern PyMethodDef sim_methods[];
    extern PyModuleDef emb_module;
    extern PyModuleDef sim_module;

    extern PyObject *PyInit_emb();
    extern PyObject *PyInit_sim();
}

#endif //MINOTAUR_CPP_EMBEDDEDCONTROLLER_H
