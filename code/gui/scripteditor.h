#ifndef SCRIPTEDITOR_H
#define SCRIPTEDITOR_H

#include <QDialog>

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
    Ui::ScriptEditor *ui;
    QString m_activeFile;
};

#endif // SCRIPTEDITOR_H
