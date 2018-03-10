#include "actionbox.h"

#include "../utility/utility.h"

ActionButton::ActionButton(QString &&label, QWidget *parent)
    : QPushButton(label, parent) {
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
    m_actions.clear();
    hide();
}

void ActionBox::set_actions() {
    setLayout(m_layout.get());
    show();
}

ActionButton *ActionBox::add_action(QString &&label) {
    std::size_t size = m_actions.size();
    m_actions.emplace_back(new ActionButton(std::forward<QString>(label), this));
    return m_actions[size].get();
}
