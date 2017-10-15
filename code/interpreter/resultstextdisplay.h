#ifndef MINOTAUR_CPP_RESULTSTEXTDISPLAY_H
#define MINOTAUR_CPP_RESULTSTEXTDISPLAY_H

#include <QTextEdit>

class ResultsTextDisplay : public QTextEdit {
Q_OBJECT

public:
    ResultsTextDisplay(QWidget *parent = 0);
    ~ResultsTextDisplay();

public Q_SLOTS:
    void appendResults(const QString &script,
                       const QString &out,
                       const QString &err);
};


#endif //MINOTAUR_CPP_RESULTSTEXTDISPLAY_H