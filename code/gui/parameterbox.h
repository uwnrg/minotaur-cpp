#ifndef MINOTAUR_CPP_PARAMETERBOX_H
#define MINOTAUR_CPP_PARAMETERBOX_H

#include <QDialog>
#include <memory>

namespace Ui {
    class ParameterBox;
    class ParameterSlot;
}

class ParameterBox;

class ParameterSlot : public QWidget {
Q_OBJECT

public:
    explicit ParameterSlot(ParameterBox *parent, QString &&name);

private:
    std::unique_ptr<Ui::ParameterSlot> ui;
};

class ParameterBox : public QDialog {
Q_OBJECT

public:
    explicit ParameterBox(QWidget *parent = nullptr);

private:
    std::unique_ptr<Ui::ParameterBox> ui;
};

#endif //MINOTAUR_CPP_PARAMETERBOX_H
