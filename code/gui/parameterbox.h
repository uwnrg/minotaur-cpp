#ifndef PARAMETERBOX_H
#define PARAMETERBOX_H

#include "../utility/weak_ref.h"
#include <QDialog>
#include <memory>

namespace Ui {
    class ParameterBox;
}

class MainWindow;
class ParameterSlot;

class ParameterBox : public QDialog {
Q_OBJECT

public:
    explicit ParameterBox(MainWindow *parent = nullptr);

    ~ParameterBox() override;

    weak_ref<ParameterSlot> add_slot(
        const QString &name,
        const QVariant &def
    );

    void remove_slot(weak_ref<ParameterSlot> &slot);

protected:
    Q_SIGNAL void set_values();

private:
    Ui::ParameterBox *ui;

    std::vector<std::unique_ptr<ParameterSlot>> m_slots;
};

#endif // PARAMETERBOX_H
