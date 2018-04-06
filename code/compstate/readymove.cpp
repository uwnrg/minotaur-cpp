#include "readymove.h"
#include "common.h"
#include "../utility/algorithm.h"
#include "../gui/mainwindow.h"

namespace det_ {
    enum {
        COLLIDE_NORM_DEV = 4,
        COLLIDE_LOC_ACPT = 3,

        RDY_MV_NORM_DEV = 4,
        RDY_MV_LOC_ACPT = 3,
    };
}

ReadyMove::ReadyMove(std::weak_ptr<Controller> sol, nrg::dir dir) :
    m_sol(std::move(sol)),
    m_dir(dir),

    m_state(State::UNINITIALIZED),

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
    m_timer.start(Procedure::TIMER_FAST, this);
}

void ReadyMove::stop() {
    m_timer.stop();
}

void ReadyMove::timerEvent(QTimerEvent *ev) {
    if (ev->timerId() == m_timer.timerId()) {
        movement_loop();
    }
}

void ReadyMove::movement_loop() {
    CompetitionState &state = Main::get()->state();
    if (
        !state.is_object_box_fresh() ||
        !state.is_object_box_valid() ||
        !state.is_robot_box_fresh() ||
        !state.is_robot_box_valid()
        ) {
        return;
    }
    log() << "Ready Move State: " << m_state;
    m_state_label->setText("Ready State: " + QString::number(m_state));
    switch (m_state) {
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
        m_resolve = rob_rect.center() + resolve_delta;
        m_state = State::COLLIDING;
    } else {
        // Ready to move to the correct side
        m_state = State::READY_MOVE;
    }
}

void ReadyMove::do_colliding() {
#ifndef NDEBUG
    assert(m_resolve.x() != 0);
    assert(m_resolve.y() != 0);
#endif
    path2d path = {m_resolve};
    // Create a procedure whose goal is to move to the location
    // that resolves the collision
    m_proc = std::make_unique<Procedure>(
        m_sol, path,
        det_::COLLIDE_LOC_ACPT, det_::COLLIDE_NORM_DEV
    );
    m_proc->start();
    m_state = State::COLLIDING_PROC;
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
    m_state = State::READY_MOVE;
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
    path2d path = algo::robot_object_path(rob_rect, obj_rect, m_dir);
    // Create the procedure and hand over control
    m_proc = std::make_unique<Procedure>(
        m_sol, path,
        det_::RDY_MV_LOC_ACPT, det_::RDY_MV_NORM_DEV
    );
    m_proc->start();
    m_state = State::READY_MOVE_PROC;
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
    m_timer.stop();
    m_done = true;
}

bool ReadyMove::is_done() const {
    return m_done;
}
