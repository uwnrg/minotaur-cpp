#ifndef MINOTAUR_CPP_SERIAL_H_H
#define MINOTAUR_CPP_SERIAL_H_H

#include <memory>

#include <QDialog>
#include <QSlider>
#include <QLineEdit>

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

    Q_SLOT void up_slider_changed(int value);

    Q_SLOT void down_slider_changed(int value);

    Q_SLOT void left_slider_changed(int value);

    Q_SLOT void right_slider_changed(int value);

    Q_SLOT void up_box_changed();

    Q_SLOT void down_box_changed();

    Q_SLOT void left_box_changed();

    Q_SLOT void right_box_changed();

    void slider_changed(int value, int dir);

    void box_changed(QString value, int dir);

private:
    enum Power {
        POWER_INTERVAL = 1,
        POWER_MAX = 255,
        POWER_MIN = 240
    };

    enum Delay {
        DELAY_INTERVAL = 10,
        DELAY_MAX = 150,
        DELAY_MIN = 10,
        DELAY_DEFAULT = 50
    };

    Ui::SerialBox *ui;
    std::shared_ptr<Solenoid> m_solenoid;

    QSlider *m_sliders[4];
    QLineEdit *m_edit_boxes[4];

    SerialStatus m_status;
};

#endif //MINOTAUR_CPP_SERIAL_H_H
