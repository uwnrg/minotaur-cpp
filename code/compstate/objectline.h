#ifndef MINOTAUR_CPP_OBJECTLINE_H
#define MINOTAUR_CPP_OBJECTLINE_H

#include <QObject>

#include "objectmove.h"
#include "readymove.h"

class StatusLabel;

class ObjectLine : public QObject {
Q_OBJECT

public:
    explicit ObjectLine(std::weak_ptr<Controller> sol, nrg::dir dir, double target, double base);
    ~ObjectLine() override;

    void start();
    void stop();

    bool is_done() const;

private:
    enum State {
        REQUIRE_READY_MOVE,
        DOING_READY_MOVE,

        REQUIRE_OBJECT_MOVE,
        DOING_OBJECT_MOVE,

        REQUIRE_CORRECTION,
        REQUIRE_CORRECTION_READY_MOVE,
        DOING_CORRECTION_READY_MOVE,
        REQUIRE_CORRECTION_OBJECT_MOVE,
        DOING_CORRECTION_OBJECT_MOVE
    };

    void timerEvent(QTimerEvent *ev) override;

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
    nrg::dir m_dir;
    double m_target;
    double m_base;

    bool m_done;

    State m_state;

    QBasicTimer m_timer;

    StatusLabel *m_state_label;

    std::unique_ptr<ReadyMove> m_ready_move;
    std::unique_ptr<ObjectMove> m_object_move;

    nrg::dir m_correction_dir;
};

#endif //MINOTAUR_CPP_OBJECTLINE_H
