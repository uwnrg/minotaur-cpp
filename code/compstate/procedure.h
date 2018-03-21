#ifndef MINOTAUR_CPP_PROCEDURE_H
#define MINOTAUR_CPP_PROCEDURE_H

#include <memory>

#include <QObject>
#include <QBasicTimer>

#include "code/utility/vector2d.h"

class Solenoid;

class Procedure : public QObject {
Q_OBJECT

public:
    explicit Procedure(std::weak_ptr<Solenoid> sol, path2d<double> &&path);

private:
    void timerEvent(QTimerEvent *ev) override;

    void movement_loop();

    // estimated_power is the positive error in the direction of movement,
    // and it is up to the controller to translate this into a solenoid voltage
    void move_right(double estimated_power);
    void move_left(double estimated_power);
    void move_up(double estimated_power);
    void move_down(double estimated_power);

    double m_loc_accept;
    path2d<double> m_path;
    std::size_t m_index;
    std::weak_ptr<Solenoid> m_sol;
    QBasicTimer m_timer;
};

#endif //MINOTAUR_CPP_PROCEDURE_H
