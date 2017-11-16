#include <QDir>
#include <QtCore/QTextStream>

#include "scriptwindow.h"
#include "ui_scriptwindow.h"

ScriptWindow::ScriptWindow(QWidget *parent) :
        QDialog(parent),
        ui(new Ui::ScriptWindow) {
    ui->setupUi(this);

    // Create text edit and text display
    m_interpreter_text_edit = new InterpreterTextEdit(this);
    m_results_text_display = new ResultsTextDisplay(this);
    m_script_editor = new ScriptEditor(this);
	m_file_dialog = new QFileDialog(this, "Run Python File",
	                                QDir::currentPath() + QDir::separator() + PYTHON_SCRIPT_DIR,
                                    "Python script (*.py)");
    ui->interpreterLayout->addWidget(m_interpreter_text_edit);
    ui->displayLayout->addWidget(m_results_text_display);

    // Make the font look like code
    QFont font;
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
    connect(ui->openEditorButton, SIGNAL(clicked()), this, SLOT(openScriptEditor()));
    connect(ui->resetButton, SIGNAL(clicked()), this, SLOT(resetInterpreter()));
    connect(ui->cancelButton, SIGNAL(clicked()), this, SLOT(closeInterpreter()));
    connect(ui->runButton, SIGNAL(clicked()), this, SLOT(runScript()));
    connect(ui->runFileButton, SIGNAL(clicked()), this, SLOT(openRunFile()));

    // Text editor signals
    connect(m_file_dialog, SIGNAL(fileSelected(const QString &)), this, SLOT(processRunFile(const QString &)));
    connect(m_interpreter_text_edit, SIGNAL(scriptSubmitted()), this, SLOT(runScript()));
    connect(this, SIGNAL(scriptSubmitted(const QString&, const QString&, const QString&)),
            m_results_text_display, SLOT(appendResults(const QString&, const QString&, const QString&)));
}

ScriptWindow::~ScriptWindow() {
    delete ui;
    delete m_interpreter_text_edit;
    delete m_results_text_display;
	delete m_file_dialog;
    delete m_script_editor;
}

void ScriptWindow::setVisible(bool visible) {
    if (visible) { PythonEngine::getInstance().initialize(); }
    else { PythonEngine::getInstance().stopEngine(); }
    QDialog::setVisible(visible);
}

void ScriptWindow::reject() {
    PythonEngine::getInstance().stopEngine();
    QDialog::reject();
}

void ScriptWindow::resetInterpreter() {
    // Clear text areas
    m_interpreter_text_edit->clear();
    m_results_text_display->clear();
    // Restart engine
    PythonEngine::getInstance().stopEngine();
    PythonEngine::getInstance().initialize();
}

void ScriptWindow::closeInterpreter() {
    PythonEngine::getInstance().stopEngine();
    this->reject();
}

void ScriptWindow::runScript() {
    if (!PythonEngine::getInstance().isReady()) {
        Logger::log("Error: running script while PyEngine is off", Logger::ERROR);
        return;
    }
    std::string script = m_interpreter_text_edit->toPlainText().toStdString();
    std::string pyOut, pyErr;
    PythonEngine::getInstance().run(script, &pyOut, &pyErr);
    // Emit signal to output display to append results
    Q_EMIT scriptSubmitted(
            QString::fromStdString(script),
            QString::fromStdString(pyOut),
            QString::fromStdString(pyErr));
    m_interpreter_text_edit->clear();
}

void ScriptWindow::openRunFile() {
	m_file_dialog->open();
}

void ScriptWindow::processRunFile(const QString &filePath) {
    // Check the file extension for .py
    QStringList parts = filePath.split('.', QString::SkipEmptyParts);
    if (parts.last() != "py") {
        Q_EMIT scriptSubmitted("", "", "Cannot run non-python file\n");
        return;
    }
#ifndef NDEBUG
    Logger::log("Processing python script file: " + filePath.toStdString(), Logger::DEBUG);
#endif
    QFile scriptFile(filePath);
    if (!scriptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Logger::log("Error: could not open selected script file", Logger::ERROR);
        return;
    }
    QTextStream scriptIn(&scriptFile);
    std::string scriptText = scriptIn.readAll().toStdString();
    if (!PythonEngine::getInstance().isReady()) {
        Logger::log("Error: running script while PyEngine is off", Logger::ERROR);
        return;
    }
    std::string pyOut, pyErr;
    PythonEngine::getInstance().run(scriptText, &pyOut, &pyErr);
    int sepIndex = filePath.lastIndexOf('/');
    // Display file name to user
    if (sepIndex >= 0) {
        QStringRef fileName(&filePath, sepIndex + 1, filePath.length() - sepIndex - 1);
        Q_EMIT scriptSubmitted(
                "Running " + fileName.toString() + '\n',
                QString::fromStdString(pyOut),
                QString::fromStdString(pyErr));
    } else {
        Q_EMIT scriptSubmitted(
                "Running " + filePath + '\n',
                QString::fromStdString(pyOut),
                QString::fromStdString(pyErr));
    }
}

void ScriptWindow::openScriptEditor() {
    m_script_editor->show();
}
