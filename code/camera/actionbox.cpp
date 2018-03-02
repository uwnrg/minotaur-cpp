#include "actionbox.h"

#include <QVBoxLayout>

#include "code/utility/utility.h"

ActionButton::ActionButton(QWidget *parent)
    : QPushButton(parent) {
    setMinimumSize(150, 50);
    setMaximumSize(200, 75);
}

ActionBox::ActionBox(QWidget *parent)
    : QDialog(parent),
      m_layout(std::make_unique<QVBoxLayout>()) {
    reset_actions();
    setLayout(m_layout.get());
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

void ActionBox::add_action(ActionButton *action) {
    m_layout->addWidget(action);
}
