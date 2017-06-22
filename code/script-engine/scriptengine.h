#ifndef SCRIPTENGINE_H
#define SCRIPTENGINE_H

#include <string>
#include <memory>
#include "../controller/controller.h"

class ScriptEngine {
public:
    ScriptEngine(const std::shared_ptr<Controller>&);

    void setController(const std::shared_ptr<Controller>&);
    int runScript (const std::string python_file) const;

    ~ScriptEngine();
private:
    std::shared_ptr<Controller> m_controller;
};

#endif // SCRIPTENGINE_H
