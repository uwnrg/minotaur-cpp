#include "PythonEngine.h"
#include "../utility/logger.h"

PythonEngine::PythonEngine() {
    Logger::log("Initializing Python engine...", Logger::DEBUG);
    wchar_t program_name[] = L"Minotaur-CPP";
    Py_SetProgramName(program_name);
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
}

PythonEngine::~PythonEngine() {
    Py_Finalize();
}

void PythonEngine::setStream(QTextEdit *output_stream) {
    m_outfield = output_stream;
}
