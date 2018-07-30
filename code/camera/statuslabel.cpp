#include "statuslabel.h"
#include "../utility/font.h"

StatusLabel::StatusLabel(QWidget *parent, std::size_t id, const QString &initial) :
    QLabel(initial, parent),
    m_id(id) {
    setFont(qfont::courier());
}

std::size_t StatusLabel::id() const {
    return m_id;
}
