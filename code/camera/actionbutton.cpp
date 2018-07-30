#include "actionbutton.h"

ActionButton::ActionButton(QString &&label, QWidget *parent) :
    QPushButton(label, parent) {
    // Set a fixed size for the buttons
    setMinimumSize(150, 50);
    setMaximumSize(200, 75);
}
