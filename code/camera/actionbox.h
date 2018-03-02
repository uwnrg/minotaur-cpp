#ifndef MINOTAUR_CPP_ACTIONBOX_H
#define MINOTAUR_CPP_ACTIONBOX_H

#include <memory>

#include <QDialog>
#include <QPushButton>
#include <QMetaType>

class ActionBox;

Q_DECLARE_METATYPE(ActionBox *);

class QVBoxLayout;

class ActionButton : public QPushButton {
public:
    explicit ActionButton(QWidget *parent = nullptr);

};

class ActionBox : public QDialog {
Q_OBJECT

public:
    explicit ActionBox(QWidget *parent = nullptr);

    ~ActionBox() override;

    void add_action(ActionButton *action);

    Q_SLOT void reset_actions();

    Q_SLOT void set_actions();

private:
    std::unique_ptr<QVBoxLayout> m_layout;

};

#endif //MINOTAUR_CPP_ACTIONBOX_H
