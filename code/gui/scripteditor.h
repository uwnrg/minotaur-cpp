#ifndef SCRIPTEDITOR_H
#define SCRIPTEDITOR_H

#include <QDialog>
#include <code/interpreter/codeeditor.h>

namespace Ui {
    class ScriptEditor;
}

class ScriptEditor : public QDialog {
Q_OBJECT

public:
    explicit ScriptEditor(QWidget *parent = nullptr);
    ~ScriptEditor() override;

protected:
    void reject() override;

private Q_SLOTS:
    /**
     * Slot that opens the file explorer for a python
     * file to edit.
     */
    void openFile();
    /**
     * Slot that saves the current file that is
     * being edited. If no file is being edited
     * then redirects to save as.
     */
    void save();
    /**
     * Save the current text as a new file.
     */
    void saveAs();
    /**
     * Discard current changes and create a new file.
     */
    void newFile();

private:
    std::unique_ptr<Ui::ScriptEditor> ui;
    std::unique_ptr<CodeEditor> m_code_editor;
    QString m_active_file;
};

#endif // SCRIPTEDITOR_H
