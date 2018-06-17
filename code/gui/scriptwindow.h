#ifndef SCRIPTWINDOW_H
#define SCRIPTWINDOW_H

#include "../interpreter/pythonengine.h"
#include "../interpreter/interpretertextedit.h"
#include "../interpreter/resultstextdisplay.h"
#include "scripteditor.h"

#include <QFileDialog>
#include <QDialog>
#include <memory>


namespace Ui {
    class ScriptWindow;
}

/**
 * The script window has two text areas, one in which
 * the user enters a python script, and the other which
 * displays the results in a scrolling manner. This
 * creates an interpreter environment similar to the commandline
 * python interpreter.
 */
class ScriptWindow : public QDialog {
Q_OBJECT

public:
    /**
     * Constructor initializes the views and connects signals.
     *
     * @param parent parent widget
     */
    explicit ScriptWindow(QWidget *parent = 0);
    /**
     * Destructor. Clean up the windows.
     */
    ~ScriptWindow() override;

protected:
    /**
     * Initialize the python engine if the window is being opened,
     * otherwise shut down the engine.
     *
     * @param visible true if the window is being opened, false otherwise
     */
    void setVisible(bool visible) override;

    /**
     * Shut down the python engine when the user exits.
     */
    void reject() override;

private Q_SLOTS:
    /**
     * The python environment is reset by shutting
     * down the interpreter and reinitializing.
     */
    void resetInterpreter();

    /**
     * SLOT which shuts down the interpreter and closes
     * the interpreter window.
     */
    void closeInterpreter();

    /**
     * This SLOT is called when the user submits a script.
     * This function will acquire the script text from the
     * interpreter text edit and submit the script to the
     * active python engine. The results are emitted so
     * that they are appended to the results text area.
     */
    void runScript();

    /**
     * SLOT which will open the file explorer dialog.
     */
    void openRunFile();

    /**
     * SLOT which will take a file selected from the QFileDialog,
     * open the file, read the script text, and submit the
     * text to the python engine.
     *
     * @param filePath the path to the script file
     */
    void processRunFile(const QString &filePath);

    /**
     * SLOT which opens the script editor.
     */
    void openScriptEditor();

Q_SIGNALS:
    /**
     * Signal emitted when a script has been submitted to the python engine.
     * The results text display accepts the signal and displays the
     * script results to the user.
     *
     * @param script the script text
     * @param out the script output
     * @param err the script error
     */
    void scriptSubmitted(const QString &script, const QString &out, const QString &err);

private:


private:
    std::unique_ptr<Ui::ScriptWindow> ui;
    /**
     * Text edit where the user writes python code.
     */
    std::unique_ptr<InterpreterTextEdit> m_interpreter_text_edit;
    /**
     * Text display where python output is appended.
     */
    std::unique_ptr<ResultsTextDisplay> m_results_text_display;
    /**
     * File explorer for python scripts.
     */
    std::unique_ptr<QFileDialog> m_file_dialog;
    /**
     * Python script editing window.
     */
    std::unique_ptr<ScriptEditor> m_script_editor;

};

#endif // SCRIPTWINDOW_H
