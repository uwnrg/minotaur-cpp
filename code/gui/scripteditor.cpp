#include <QFileDialog>
#include <QTextStream>

#include "scripteditor.h"
#include "ui_scripteditor.h"

#include "../utility/logger.h"

ScriptEditor::ScriptEditor(QWidget *parent) :
        QDialog(parent),
        m_activeFile(),
        ui(new Ui::ScriptEditor) {
    ui->setupUi(this);

    // Make the font look like code
    QFont font;
    font.setFamily("Courier");
    font.setStyleHint(QFont::Monospace);
    font.setFixedPitch(true);
    font.setPointSize(8);
    QFontMetrics metrics(font);
    const int tabSize = 4;
    const int tabStopWidth = tabSize * metrics.width(' ');
    ui->textEdit->setFont(font);
    ui->textEdit->setTabStopWidth(tabStopWidth);

    // Connect slots
    connect(ui->openFileButton, SIGNAL(clicked()), this, SLOT(openFile()));
    connect(ui->saveButton, SIGNAL(clicked()), this, SLOT(save()));
    connect(ui->saveAsButton, SIGNAL(clicked()), this, SLOT(saveAs()));
    connect(ui->newButton, SIGNAL(clicked()), this, SLOT(newFile()));
}

ScriptEditor::~ScriptEditor() {
    delete ui;
}

void ScriptEditor::reject() {
    m_activeFile.clear();
    ui->textEdit->clear();
    QDialog::reject();
}

void ScriptEditor::openFile() {
    QString filePath = QFileDialog::getOpenFileName(
            this, "Open Python Script",
            QDir::currentPath() + QDir::separator() + PYTHON_SCRIPT_DIR,
            "Python script (*.py)");
#ifndef NDEBUG
    Logger::log("Opened file: " + filePath.toStdString(), Logger::DEBUG);
#endif
    if (filePath.split('.', QString::SkipEmptyParts).last() != "py") {
        Logger::log("Cannot open non-python file", Logger::ERROR);
        return;
    }
    QFile scriptFile(filePath);
    if (!scriptFile.open(QIODevice::ReadOnly | QIODevice::Text)) {
        Logger::log("Error: could not open selected script file", Logger::ERROR);
        return;
    }
    QTextStream scriptIn(&scriptFile);
    QString scriptText = scriptIn.readAll();
    // Set the text editor contents to the file contents
    ui->textEdit->setText(scriptText);
    this->m_activeFile = std::move(filePath);
}

void ScriptEditor::save() {
    if (this->m_activeFile.isEmpty()) {
        saveAs();
        return;
    }
#ifndef NDEBUG
    Logger::log("Saving active file: " + m_activeFile.toStdString(), Logger::DEBUG);
#endif
    QFile scriptFile(m_activeFile);
    if (!scriptFile.open(QIODevice::WriteOnly)) {
        Logger::log("Error: could not save file", Logger::ERROR);
        return;
    }
    QTextStream scriptOut(&scriptFile);
    scriptOut << ui->textEdit->toPlainText();
#ifndef NDEBUG
    Logger::log("Save successful", Logger::DEBUG);
#endif
}

void ScriptEditor::saveAs() {
    QString filePath = QFileDialog::getSaveFileName(
            this, "Save Script",
            QDir::currentPath() + QDir::separator() + PYTHON_SCRIPT_DIR,
            "Python script (*.py)");
#ifndef NDEBUG
    Logger::log("Saving to file: " + filePath.toStdString(), Logger::DEBUG);
#endif
    if (filePath.isEmpty() || filePath.split('.', QString::SkipEmptyParts).last() != "py") {
        Logger::log("Error: invalid save file", Logger::ERROR);
        return;
    }
    QFile scriptFile(filePath);
    if (!scriptFile.open(QIODevice::WriteOnly)) {
        Logger::log("Error: could not save file", Logger::ERROR);
        return;
    }
    QTextStream scriptOut(&scriptFile);
    scriptOut << ui->textEdit->toPlainText();
#ifndef NDEBUG
    Logger::log("Save successful", Logger::DEBUG);
#endif
}

void ScriptEditor::newFile() {
    m_activeFile.clear();
    ui->textEdit->clear();
}
