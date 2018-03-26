#ifndef MINOTAUR_CPP_STATUSBOX_H
#define MINOTAUR_CPP_STATUSBOX_H

#include <QDialog>
#include <QLabel>
#include <QVBoxLayout>

#include <memory>
#include <unordered_map>

class StatusLabel : public QLabel {
public:
    StatusLabel(QWidget *parent, std::size_t id, const QString &initial);

    std::size_t id() const;

private:
    std::size_t m_id;
};

class StatusBox : public QDialog {
Q_OBJECT

public:
    explicit StatusBox(QWidget *parent = nullptr);

    StatusLabel *add_label(const QString &initial);

    void remove_label(StatusLabel *label);

private:
    std::size_t m_id;
    std::unique_ptr<QVBoxLayout> m_layout;
    std::unordered_map<std::size_t, std::unique_ptr<StatusLabel>> m_labels;
};

#endif //MINOTAUR_CPP_STATUSBOX_H
