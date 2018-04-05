#ifndef MINOTAUR_CPP_ACTIONBOX_H
#define MINOTAUR_CPP_ACTIONBOX_H

#include <memory>

#include <QDialog>
#include <QPushButton>
#include <QMetaType>
#include <QVBoxLayout>

// Forward declaration
class ActionBox;

/**
 * Push button implementation that sets a fixed size
 * and manages the button label. Used with the ActionBox.
 */
class ActionButton : public QPushButton {
public:
    explicit ActionButton(QString &&label, QWidget *parent = nullptr);
};

/**
 * A Dialog that is shown when VideoModifiers in the camera display
 * need to have access to user input from the UI. VideoModifiers can
 * attach buttons to the widget and connect them to slots.
 *
 * This class is responsible for managing the allocated buttons.
 */
class ActionBox : public QDialog {
Q_OBJECT

public:
    explicit ActionBox(QWidget *parent = nullptr);

    /**
     * Create a new action button and return a reference to it
     * so that the caller may connect its signals to slots.
     *
     * @param label the label of the button
     * @return reference to the button
     */
    ActionButton *add_action(QString &&label);

    /**
     * Clear the buttons in the UI and hide the ActionBox.
     */
    Q_SLOT void reset_actions();

    /**
     * Opens the UI with previous added actions and buttons.
     */
    Q_SLOT void set_actions();

private:
    /**
     * Layout of the Dialog where buttons are placed
     * and which is reset with the actions.
     */
    std::unique_ptr<QVBoxLayout> m_layout;
    /**
     * List of the button pointers, which are managed by
     * unique pointers.
     */
    std::vector<std::unique_ptr<ActionButton>> m_actions;
};

#endif //MINOTAUR_CPP_ACTIONBOX_H
