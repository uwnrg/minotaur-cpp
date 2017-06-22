#ifndef MINOTAUR_CPP_PYTHONENGINE_H
#define MINOTAUR_CPP_PYTHONENGINE_H
#undef slots // prevent Qt macro from interfering
#define _hypot hypot // workaround for a bug in Python.h
#include <Python.h>
#include <QTextEdit>

class PythonEngine {
public:
    static PythonEngine &getInstance() {
        static PythonEngine engine;
        return engine;
    }
    void setStream(QTextEdit *output_stream);

private:
    std::string m_buffer;
    QTextEdit *m_outfield;

    PyObject *m_main_module;
    PyObject *m_stdout;
    PyObject *m_stderr;

    PythonEngine();
    ~PythonEngine();

public:
    PythonEngine(PythonEngine const&) = delete;
    void operator=(PythonEngine const&) = delete;
};


#endif //MINOTAUR_CPP_PYTHONENGINE_H
