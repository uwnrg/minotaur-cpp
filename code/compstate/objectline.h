#ifndef MINOTAUR_CPP_OBJECTLINE_H
#define MINOTAUR_CPP_OBJECTLINE_H

#include <QObject>
#include <memory>

class Controller;
class StatusLabel;
class ObjectMove;
class ReadyMove;

/**
 * This object combines ReadyMove and ObjectMove to have the robot push the
 * object along an axis-aligned line. This object will place the robot on the
 * correct side of the object to begin moving, and then hand control over to
 * ObjectMove.
 *
 * If the ReadyMove returns WRONG_SIDE, this class will move the robot
 * back to the correct side. If ReadyMove returns EXCEEDED_NORM, ObjectLine will
 * move the robot to a correction side and use ObjectMove to reduce the normal
 * deviation, without caring for displacement along the line of motion.
 */
class ObjectLine : public QObject {
Q_OBJECT

public:
    explicit ObjectLine(std::weak_ptr<Controller> sol, int dir, double target, double base);
    ~ObjectLine() override;

    void start();
    void stop();

    bool is_done() const;

private:
    void timerEvent(QTimerEvent *ev) override;

    class Impl;
    void movement_loop();

    void do_require_ready_move();
    void do_doing_ready_move();
    void do_require_object_move();
    void do_doing_object_move();

    void do_require_correction();
    void do_require_correction_ready_move();
    void do_doing_correction_ready_move();
    void do_require_correction_object_move();
    void do_doing_correction_object_move();

    std::weak_ptr<Controller> m_sol;
    std::unique_ptr<Impl> m_impl;
    bool m_done;

    StatusLabel *m_state_label;

    std::unique_ptr<ReadyMove> m_ready_move;
    std::unique_ptr<ObjectMove> m_object_move;

};

#endif //MINOTAUR_CPP_OBJECTLINE_H
