#include "PythonEngine.h"

PythonEngine::PythonEngine() {
}

PythonEngine::~PythonEngine() {
    if (Py_IsInitialized()) Py_Finalize();
}

std::string PythonEngine::getStdout(bool clear) {
    m_stdout_value = PyObject_GetAttrString(m_stdout, "value");
    std::string str_val(PyUnicode_AsUTF8(m_stdout_value));
    if (clear) PyObject_SetAttrString(m_stdout, "value", PyUnicode_FromString(""));
    return str_val;
}

std::string PythonEngine::getStderr(bool clear) {
    m_stderr_value = PyObject_GetAttrString(m_stderr, "value");
    std::string str_val(PyUnicode_AsUTF8(m_stderr_value));
    if (clear) PyObject_SetAttrString(m_stderr, "value", PyUnicode_FromString(""));
    return str_val;
}

void PythonEngine::append_module(std::string name, PyObject *(*init_func)(void)) {
    std::pair<std::string, PyObject *(*)(void)> module(name, init_func);
    m_embedded_modules.push_back(module);
}

bool PythonEngine::initialize() {
    Logger::log("Initializing Python engine...", Logger::DEBUG);
    wchar_t program_name[] = L"Minotaur-CPP";
    Py_SetProgramName(program_name);
    for (unsigned int i = 0; i < m_embedded_modules.size(); i++)
        PyImport_AppendInittab(m_embedded_modules[i].first.c_str(), m_embedded_modules[i].second);
    Py_Initialize();

    // Add the import path for our scripts
    Logger::log("Adding import path for scripts", Logger::DEBUG);
    m_main_module = PyImport_AddModule("__main__");
    PyRun_SimpleString("import sys\n"
                               "root_path = sys.executable\n"
                               "root_path = root_path[:root_path.rfind('\\\\')]\n"
                               "sys.path.append(root_path + \"\\\\"
                               PYTHON_SCRIPT_DIR
                               "\")\n");
    Logger::log("Redirecting stdout and stderr", Logger::DEBUG);
    PyRun_SimpleString("class CatchOut:\n"
                               "\tdef __init__(self):\n"
                               "\t\tself.value = ''\n"
                               "\tdef write(self, txt):\n"
                               "\t\tself.value += txt\n"
                               "\tdef flush(self):\n"
                               "\t\tself.value = ''\n"
                               "catchStdout = CatchOut()\n"
                               "catchStderr = CatchOut()\n"
                               "sys.stdout = catchStdout\n"
                               "sys.stderr = catchStderr\n"
    );
    m_stdout = PyObject_GetAttrString(m_main_module, "catchStdout");
    m_stderr = PyObject_GetAttrString(m_main_module, "catchStderr");
    Logger::log("Python engine initialized", Logger::DEBUG);
    return (bool) Py_IsInitialized();
}

bool PythonEngine::stopEngine() {
    Logger::log("Stopping Python engine", Logger::DEBUG);
    Py_Finalize();
    m_main_module = nullptr;
    m_stderr = nullptr;
    m_stdout = nullptr;
    m_stdout_value = nullptr;
    m_stderr_value = nullptr;
    return !Py_IsInitialized();
}

bool PythonEngine::isReady() {
    return (bool) Py_IsInitialized();
}

bool PythonEngine::run(std::string script, std::string *out, std::string *err) {
    if (!Py_IsInitialized()) return false;
    PyRun_SimpleString(script.c_str());
    *out = getStdout();
    *err = getStderr();
    return err->empty();
}
