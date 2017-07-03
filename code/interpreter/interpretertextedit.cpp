#include "interpretertextedit.h"

InterpreterTextEdit::InterpreterTextEdit(QWidget *parent) : QTextEdit(parent) {}

InterpreterTextEdit::~InterpreterTextEdit() {}

void InterpreterTextEdit::keyPressEvent(QKeyEvent *e) {
    if (e->modifiers() & Qt::ShiftModifier
        && (e->key() == Qt::Key_Return || e->key() == Qt::Key_Enter)) {
        Q_EMIT scriptSubmitted();
    } else {
        QTextEdit::keyPressEvent(e);
    }
}

void InterpreterTextEdit::focusInEvent(QFocusEvent *e) {
    QTextEdit::focusInEvent(e);
}