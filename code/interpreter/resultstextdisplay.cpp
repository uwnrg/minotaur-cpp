#include "resultstextdisplay.h"

ResultsTextDisplay::ResultsTextDisplay(QWidget *parent) :
        CodeEditor(parent) {
    this->setReadOnly(true);
    QPalette palette = this->palette();
    palette.setColor(QPalette::Base, QColor(39, 40, 34));
    palette.setColor(QPalette::Text, QColor(248, 248, 242));
    this->setPalette(palette);
}

ResultsTextDisplay::~ResultsTextDisplay() = default;

void ResultsTextDisplay::append(const QString &str) {
    this->moveCursor(QTextCursor::End);
    this->insertPlainText(str);
    this->insertPlainText("\n");
}

void ResultsTextDisplay::appendResults(
        const QString &script,
        const QString &out,
        const QString &err) {
    if (script.length()) { this->append(script); }
    if (out.length()) { this->append(out); }
    if (err.length()) { this->append(err); }
}