#ifndef MINOTAUR_CPP_OBJECTSTRATEGY_H
#define MINOTAUR_CPP_OBJECTSTRATEGY_H

#include "procedure.h"

#include "../controller/controller.h"
#include "../utility/vector.h"
#include "../utility/rect.h"

#include <QObject>
#include <QTimer>

class StatusLabel;

/**
 * This procedure object leverages the robot Procedure with the goal
 * of moving the robot to the position around the object to move the object.
 *
 * For instance, if the object must be moved downwards, this object will
 * move the robot to the top side of the object, without colliding.
 */
class ReadyMove : public QObject {
Q_OBJECT

public:
    /**
     * States of the object. The ReadyMove can be uninitialized, which requires
     * checking the status of the robot.
     *
     * In COLLIDING, the robot and object's bounding box collision is resolved, and
     * in READY_MOVE the robot is moved along a path to the correct location.
     */
    enum State {
        UNINITIALIZED,
        COLLIDING,
        READY_MOVE,

        COLLIDING_PROC,
        READY_MOVE_PROC,
    };

    explicit ReadyMove(std::weak_ptr<Controller> sol, nrg::dir dir);
    ~ReadyMove();

    void start();
    void stop();

    bool is_done() const;

private:
    void timerEvent(QTimerEvent *ev) override;

    void movement_loop();

    void do_uninitialized();
    void do_colliding();
    void do_colliding_proc();
    void do_ready_move();
    void do_ready_move_proc();

private:
    std::weak_ptr<Controller> m_sol;
    /**
     * The desired side of the object to be on.
     */
    nrg::dir m_dir;

    /**
     * Internal state of the object.
     */
    State m_state;
    /**
     * The collision resolution vector.
     */
    vector2d m_resolve;

    /**
     * Procedure instance used to traverse paths and
     * resolve collisions.
     */
    std::unique_ptr<Procedure> m_proc;

    StatusLabel *m_state_label;

    QBasicTimer m_timer;

    bool m_done;
};

#endif //MINOTAUR_CPP_OBJECTSTRATEGY_H
