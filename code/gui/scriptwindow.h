#ifndef SCRIPTWINDOW_H
#define SCRIPTWINDOW_H

#include <QDialog>
#include <memory>

#include "../interpreter/pythonengine.h"
#include "../interpreter/interpretertextedit.h"
#include "../interpreter/resultstextdisplay.h"

namespace Ui {
    class ScriptWindow;
}

class ScriptWindow : public QDialog {
Q_OBJECT

public:
    explicit ScriptWindow(QWidget *parent = 0);
    ~ScriptWindow() override;

protected:
    void setVisible(bool visible);
    void reject();

private Q_SLOTS:
    void resetInterpreter();
    void closeInterpreter();
    // Run the current buffered script
    void runScript();

Q_SIGNALS:
    // Signal fired when the user submits a python script
    void scriptSubmitted(const QString &script, const QString &out, const QString &err);

private:
    Ui::ScriptWindow *ui;
    // Where the user enters python commands
    InterpreterTextEdit *m_interpreter_text_edit;
    // Where the python output is displayed
    ResultsTextDisplay *m_results_text_display;
};

#endif // SCRIPTWINDOW_H
