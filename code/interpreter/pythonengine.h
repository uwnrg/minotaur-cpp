#ifndef MINOTAUR_CPP_PYTHONENGINE_H
#define MINOTAUR_CPP_PYTHONENGINE_H


#define PyObject_GetAttrString(object, strName) PyObject_GetAttrString(object, strName)
#define PyObject_SetAttrString(object, strName, value) PyObject_SetAttrString(object, strName, value)

#include "../controller/controller.h"
#include "python.h"
#include <string>
#include <memory>

class PythonEngine {
public:
    static PythonEngine &getInstance() {
        static PythonEngine engine;
        return engine;
    }
private:
    // For redirecting Python output to results text display
    PyObject *m_main_module;
    PyObject *m_stdout;
    PyObject *m_stderr;
    PyObject *m_stdout_value;
    PyObject *m_stderr_value;

    std::vector<std::pair<std::string, PyObject * (*)(void)>> m_embedded_modules;

    PythonEngine();
    ~PythonEngine();

    std::string getStdout(bool clear = true);
    std::string getStderr(bool clear = true);
public:
    PythonEngine(PythonEngine const&) = delete;
    void operator=(PythonEngine const&) = delete;

    // Add an embedded python module defined elsewhere, exposing it to interpreter
    void append_module(std::string name, PyObject * (*init_func)());
    bool initialize();
    bool stopEngine();
    bool isReady();
    // Run the script and store the results
    bool run(std::string script, std::string *out, std::string *err);
};


#endif //MINOTAUR_CPP_PYTHONENGINE_H
