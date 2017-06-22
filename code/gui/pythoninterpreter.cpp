#include "pythoninterpreter.h"
#include "ui_pythoninterpreter.h"
#include "../utility/logger.h"

PythonInterpreter::PythonInterpreter(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::PythonInterpreter) {
    ui->setupUi(this);
    setWindowTitle("Python Interpreter");

    interpreter_text_edit = new InterpreterTextEdit(this);
    ui->interpreterLayout->addWidget(interpreter_text_edit);

    connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetInterpreter()));
}

PythonInterpreter::~PythonInterpreter() {
    delete ui;
    delete interpreter_text_edit;
}

void PythonInterpreter::resetInterpreter() {
    Logger::log("Resetting interpreter", Logger::DEBUG);
}

InterpreterTextEdit::InterpreterTextEdit(QWidget *parent) :
        QTextEdit(parent) {
}

InterpreterTextEdit::~InterpreterTextEdit() {
}

void InterpreterTextEdit::focusInEvent(QFocusEvent *e) {
    QTextEdit::focusInEvent(e);
}

void InterpreterTextEdit::keyPressEvent(QKeyEvent *e) {
    switch (e->key()) {
        case Qt::Key_Return:
        case Qt::Key_Enter:
            break;
        default:
            break;
    }
    QTextEdit::keyPressEvent(e);
}