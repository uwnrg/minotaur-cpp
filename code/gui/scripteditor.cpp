#include "../interpreter/codeeditor.h"
#include "../utility/font.h"
#include "../utility/logger.h"
#include "../utility/utility.h"

#include "ui_scripteditor.h"
#include "scripteditor.h"

#include <QFileDialog>
#include <QTextStream>

ScriptEditor::ScriptEditor(QWidget *parent) :
    QDialog(parent),
    ui(std::make_unique<Ui::ScriptEditor>()),
    m_code_editor(std::make_unique<CodeEditor>(this)),
    m_active_file() {
    ui->setupUi(this);

    // Make the font look like code
    QFont font = qfont::courier(8);
    m_code_editor->setFont(font);
    m_code_editor->setTabStopWidth(qfont::tab_stop(font));
    ui->editorLayout->addWidget(m_code_editor.get());

    QPalette palette = m_code_editor->palette();
    palette.setColor(QPalette::Base, QColor(39, 40, 34));
    palette.setColor(QPalette::Text, QColor(248, 248, 242));
    m_code_editor->setPalette(palette);

    // Connect slots
    connect(ui->openFileButton, &QPushButton::clicked, this, &ScriptEditor::openFile);
    connect(ui->saveButton, &QPushButton::clicked, this, &ScriptEditor::save);
    connect(ui->saveAsButton, &QPushButton::clicked, this, &ScriptEditor::saveAs);
    connect(ui->newButton, &QPushButton::clicked, this, &ScriptEditor::newFile);
}

ScriptEditor::~ScriptEditor() = default;

void ScriptEditor::reject() {
    m_active_file.clear();
    m_code_editor->clear();
    QDialog::reject();
}

void ScriptEditor::openFile() {
    QString file_path = QFileDialog::getOpenFileName(
        this, "Open Python Script",
        QDir::currentPath() + QDir::separator() + PYTHON_SCRIPT_DIR,
        "Python script (*.py)"
    );
#ifndef NDEBUG
    debug() << "Opened file: " << file_path;
#endif
    if (file_path.split('.', QString::SkipEmptyParts).last() != "py") {
        fatal() << "Cannot open non-python file";
        return;
    }
    QFile script_file(file_path);
    if (!script_file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        fatal() << "Could not open specified file";
        return;
    }
    QTextStream script_in(&script_file);
    QString script_text = script_in.readAll();
    // Set the text editor contents to the file contents
    m_code_editor->document()->setPlainText(script_text);
    m_active_file = std::move(file_path);
}

void ScriptEditor::save() {
    if (this->m_active_file.isEmpty()) {
        saveAs();
        return;
    }
#ifndef NDEBUG
    debug() << "Saving active file: " << m_active_file;
#endif
    QFile script_file(m_active_file);
    if (!script_file.open(QIODevice::WriteOnly)) {
        fatal() << "Error: could not save file";
        return;
    }
    QTextStream script_out(&script_file);
    script_out << m_code_editor->toPlainText();
#ifndef NDEBUG
    debug() << "Save successful";
#endif
}

void ScriptEditor::saveAs() {
    QString file_path = QFileDialog::getSaveFileName(
        this, "Save Script",
        QDir::currentPath() + QDir::separator() + PYTHON_SCRIPT_DIR,
        "Python script (*.py)");
#ifndef NDEBUG
    debug() << "Saving to file: " << file_path;
#endif
    if (file_path.isEmpty() || file_path.split('.', QString::SkipEmptyParts).last() != "py") {
        fatal() << "Error: invalid save file";
        return;
    }
    QFile script_file(file_path);
    if (!script_file.open(QIODevice::WriteOnly)) {
        fatal() << "Error: could no save file";
        return;
    }
    QTextStream script_out(&script_file);
    script_out << m_code_editor->toPlainText();
#ifndef NDEBUG
    debug() << "Save successful";
#endif
}

void ScriptEditor::newFile() {
    m_active_file.clear();
    m_code_editor->clear();
}
