#ifndef MINOTAUR_CPP_SERIAL_H_H
#define MINOTAUR_CPP_SERIAL_H_H

#include <memory>

#include <QDialog>
#include <QSlider>

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

    ~SerialBox();

    void setup_power_slider(QSlider &slider);

    void setup_slider(QSlider &slider, int value, int min, int max, int interval);

    Q_SLOT void update_status(SerialStatus new_status);

    Q_SLOT void attempt_connection();

    Q_SLOT void append_text(const std::string &text);

private:
    enum Power {
        POWER_INTERVAL = 8,
        POWER_MAX = 255,
        POWER_MIN = 0
    };

    enum Delay {
        DELAY_INTERVAL = 10,
        DELAY_MAX = 150,
        DELAY_MIN = 10,
        DELAY_DEFAULT = 50
    };

    Ui::SerialBox *ui;
    std::shared_ptr<Solenoid> m_solenoid;

    SerialStatus m_status;
};

#endif //MINOTAUR_CPP_SERIAL_H_H
