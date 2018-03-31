#ifndef MINOTAUR_CPP_OBJECTSTRATEGY_H
#define MINOTAUR_CPP_OBJECTSTRATEGY_H

#include "procedure.h"

#include "../controller/controller.h"
#include "../utility/vector.h"
#include "../utility/rect.h"

#include <QObject>
#include <QTimer>

class ReadyMove : public QObject {
Q_OBJECT

public:
    enum State {
        UNINITIALIZED,
        COLLIDING,
        READY_MOVE,

        COLLIDING_PROC,
        READY_MOVE_PROC,
    };

    explicit ReadyMove(std::weak_ptr<Controller> sol, nrg::dir dir);

    void start();

    void stop();

private:
    void timerEvent(QTimerEvent *ev) override;

    void movement_loop();

    void do_uninitialized();
    void do_colliding();
    void do_colliding_proc();
    void do_ready_move();
    void do_ready_move_proc();

    bool is_done() const;

private:
    std::weak_ptr<Controller> m_sol;
    nrg::dir m_dir;

    State m_state;
    vector2d m_resolve;

    std::unique_ptr<Procedure> m_proc;

    QBasicTimer m_timer;

    bool m_done;
};

#endif //MINOTAUR_CPP_OBJECTSTRATEGY_H
