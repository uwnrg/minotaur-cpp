#ifndef PYTHONINTERPRETER_H
#define PYTHONINTERPRETER_H

#include <QDialog>
#include <QKeyEvent>
#include <QTextEdit>

class InterpreterTextEdit;

namespace Ui {
    class PythonInterpreter;
}

class PythonInterpreter : public QDialog {
Q_OBJECT

public:
    explicit PythonInterpreter(QWidget *parent = 0);
    ~PythonInterpreter();

private Q_SLOTS:
    void resetInterpreter();

private:
    Ui::PythonInterpreter *ui;
    InterpreterTextEdit *interpreter_text_edit;
};

class InterpreterTextEdit : public QTextEdit {
Q_OBJECT

public:
    InterpreterTextEdit(QWidget *parent = 0);
    ~InterpreterTextEdit();

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void focusInEvent(QFocusEvent *e) override;
};

#endif // PYTHONINTERPRETER_H
