#include "parameterbox.h"
#include "ui_parameterbox.h"

#include "parameterslot.h"
#include "mainwindow.h"

ParameterBox::ParameterBox(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::ParameterBox) {
    ui->setupUi(this);
}

ParameterBox::~ParameterBox() {
    delete ui;
}

weak_ref<ParameterSlot> ParameterBox::add_slot(
    const QString &name,
    const QVariant &def
) {
    std::size_t index = m_slots.size();
    m_slots.emplace_back(name, def, this);
    ParameterSlot *slot = m_slots[index].get();
    ui->layout->addWidget(slot);
    return slot;
}

void ParameterBox::remove_slot(const weak_ref<ParameterSlot> &slot) {
    auto it = m_slots.begin();
    for (; it != m_slots.end() && it->get() != slot.get(); ++it);
    if (it != m_slots.end()) {
        ui->layout->removeWidget(slot.get());
        m_slots.erase(it);
    }
}
