#include "statusbox.h"
#include "../utility/font.h"
#include "../utility/utility.h"

#ifndef NDEBUG
#include <QDebug>
#endif

StatusLabel::StatusLabel(QWidget *parent, std::size_t id, const QString &initial) :
    QLabel(initial, parent),
    m_id(id) {
    setFont(qfont::courier());
}

std::size_t StatusLabel::id() const {
    return m_id;
}

StatusBox::StatusBox(QWidget *parent) :
    QDialog(parent),
    m_id(0),
    m_layout(std::make_unique<QVBoxLayout>(this)) {
    setMinimumSize(400, 200);
    setLayout(m_layout.get());
}

StatusLabel* StatusBox::add_label(const QString &initial) {
#ifndef NDEBUG
    qDebug() << "Creating label: " << initial;
#endif
    std::size_t id = m_id++;
    auto *label = new StatusLabel(this, id, initial);
    m_labels[id].reset(label);
    m_layout->addWidget(label);
    if (m_labels.size() == 1) { show(); }
    return label;
}

void StatusBox::remove_label(StatusLabel *label) {
    if (!label) { return; }
    auto it = m_labels.find(label->id());
    if (it != m_labels.end()) {
        m_layout->removeWidget(label);
        m_labels.erase(it);
        if (m_labels.empty()) { hide(); }
    }
}
