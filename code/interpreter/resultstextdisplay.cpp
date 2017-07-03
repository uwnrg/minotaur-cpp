#include "resultstextdisplay.h"

ResultsTextDisplay::ResultsTextDisplay(QWidget *parent) :
        QTextEdit(parent) {
    this->setReadOnly(true);
}

ResultsTextDisplay::~ResultsTextDisplay() {}

void ResultsTextDisplay::appendResults(
        const QString &script,
        const QString &out,
        const QString &err) {
    if (script.length()) this->append(script);
    if (out.length()) this->append(out);
    if (err.length()) this->append(err);
}