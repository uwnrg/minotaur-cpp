#ifndef PARAMETERSLOT_H
#define PARAMETERSLOT_H

#include <QVariant>
#include <QWidget>

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

    Q_SLOT void set_value();

    Q_SIGNAL void value_set(const QVariant &value);

private:
    Ui::ParameterSlot *ui;
};

#endif // PARAMETERSLOT_H
