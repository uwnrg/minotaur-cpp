#ifndef MINOTAUR_CPP_SERIAL_H_H
#define MINOTAUR_CPP_SERIAL_H_H

#include <memory>

#include <QDialog>

#include "ui_serialbox.h"
#include "../controller/solenoid.h"

namespace Ui {
    class SerialBox;
}

class SerialBox : public QDialog {
Q_OBJECT

public:
    explicit SerialBox(
        const std::shared_ptr<Solenoid> &solenoid,
        QWidget *parent = nullptr
    );

    Q_SLOT void update_status(SerialStatus new_status);

    Q_SLOT void attempt_connection();

private:
    std::unique_ptr<Ui::SerialBox> ui;
    std::shared_ptr<Solenoid> m_solenoid;

    SerialStatus m_status;
};

#endif //MINOTAUR_CPP_SERIAL_H_H
