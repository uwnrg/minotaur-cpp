#ifndef PARAMETERBOX_H
#define PARAMETERBOX_H

#include <QDialog>
#include <memory>

#include "../utility/weak_ref.h"

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

private:
    Ui::ParameterBox *ui;

    std::vector<std::unique_ptr<ParameterSlot>> m_slots;
};

#endif // PARAMETERBOX_H
