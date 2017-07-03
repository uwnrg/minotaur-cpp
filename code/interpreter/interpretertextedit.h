#ifndef MINOTAUR_CPP_INTERPRETERTEXTEDIT_H
#define MINOTAUR_CPP_INTERPRETERTEXTEDIT_H

#include <QTextEdit>
#include <QKeyEvent>

class InterpreterTextEdit : public QTextEdit {
Q_OBJECT

public:
    InterpreterTextEdit(QWidget *parent = 0);
    ~InterpreterTextEdit();

Q_SIGNALS:
    void scriptSubmitted();

protected:
    void keyPressEvent(QKeyEvent *e) override;
    void focusInEvent(QFocusEvent *e) override;
};


#endif //MINOTAUR_CPP_INTERPRETERTEXTEDIT_H
