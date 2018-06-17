#ifndef MINOTAUR_CPP_INTERPRETERTEXTEDIT_H
#define MINOTAUR_CPP_INTERPRETERTEXTEDIT_H

#include "codeeditor.h"

class InterpreterTextEdit : public CodeEditor {
Q_OBJECT

public:
    InterpreterTextEdit(QWidget *parent = 0);
    ~InterpreterTextEdit();

Q_SIGNALS:
    // Signal when a script has been submitted
    // to the engine by the interpreter window
    void scriptSubmitted();

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void focusInEvent(QFocusEvent *e) override;
};


#endif //MINOTAUR_CPP_INTERPRETERTEXTEDIT_H
