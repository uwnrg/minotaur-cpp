#ifndef MINOTAUR_CPP_PROCEDURE_H
#define MINOTAUR_CPP_PROCEDURE_H

#include <memory>

#include <QObject>
#include <QBasicTimer>

#include "code/utility/vector2d.h"

class Controller;
class StatusLabel;

class Procedure : public QObject {
Q_OBJECT

public:
    explicit Procedure(std::weak_ptr<Controller> sol, path2d<double> &&path);

    ~Procedure() override;

    void start();

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
    double m_norm_dev;
    path2d<double> m_path;
    vector2d<double> m_initial;
    std::size_t m_index;
    std::weak_ptr<Controller> m_sol;
    QBasicTimer m_timer;

    StatusLabel *m_dir_label;
    StatusLabel *m_err_label;
    StatusLabel *m_index_label;
    StatusLabel *m_perp_label;
};

#endif //MINOTAUR_CPP_PROCEDURE_H
