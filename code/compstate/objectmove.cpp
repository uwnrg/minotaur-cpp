#include "objectmove.h"
#include "../gui/mainwindow.h"

namespace det_ {
    enum {
        MOVE_ALIGNMENT = 3
    };
}

ObjectMove::ObjectMove(
    std::weak_ptr<Controller> sol,
    nrg::dir dir,
    double target,
    double norm_base,
    double norm_dev
) :
    m_sol(sol),
    m_dir(dir),
    m_target(target),
    m_norm_base(norm_base),
    m_norm_dev(norm_dev),

    m_done(false),

    m_delegate(std::move(sol), {}) {}

bool ObjectMove::is_done() const {
    return m_done;
}

ObjectMove::Stop ObjectMove::get_stop() const {
    return m_stop;
}

void ObjectMove::start() {
    m_timer.start(200, this);
}

void ObjectMove::stop() {
    m_timer.stop();
}

void ObjectMove::timerEvent(QTimerEvent *ev) {
    if (ev->timerId() == m_timer.timerId()) {
        movement_loop();
    }
}

void ObjectMove::movement_loop() {
    CompetitionState &state = Main::get()->state();
    rect2d rob = state.get_robot_box(true);
    rect2d obj = state.get_object_box(true);
    vector2d obj_loc = obj.center();
    vector2d rob_loc = rob.center();
    // Check if the object has passed the target threshold
    // and that the robot is on the correct side of the object
    m_stop = Stop::OKAY;
    if (beyond_target(obj_loc)) {
        m_stop = Stop::AT_TARGET;
    } else if (exceeded_norm(obj_loc)) {
        m_stop = Stop::EXCEEDED_NORM;
    } else if (!robot_correct_side(rob_loc, obj_loc)) {
        m_stop = Stop::WRONG_SIDE;
    }
    if (m_stop != Stop::OKAY) {
        m_timer.stop();
        m_done = true;
        return;
    }
    // Make sure the robot is aligned for proper movement
    double align_err = alignment_err(rob_loc, obj_loc);
    if (align_err > det_::MOVE_ALIGNMENT) {
        // Correct for alignment
        correct(align_err);
    } else {
        // Move in desired direction
        do_move(target_err(obj_loc));
    }
}

void ObjectMove::do_move(double delta) {
    if (delta == 0) { return; }
    switch (m_dir) {
        case nrg::dir::RIGHT:
            m_delegate.move_right(delta);
            break;
        case nrg::dir::LEFT:
            m_delegate.move_left(delta);
            break;
        case nrg::dir::DOWN:
            m_delegate.move_down(delta);
            break;
        case nrg::dir::UP:
            m_delegate.move_up(delta);
            break;
        default:
            break;
    }
}

void ObjectMove::correct(double delta) {
    switch (m_dir) {
        case nrg::dir::RIGHT:
        case nrg::dir::LEFT:
            if (delta > 0) { m_delegate.move_right(delta); }
            else if (delta < 0) { m_delegate.move_left(-delta); }
            break;
        case nrg::dir::DOWN:
        case nrg::dir::UP:
            if (delta > 0) { m_delegate.move_down(delta); }
            else if (delta < 0) { m_delegate.move_up(delta); }
            break;
        default:
            break;
    }
}

bool ObjectMove::beyond_target(const vector2d &obj_loc) const {
    switch (m_dir) {
        case nrg::dir::RIGHT:
            return obj_loc.x() > m_target;
        case nrg::dir::LEFT:
            return obj_loc.x() < m_target;
        case nrg::dir::DOWN:
            return obj_loc.y() > m_target;
        case nrg::dir::UP:
            return obj_loc.y() < m_target;
        default:
            return false;
    }
}

bool ObjectMove::robot_correct_side(const vector2d &rob_loc, const vector2d &obj_loc) const {
    switch (m_dir) {
        case nrg::dir::RIGHT:
            return rob_loc.x() < obj_loc.x();
        case nrg::dir::LEFT:
            return rob_loc.x() > obj_loc.x();
        case nrg::dir::DOWN:
            return rob_loc.y() < obj_loc.y();
        case nrg::dir::UP:
            return rob_loc.y() > obj_loc.y();
        default:
            return false;
    }
}

bool ObjectMove::exceeded_norm(const vector2d &obj_loc) const {
    double delta;
    switch (m_dir) {
        case nrg::dir::RIGHT:
        case nrg::dir::LEFT:
            delta = obj_loc.y() - m_norm_base;
            break;
        case nrg::dir::DOWN:
        case nrg::dir::UP:
            delta = obj_loc.x() - m_norm_base;
            break;
        default:
            delta = 0;
            break;
    }
    return fabs(delta) > m_norm_dev;
}

double ObjectMove::alignment_err(const vector2d &rob_loc, const vector2d &obj_loc) const {
    switch (m_dir) {
        case nrg::dir::RIGHT:
        case nrg::dir::LEFT:
            return obj_loc.y() - rob_loc.y();
        case nrg::dir::DOWN:
        case nrg::dir::UP:
            return obj_loc.x() - rob_loc.x();
        default:
            return 0;
    }
}

double ObjectMove::target_err(const vector2d &obj_loc) const {
    switch (m_dir) {
        case nrg::dir::RIGHT:
            return m_target - obj_loc.x();
        case nrg::dir::LEFT:
            return obj_loc.x() - m_target;
        case nrg::dir::DOWN:
            return m_target - obj_loc.y();
        case nrg::dir::UP:
            return obj_loc.y() - m_target;
        default:
            return 0;
    }
}
