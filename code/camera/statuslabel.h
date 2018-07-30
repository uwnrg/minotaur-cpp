#ifndef MINOTAUR_CPP_STATUSLABEL_H
#define MINOTAUR_CPP_STATUSLABEL_H

#include "../base/statuslabel.h"
#include <QLabel>

/**
 * Status label is a QLabel with an attached ID so
 * that one can be removed from the StatusBox.
 */
class StatusLabel : public QLabel, public base::StatusLabel {
public:
    StatusLabel(QWidget *parent, std::size_t id, const QString &initial);

    std::size_t id() const;

private:
    std::size_t m_id;
};

#endif //MINOTAUR_CPP_STATUSLABEL_H
