#ifndef MINOTAUR_CPP_ACTIONBOX_H
#define MINOTAUR_CPP_ACTIONBOX_H

#include <memory>

#include <QDialog>
#include <QPushButton>
#include <QMetaType>
#include <QVBoxLayout>

class ActionBox;

class ActionButton : public QPushButton {
public:
    explicit ActionButton(QString &&label, QWidget *parent = nullptr);

};

class ActionBox : public QDialog {
Q_OBJECT

public:
    explicit ActionBox(QWidget *parent = nullptr);

    ActionButton *add_action(QString &&label);

    Q_SLOT void reset_actions();

    Q_SLOT void set_actions();

private:
    std::unique_ptr<QVBoxLayout> m_layout;
    std::vector<std::unique_ptr<ActionButton>> m_actions;

};

#endif //MINOTAUR_CPP_ACTIONBOX_H
