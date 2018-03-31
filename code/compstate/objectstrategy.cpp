#include "objectstrategy.h"
#include "common.h"
#include "../utility/algorithm.h"
#include "../gui/mainwindow.h"

namespace det_ {
    enum {
        COLLIDE_NORM_DEV = 4,
        COLLIDE_LOC_ACPT = 3,

        RDY_MV_NORM_DEV = 3,
        RDY_MV_LOC_ACPT = 2
    };
}

ObjectStrategy::ObjectStrategy(std::weak_ptr<Controller> sol, double delta, nrg::dir dir) :
    m_sol(std::move(sol)),
    m_delta(delta),
    m_dir(dir),

    m_state(State::UNINITIALIZED) {}

void ObjectStrategy::start() {
    m_timer.start(10, this);
}

void ObjectStrategy::stop() {
    m_timer.stop();
}

void ObjectStrategy::timerEvent(QTimerEvent *ev) {
    if (ev->timerId() == m_timer.timerId()) {
        movement_loop();
    }
}

void ObjectStrategy::movement_loop() {
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

void ObjectStrategy::do_uninitialized() {
    CompetitionState &state = Main::get()->state();
    if (
        !state.is_object_box_fresh() ||
        !state.is_object_box_valid() ||
        !state.is_robot_box_fresh() ||
        !state.is_robot_box_valid()
    ) {
        return;
    }
    // Initialize the target location
    rect2d obj_rect = state.get_object_box(true);
    m_target = obj_rect.center();
    switch (m_dir) {
        case nrg::dir::RIGHT:
            m_target.x() += m_delta;
            break;
        case nrg::dir::LEFT:
            m_target.x() -= m_delta;
            break;
        case nrg::dir::DOWN:
            m_target.y() += m_delta;
            break;
        case nrg::dir::UP:
            m_target.y() -= m_delta;
            break;
        default:
            break;
    }
    // Check for collision
    ready_move_collide_block(state, obj_rect);
}

void ObjectStrategy::ready_move_collide_block(CompetitionState &state, const rect2d &obj_rect) {
    rect2d rob_rect = state.get_robot_box(true);
    if (algo::aabb_collide(rob_rect, obj_rect)) {
        vector2d resolve_delta = algo::resolve_aabb_collide(rob_rect, obj_rect);
        resolve_delta.x() *= 1.5;
        resolve_delta.y() *= 1.5;
        m_resolve = rob_rect.center() + resolve_delta;
        m_state = State::COLLIDING;
    } else {
        m_state = State::READY_MOVE;
    }
}

void ObjectStrategy::do_colliding() {
#ifndef NDEBUG
    assert(m_resolve.x() != 0);
    assert(m_resolve.y() != 0);
#endif
    m_proc = std::make_unique<Procedure>(
        m_sol, {m_resolve},
        det_::COLLIDE_LOC_ACPT, det_::COLLIDE_NORM_DEV
    );
    m_proc->start();
    m_state = State::COLLIDING_PROC;
}

void ObjectStrategy::do_colliding_proc() {
#ifndef NDEBUG
    assert(!!m_proc);
#endif
    if (m_proc->is_done()) {
        m_proc.reset();
    } else {
        return;
    }
    // Check for collision
    CompetitionState &state = Main::get()->state();
    rect2d obj_rect = state.get_object_box(true);
    ready_move_collide_block(state, obj_rect);
}

nrg::dir move_dir_to_side(nrg::dir dir) {
    // Direction is cyclic modulus 4 and
    // TOP + 2 = BOTTOM
    return (dir + 2) % 4;
}

void ObjectStrategy::do_ready_move() {
    CompetitionState &state = Main::get()->state();
    // Grab and consume object and robot boxes
    rect2d obj_rect = state.get_object_box(true);
    rect2d rob_rect = state.get_robot_box(true);
#ifndef NDEBUG
    assert(!algo::aabb_collide(obj_rect, rob_rect));
#endif
    // Generate the traverse path
    path2d path = algo::robot_object_path(rob_rect, obj_rect, move_dir_to_side(m_dir));
    // Create the procedure and hand over control
    m_proc = std::make_unique<Procedure>(
        m_sol, path,
        det_::RDY_MV_LOC_ACPT, det_::RDY_MV_NORM_DEV
    );
    m_proc->start();
    m_state = State::READY_MOVE_PROC;
}

void ObjectStrategy::do_ready_move_proc() {
#ifndef NDEBUG
    assert(!!m_proc);
#endif
    if (m_proc->is_done()) {
        m_proc.reset();
    } else {
        return;
    }
    m_state = PUSHING;
}
