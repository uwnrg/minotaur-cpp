#include "scriptwindow.h"
#include "ui_scriptwindow.h"

ScriptWindow::ScriptWindow(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::ScriptWindow) {
    ui->setupUi(this);
    this->setWindowTitle("Python Interpreter");

    // Create text edit and text display
    m_interpreter_text_edit = new InterpreterTextEdit(this);
    m_results_text_display = new ResultsTextDisplay(this);
    ui->interpreterLayout->addWidget(m_interpreter_text_edit);
    ui->displayLayout->addWidget(m_results_text_display);

    QFont font;
    // Make the font look like code
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(8);
    QFontMetrics metrics(font);
    const int tabSize = 4;
    const int tabStopWidth = tabSize * metrics.width(' ');
    m_interpreter_text_edit->setFont(font);
    m_results_text_display->setFont(font);
    m_interpreter_text_edit->setTabStopWidth(tabStopWidth);
    m_results_text_display->setTabStopWidth(tabStopWidth);

    // Connect signals to slots
    connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetInterpreter()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(closeInterpreter()));
    connect(ui->runButton, SIGNAL(clicked()), this, SLOT(runScript()));
    connect(m_interpreter_text_edit, SIGNAL(scriptSubmitted()), this, SLOT(runScript()));
    connect(this, SIGNAL(scriptSubmitted(const QString&, const QString&, const QString&)),
            m_results_text_display, SLOT(appendResults(const QString&, const QString&, const QString&)));
}

ScriptWindow::~ScriptWindow() {
    delete ui;
    delete m_interpreter_text_edit;
    delete m_results_text_display;
}

void ScriptWindow::setVisible(bool visible) {
    if (visible) PythonEngine::getInstance().initialize();
    else PythonEngine::getInstance().stopEngine();
    QDialog::setVisible(visible);
}

void ScriptWindow::reject() {
    PythonEngine::getInstance().stopEngine();
    QDialog::reject();
}

// Clear the interpreter text fields and restart the engine
void ScriptWindow::resetInterpreter() {
    m_interpreter_text_edit->clear();
    m_results_text_display->clear();
    PythonEngine::getInstance().stopEngine();
    PythonEngine::getInstance().initialize();
}

void ScriptWindow::closeInterpreter() {
    PythonEngine::getInstance().stopEngine();
    this->reject();
}

// Parse a script from the interpreter text edit and send
// to python engine, then display the results
void ScriptWindow::runScript() {
    if (!PythonEngine::getInstance().isReady()) return;
    std::string script = m_interpreter_text_edit->toPlainText().toStdString();
    std::string *out = new std::string;
    std::string *err = new std::string;
    PythonEngine::getInstance().run(script, out, err);
    // Emit signal to output display to append results
    Q_EMIT scriptSubmitted(
            QString::fromStdString(script),
            QString::fromStdString(*out),
            QString::fromStdString(*err));
    m_interpreter_text_edit->clear();
    delete out;
    delete err;
}