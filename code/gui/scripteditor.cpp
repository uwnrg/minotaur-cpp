#include <QFileDialog>
#include <QTextStream>

#include "scripteditor.h"
#include "ui_scripteditor.h"

#include "../utility/logger.h"

ScriptEditor::ScriptEditor(QWidget *parent) :
        QDialog(parent),
        m_active_file(),
        ui(new Ui::ScriptEditor) {
    ui->setupUi(this);

    // Make the font look like code
    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(8);
    QFontMetrics metrics(font);
    const int tab_size = 4;
    const int tab_stop_width = tab_size * metrics.width(' ');
    m_code_editor = new CodeEditor(this);
    m_code_editor->setFont(font);
    m_code_editor->setTabStopWidth(tab_stop_width);
    ui->editorLayout->addWidget(m_code_editor);

    QPalette palette = m_code_editor->palette();
    palette.setColor(QPalette::Base, QColor(39, 40, 34));
    palette.setColor(QPalette::Text, QColor(248,248,242));
    m_code_editor->setPalette(palette);

    // Connect slots
    connect(ui->openFileButton, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->saveAsButton, SIGNAL(clicked()), this, SLOT(saveAs()));
    connect(ui->newButton, SIGNAL(clicked()), this, SLOT(newFile()));
}

ScriptEditor::~ScriptEditor() {
    delete ui;
    delete m_code_editor;
}

void ScriptEditor::reject() {
    m_active_file.clear();
    m_code_editor->clear();
    QDialog::reject();
}

void ScriptEditor::openFile() {
    QString file_path = QFileDialog::getOpenFileName(
            this, "Open Python Script",
            QDir::currentPath() + QDir::separator() + PYTHON_SCRIPT_DIR,
            "Python script (*.py)");
#ifndef NDEBUG
    Logger::log("Opened file: " + file_path.toStdString(), Logger::DEBUG);
#endif
    if (file_path.split('.', QString::SkipEmptyParts).last() != "py") {
        Logger::log("Cannot open non-python file", Logger::FATAL);
        return;
    }
    QFile script_file(file_path);
    if (!script_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Logger::log("Error: could not open selected script file", Logger::FATAL);
        return;
    }
    QTextStream script_in(&script_file);
    QString script_text = script_in.readAll();
    // Set the text editor contents to the file contents
    m_code_editor->document()->setPlainText(script_text);
    this->m_active_file = std::move(file_path);
}

void ScriptEditor::save() {
    if (this->m_active_file.isEmpty()) {
        saveAs();
        return;
    }
#ifndef NDEBUG
    Logger::log("Saving active file: " + m_active_file.toStdString(), Logger::DEBUG);
#endif
    QFile script_file(m_active_file);
    if (!script_file.open(QIODevice::WriteOnly)) {
        Logger::log("Error: could not save file", Logger::FATAL);
        return;
    }
    QTextStream script_out(&script_file);
    script_out << m_code_editor->toPlainText();
#ifndef NDEBUG
    Logger::log("Save successful", Logger::DEBUG);
#endif
}

void ScriptEditor::saveAs() {
    QString file_path = QFileDialog::getSaveFileName(
            this, "Save Script",
            QDir::currentPath() + QDir::separator() + PYTHON_SCRIPT_DIR,
            "Python script (*.py)");
#ifndef NDEBUG
    Logger::log("Saving to file: " + file_path.toStdString(), Logger::DEBUG);
#endif
    if (file_path.isEmpty() || file_path.split('.', QString::SkipEmptyParts).last() != "py") {
        Logger::log("Error: invalid save file", Logger::FATAL);
        return;
    }
    QFile script_file(file_path);
    if (!script_file.open(QIODevice::WriteOnly)) {
        Logger::log("Error: could not save file", Logger::FATAL);
        return;
    }
    QTextStream script_out(&script_file);
    script_out << m_code_editor->toPlainText();
#ifndef NDEBUG
    Logger::log("Save successful", Logger::DEBUG);
#endif
}

void ScriptEditor::newFile() {
    m_active_file.clear();
    m_code_editor->clear();
}
