#include "scriptengine.h"

ScriptEngine::ScriptEngine(const std::shared_ptr<Controller>& controller) :
    m_controller(controller) {}

void ScriptEngine::setController (const std::shared_ptr<Controller>& controller) {
    m_controller = controller;
}

int ScriptEngine::runScript (const std::string python_file) const {
    return 1;
}

ScriptEngine::~ScriptEngine() {}
