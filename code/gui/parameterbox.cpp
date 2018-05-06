#include "parameterbox.h"
#include "ui_parameterbox.h"

#include "parameterslot.h"
#include "mainwindow.h"

ParameterBox::ParameterBox(MainWindow *parent) :
    QDialog(parent),
    ui(new Ui::ParameterBox) {
    ui->setupUi(this);

    connect(ui->set_btn, &QPushButton::clicked, this, &ParameterBox::set_values);
}

ParameterBox::~ParameterBox() {
    delete ui;
}

weak_ref<ParameterSlot> ParameterBox::add_slot(
    const QString &name,
    const QVariant &def
) {
    auto *slot = new ParameterSlot(name, def, this);
    m_slots.emplace_back(slot);
    ui->layout->addWidget(slot);
    connect(this, &ParameterBox::set_values, slot, &ParameterSlot::set_value);
    return as_weak(slot);
}

void ParameterBox::remove_slot(weak_ref<ParameterSlot> &slot) {
    auto it = m_slots.begin();
    for (; it != m_slots.end() && it->get() != slot.get(); ++it);
    ui->layout->removeWidget(slot.get());
    if (it != m_slots.end()) { m_slots.erase(it); }
}
