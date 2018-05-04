#ifndef PARAMETERSLOT_H
#define PARAMETERSLOT_H

#include <QWidget>
#include <QVariant>

namespace Ui {
    class ParameterSlot;
}

class ParameterBox;

class ParameterSlot : public QWidget {
Q_OBJECT

public:
    explicit ParameterSlot(
        const QString &name,
        const QVariant &def,
        ParameterBox *parent = nullptr
    );

    ~ParameterSlot() override;

    void set_value();

    Q_SIGNAL void value_set(const QVariant &value);

private:
    Ui::ParameterSlot *ui;
};

#endif // PARAMETERSLOT_H
