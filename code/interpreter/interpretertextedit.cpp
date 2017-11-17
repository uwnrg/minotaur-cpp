#include "interpretertextedit.h"

InterpreterTextEdit::InterpreterTextEdit(QWidget *parent)
        : CodeEditor(parent) {
    QPalette palette = this->palette();
    palette.setColor(QPalette::Base, QColor(39, 40, 34));
    palette.setColor(QPalette::Text, QColor(248, 248, 242));
    this->setPalette(palette);
}

InterpreterTextEdit::~InterpreterTextEdit() = default;

void InterpreterTextEdit::keyPressEvent(QKeyEvent *e) {
    // Submit a script when user press Shift + Enter
    if (e->modifiers() & Qt::ShiftModifier
        && (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)) {
        Q_EMIT scriptSubmitted();
    } else {
        QPlainTextEdit::keyPressEvent(e);
    }
}

void InterpreterTextEdit::focusInEvent(QFocusEvent *e) {
    QPlainTextEdit::focusInEvent(e);
}