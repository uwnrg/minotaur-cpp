#ifndef SCRIPTWINDOW_H
#define SCRIPTWINDOW_H

#include <QDialog>
#include <memory>

#include "../script-engine/scriptengine.h"

namespace Ui {
class ScriptWindow;
}

class ScriptWindow : public QDialog
{
    Q_OBJECT

public:
    explicit ScriptWindow(std::shared_ptr<ScriptEngine>&, QWidget *parent = 0);
    ~ScriptWindow();

private:
    Ui::ScriptWindow *ui;

    std::shared_ptr<ScriptEngine> m_script_engine;
};

#endif // SCRIPTWINDOW_H
