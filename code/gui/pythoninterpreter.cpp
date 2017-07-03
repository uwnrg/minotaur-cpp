#include <code/interpreter/pythonengine.h>
#include "pythoninterpreter.h"
#include "ui_pythoninterpreter.h"

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
    this->interpreter_text_edit->clear();
    PythonEngine::getInstance().stopEngine();
    PythonEngine::getInstance().initialize();
}

InterpreterTextEdit::InterpreterTextEdit(QWidget *parent) :
        QTextEdit(parent) {
    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(12);
    const int tabSize = 4;
    QFontMetrics metrics(font);
    this->setFont(font);
    this->setTabStopWidth(tabSize * metrics.width(' '));
}

InterpreterTextEdit::~InterpreterTextEdit() {
}

void InterpreterTextEdit::focusInEvent(QFocusEvent *e) {
    QTextEdit::focusInEvent(e);
}

void InterpreterTextEdit::keyPressEvent(QKeyEvent *e) {
    // Process key event first
    QTextEdit::keyPressEvent(e);
    switch (e->key()) {
        case Qt::Key_Delete:
        case Qt::Key_Backspace:
            m_script = m_script.substr(0, m_script.length() - 1);
            break;
        case Qt::Key_Return:
        case Qt::Key_Enter:
            m_script += "\n";
            break;
        case Qt::Key_Tab:
            m_script += "\t";
            break;
        default:
            QString value = e->text();
            const char *chars = value.toUtf8().constData();
            if (value.length() == 1 && isValidChar(chars[0]))
                m_script.append(chars);
            break;
    }
    if (e->modifiers() & Qt::ShiftModifier)
        switch (e->key()) {
            case Qt::Key_Return:
            case Qt::Key_Enter:
                parseScript();
            default:
                break;
        }
}

bool InterpreterTextEdit::isValidChar(char c) {
    return 32 <= c && c <= 126; // ' ' <= c <= '~'
}

void InterpreterTextEdit::parseScript() {
    std::string *out = new std::string;
    std::string *err = new std::string;
    PythonEngine::getInstance().run(m_script, out, err);
    m_script = "";
    this->append(QString::fromStdString(*out));
    this->append(QString::fromStdString(*err));
    delete out;
    delete err;
}