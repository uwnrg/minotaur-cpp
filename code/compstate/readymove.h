#ifndef MINOTAUR_CPP_OBJECTSTRATEGY_H
#define MINOTAUR_CPP_OBJECTSTRATEGY_H

#include <QObject>
#include <memory>

class Controller;
class Procedure;
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
    explicit ReadyMove(std::weak_ptr<Controller> sol, int dir);
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
    class Impl;
    std::unique_ptr<Impl> m_impl;
    std::weak_ptr<Controller> m_sol;

    /**
     * Procedure instance used to traverse paths and
     * resolve collisions.
     */
    std::unique_ptr<Procedure> m_proc;

    StatusLabel *m_state_label;
    bool m_done;
};

#endif //MINOTAUR_CPP_OBJECTSTRATEGY_H
