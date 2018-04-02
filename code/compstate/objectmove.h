#ifndef MINOTAUR_CPP_OBJECTMOVE_H
#define MINOTAUR_CPP_OBJECTMOVE_H

#include <QObject>
#include <QBasicTimer>

#include "readymove.h"

#include "../controller/controller.h"
#include "../utility/rect.h"

class ObjectMove : public QObject {
Q_OBJECT

public:
    enum Stop {
        OKAY,
        AT_TARGET,
        EXCEEDED_NORM,
        WRONG_SIDE
    };

    explicit ObjectMove(
        std::weak_ptr<Controller> sol,
        nrg::dir dir,
        double target,
        double norm_base,
        double norm_dev
    );

    void start();
    void stop();

    bool is_done() const;
    Stop get_stop() const;

private:
    void timerEvent(QTimerEvent *ev) override;

    void movement_loop();

    void do_move(double delta);
    void correct(double delta);

    bool beyond_target(const vector2d &obj_loc) const;
    bool robot_correct_side(const vector2d &rob_loc, const vector2d &obj_loc) const;
    bool exceeded_norm(const vector2d &obj_loc) const;
    double alignment_err(const vector2d &rob_loc, const vector2d &obj_loc) const;
    double target_err(const vector2d &obj_loc) const;

    std::weak_ptr<Controller> m_sol;
    nrg::dir m_dir;
    double m_target;
    double m_norm_base;
    double m_norm_dev;

    bool m_done;
    Stop m_stop;

    QBasicTimer m_timer;

    Procedure m_delegate;
};

#endif //MINOTAUR_CPP_OBJECTMOVE_H
