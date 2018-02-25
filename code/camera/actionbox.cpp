#include "actionbox.h"

#include <QVBoxLayout>

#include "../utility/util.h"

ButtonAction::ButtonAction(QString &&label, QWidget *parent)
    : QPushButton(label, parent) {
    setMinimumSize(150, 50);
    setMaximumSize(200, 75);
}

ActionBox::ActionBox(QWidget *parent)
    : QDialog(parent),
      m_layout(std::make_unique<QVBoxLayout>()) {
    reset_actions();
    set_actions();
}

ActionBox::~ActionBox() = default;

void ActionBox::reset_actions() {
    m_layout = std::make_unique<QVBoxLayout>();
    hide();
}

void ActionBox::set_actions() {
    setLayout(m_layout.get());
    show();
}

void ActionBox::add_action(ButtonAction *action) {
    m_layout->addWidget(action);
}
