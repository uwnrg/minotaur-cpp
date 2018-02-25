#ifndef MINOTAUR_CPP_ACTIONBOX_H
#define MINOTAUR_CPP_ACTIONBOX_H

#include <memory>

#include <QDialog>
#include <QPushButton>
#include <QMetaType>

class ActionBox;

Q_DECLARE_METATYPE(ActionBox *);

class QVBoxLayout;

class ButtonAction : public QPushButton {
public:
    explicit ButtonAction(QString &&label, QWidget *parent = nullptr);

};

class ActionBox : public QDialog {
Q_OBJECT

public:
    explicit ActionBox(QWidget *parent = nullptr);

    ~ActionBox() override;

    void reset_actions();

    void set_actions();

    void add_action(ButtonAction *action);

private:
    std::unique_ptr<QVBoxLayout> m_layout;

};

#endif //MINOTAUR_CPP_ACTIONBOX_H
