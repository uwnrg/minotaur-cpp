#ifndef MINOTAUR_CPP_OBJECTSTRATEGY_H
#define MINOTAUR_CPP_OBJECTSTRATEGY_H

#include "procedure.h"
#include "compstate.h"

#include "../controller/controller.h"
#include "../utility/vector.h"
#include "../utility/rect.h"

#include <QObject>
#include <QTimer>

class ObjectStrategy : public QObject {
Q_OBJECT

public:
    enum State {
        UNINITIALIZED,
        COLLIDING,
        READY_MOVE,

        PUSHING,

        COLLIDING_PROC,
        READY_MOVE_PROC
    };

    ObjectStrategy(std::weak_ptr<Controller> sol, double delta, nrg::dir dir);

    void start();

    void stop();

private:
    void timerEvent(QTimerEvent *ev) override;

    void movement_loop();

    void ready_move_collide_block(CompetitionState &state, const rect2d &obj_rect);

    void do_uninitialized();

    void do_colliding();

    void do_colliding_proc();

    void do_ready_move();

    void do_ready_move_proc();

private:
    std::weak_ptr<Controller> m_sol;
    double m_delta;
    nrg::dir m_dir;

    State m_state;
    vector2d m_target;
    vector2d m_resolve;

    std::unique_ptr<Procedure> m_proc;

    QBasicTimer m_timer;
};

#endif //MINOTAUR_CPP_OBJECTSTRATEGY_H
