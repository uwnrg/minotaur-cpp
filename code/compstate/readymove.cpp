#include "common.h"
#include "compstate.h"
#include "readymove.h"
#include "parammanager.h"
#include "procedure.h"

#include "../camera/statusbox.h"
#include "../camera/statuslabel.h"
#include "../gui/mainwindow.h"
#include "../gui/global.h"
#include "../utility/algorithm.h"
#include "../utility/logger.h"
#include "../utility/utility.h"

#include <QBasicTimer>
#include <QTimerEvent>

#ifndef NDEBUG
#include <cassert>
#endif

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

class ReadyMove::Impl {
public:
    Impl(int t_dir, State t_state);

    /**
     * The desired side of the object to be on.
     */
    nrg::dir dir;
    /**
     * Internal state of the object.
     */
    State state;
    /**
     * The collision resolution vector.
     */
    vector2d resolve;
    QBasicTimer timer;
};

ReadyMove::Impl::Impl(int t_dir, State t_state) :
    dir(static_cast<nrg::dir>(t_dir)),
    state(t_state) {}

ReadyMove::ReadyMove(std::weak_ptr<Controller> sol, int dir) :
    m_impl(std::make_unique<Impl>(dir, State::UNINITIALIZED)),
    m_sol(std::move(sol)),
    m_done(false) {
    if (auto lp = Main::get()->status_box().lock()) {
        m_state_label = lp->add_label("Ready State: 0");
    }
}

ReadyMove::~ReadyMove() {
    if (auto lp = Main::get()->status_box().lock()) {
        lp->remove_label(m_state_label);
    }
}

void ReadyMove::start() {
    m_impl->timer.start(g_pm->timer_fast, this);
}

void ReadyMove::stop() {
    m_impl->timer.stop();
    if (m_proc != nullptr) {
        m_proc->stop();
    }
}

void ReadyMove::timerEvent(QTimerEvent *ev) {
    if (ev->timerId() == m_impl->timer.timerId()) {
        movement_loop();
    }
}

void ReadyMove::movement_loop() {
    CompetitionState &state = Main::get()->state();
    if (!state.is_object_box_fresh() ||
        !state.is_object_box_valid() ||
        !state.is_robot_box_fresh() ||
        !state.is_robot_box_valid()) {
        return;
    }
    log() << "Ready Move State: " << m_impl->state;
    m_state_label->setText("Ready State: " + QString::number(m_impl->state));
    switch (m_impl->state) {
        case UNINITIALIZED:
            do_uninitialized();
            break;
        case COLLIDING:
            do_colliding();
            break;
        case COLLIDING_PROC:
            do_colliding_proc();
            break;
        case READY_MOVE:
            do_ready_move();
            break;
        case READY_MOVE_PROC:
            do_ready_move_proc();
            break;
        default:
            break;
    }
}

void ReadyMove::do_uninitialized() {
    // Check for collision first
    CompetitionState &state = Main::get()->state();
    rect2d obj_rect = state.get_object_box(true);
    rect2d rob_rect = state.get_robot_box(true);
    if (algo::aabb_collide(rob_rect, obj_rect)) {
        vector2d resolve_delta = algo::resolve_aabb_collide(rob_rect, obj_rect);
        resolve_delta.x() *= 1.5;
        resolve_delta.y() *= 1.5;
        // If the bounding boxes collide resolve the collision
        // before moving
        m_impl->resolve = rob_rect.center() + resolve_delta;
        m_impl->state = State::COLLIDING;
    } else {
        // Ready to move to the correct side
        m_impl->state = State::READY_MOVE;
    }
}

void ReadyMove::do_colliding() {
#ifndef NDEBUG
    assert(m_impl->resolve.x() != 0);
    assert(m_impl->resolve.y() != 0);
#endif
    path2d path = {m_impl->resolve};
    // Create a procedure whose goal is to move to the location
    // that resolves the collision
    m_proc = std::make_unique<Procedure>(
        m_sol, path,
        g_pm->objproc_loc_acpt, g_pm->objproc_norm_dev
    );
    m_proc->start();
    m_impl->state = State::COLLIDING_PROC;
}

void ReadyMove::do_colliding_proc() {
#ifndef NDEBUG
    assert(!!m_proc);
#endif
    // Check to see if the procedure has completed
    if (m_proc->is_done()) {
        m_proc.reset();
    } else {
        return;
    }
    // Ready to move
    m_impl->state = State::READY_MOVE;
}

void ReadyMove::do_ready_move() {
    CompetitionState &state = Main::get()->state();
    // Grab and consume object and robot boxes
    rect2d obj_rect = state.get_object_box(true);
    rect2d rob_rect = state.get_robot_box(true);
#ifndef NDEBUG
    //assert(!algo::aabb_collide(obj_rect, rob_rect));
#endif
    // Generate the traverse path
    path2d path = algo::robot_object_path(rob_rect, obj_rect, m_impl->dir);
    // Create the procedure and hand over control
    m_proc = std::make_unique<Procedure>(
        m_sol, path,
        g_pm->objproc_loc_acpt, g_pm->objproc_norm_dev
    );
    m_proc->start();
    m_impl->state = State::READY_MOVE_PROC;
}

void ReadyMove::do_ready_move_proc() {
#ifndef NDEBUG
    assert(!!m_proc);
#endif
    if (m_proc->is_done()) {
        m_proc.reset();
    } else {
        return;
    }
    // When this procedure is finished, this ReadyMove is finished
    m_impl->timer.stop();
    m_done = true;
}

bool ReadyMove::is_done() const {
    return m_done;
}
