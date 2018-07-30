#include "compstate.h"
#include "objectline.h"
#include "objectmove.h"
#include "parammanager.h"
#include "readymove.h"

#include "../camera/statusbox.h"
#include "../camera/statuslabel.h"
#include "../gui/global.h"
#include "../utility/algorithm.h"
#include "../utility/logger.h"
#include "../utility/vector.h"

#include <QBasicTimer>
#include <QTimerEvent>
#include <cassert>

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

class ObjectLine::Impl {
public:
    Impl(int t_dir, double t_target, double t_base, State t_state);
    nrg::dir dir;
    double target;
    double base;
    State state;
    QBasicTimer timer;
    nrg::dir correction_dir;
};

ObjectLine::Impl::Impl(int t_dir, double t_target, double t_base, State t_state) :
    dir(static_cast<nrg::dir>(t_dir)),
    target(t_target),
    base(t_base),
    state(t_state) {}

ObjectLine::ObjectLine(
    std::weak_ptr<Controller> sol,
    int dir,
    double target,
    double base
) :
    m_sol(std::move(sol)),
    m_impl(std::make_unique<Impl>(dir, target, base, State::REQUIRE_READY_MOVE)),
    m_done(false) {
    if (auto lp = Main::get()->status_box().lock()) {
        m_state_label = lp->add_label("Line State: 0");
    }
}

ObjectLine::~ObjectLine() {
    if (auto lp = Main::get()->status_box().lock()) {
        lp->remove_label(m_state_label);
    }
}

void ObjectLine::start() {
    m_impl->timer.start(g_pm->timer_fast, this);
}

void ObjectLine::stop() {
    m_impl->timer.stop();
    if (m_ready_move != nullptr) {
        m_ready_move->stop();
    }
    if (m_object_move != nullptr) {
        m_object_move->stop();
    }
}

bool ObjectLine::is_done() const {
    return m_done;
}

void ObjectLine::timerEvent(QTimerEvent *ev) {
    if (ev->timerId() == m_impl->timer.timerId()) {
        movement_loop();
    }
}

void ObjectLine::movement_loop() {
    CompetitionState &state = Main::get()->state();
    if (
        !state.is_robot_box_fresh() ||
        !state.is_robot_box_valid() ||
        !state.is_object_box_fresh() ||
        !state.is_object_box_valid()
    ) { return; }

    log() << "Line State: " <<  m_impl->state;
    m_state_label->setText("Line State: " + QString::number(m_impl->state));
    switch (m_impl->state) {
        case State::REQUIRE_READY_MOVE:
            do_require_ready_move();
            break;
        case State::DOING_READY_MOVE:
            do_doing_ready_move();
            break;
        case State::REQUIRE_OBJECT_MOVE:
            do_require_object_move();
            break;
        case State::DOING_OBJECT_MOVE:
            do_doing_object_move();
            break;
        case State::REQUIRE_CORRECTION:
            do_require_correction();
            break;
        case State::REQUIRE_CORRECTION_READY_MOVE:
            do_require_correction_ready_move();
            break;
        case State::DOING_CORRECTION_READY_MOVE:
            do_doing_correction_ready_move();
            break;
        case State::REQUIRE_CORRECTION_OBJECT_MOVE:
            do_require_correction_object_move();
            break;
        case State::DOING_CORRECTION_OBJECT_MOVE:
            do_doing_correction_object_move();
            break;
        default:
            break;
    }
}

void ObjectLine::do_require_ready_move() {
    // Find the side of the object on which the robot needs to be
    nrg::dir side_dir = invert_dir(m_impl->dir);
    m_ready_move = std::make_unique<ReadyMove>(m_sol, side_dir);
    // Hand control over to ReadyMove
    m_ready_move->start();
    m_impl->state = State::DOING_READY_MOVE;
}

void ObjectLine::do_doing_ready_move() {
#ifndef NDEBUG
    assert(!!m_ready_move);
#endif
    // When the ReadyMove is done, process the ObjectMove
    if (m_ready_move->is_done()) {
        m_ready_move.reset();
        m_impl->state = State::REQUIRE_OBJECT_MOVE;
    }
}

void ObjectLine::do_require_object_move() {
    nrg::dir dir = m_impl->dir;
    double target = m_impl->target;
    double norm_base = m_impl->base;
    double norm_dev = g_pm->objline_move_dev;
    // Hand control over to the ObjectMove
    m_object_move = std::make_unique<ObjectMove>(m_sol, dir, target, norm_base, norm_dev);
    m_object_move->start();
    m_impl->state = State::DOING_OBJECT_MOVE;
}

void ObjectLine::do_doing_object_move() {
#ifndef NDEBUG
    assert(!!m_object_move);
#endif
    if (!m_object_move->is_done()) {
        return;
    }
    // Check the stop condition
    ObjectMove::Stop stop_cond = m_object_move->get_stop();
#ifndef NDEBUG
    assert(stop_cond != ObjectMove::Stop::OKAY);
#endif
    m_object_move.reset();
    log() << "Stop Condition: " << stop_cond;
    switch (stop_cond) {
        case ObjectMove::Stop::AT_TARGET:
            // If the ObjectMove is at the target, then this procedure is complete
            m_impl->timer.stop();
            m_done = true;
            return;
        case ObjectMove::Stop::WRONG_SIDE:
            // Go back to needing a ReadyMove
            m_impl->state = State::REQUIRE_READY_MOVE;
            return;
        case ObjectMove::Stop::EXCEEDED_NORM:
            // Perform a correction
            m_impl->state = State::REQUIRE_CORRECTION;
            return;
        default:
            return;
    }
}

void ObjectLine::do_require_correction() {
    // Determine the correction direction
    CompetitionState &state = Main::get()->state();
    vector2d obj_loc = rect2d(state.get_object_box(true)).center();
    switch (m_impl->dir) {
        case nrg::dir::RIGHT:
        case nrg::dir::LEFT:
            m_impl->correction_dir = obj_loc.y() < m_impl->base ? nrg::dir::DOWN : nrg::dir::UP;
            break;
        case nrg::dir::DOWN:
        case nrg::dir::UP:
            m_impl->correction_dir = obj_loc.x() < m_impl->base ? nrg::dir::RIGHT : nrg::dir::LEFT;
            break;
        default:
            break;
    }
    m_impl->state = State::REQUIRE_CORRECTION_READY_MOVE;
}

void ObjectLine::do_require_correction_ready_move() {
    nrg::dir side_dir = invert_dir(m_impl->correction_dir);
    // ReadyMove over to the correction side
    m_ready_move = std::make_unique<ReadyMove>(m_sol, side_dir);
    m_ready_move->start();
    m_impl->state = State::DOING_CORRECTION_READY_MOVE;
}

void ObjectLine::do_doing_correction_ready_move() {
#ifndef NDEBUG
    assert(!!m_ready_move);
#endif
    if (m_ready_move->is_done()) {
        m_ready_move.reset();
        // Ready to perform correction move
        m_impl->state = State::REQUIRE_CORRECTION_OBJECT_MOVE;
    }
}

void ObjectLine::do_require_correction_object_move() {
    nrg::dir dir = m_impl->correction_dir;
    double target = m_impl->base;
    // Base and deviation don't matter in this case
    double norm_base = 0;
    double norm_dev = std::numeric_limits<double>::max();
    m_object_move = std::make_unique<ObjectMove>(m_sol, dir, target, norm_base, norm_dev);
    // Hand over control
    m_object_move->start();
    m_impl->state = State::DOING_CORRECTION_OBJECT_MOVE;
}

void ObjectLine::do_doing_correction_object_move() {
#ifndef NDEBUG
    assert(!!m_object_move);
#endif
    if (!m_object_move->is_done()) {
        return;
    }
    ObjectMove::Stop stop_cond = m_object_move->get_stop();
#ifndef NDEBUG
    // Should not stop when condition is OKAY
    assert(stop_cond != ObjectMove::Stop::OKAY);
    // Should never stop due to exceeded normal
    assert(stop_cond != ObjectMove::Stop::EXCEEDED_NORM);
#endif
    m_object_move.reset();
    m_impl->state = State::REQUIRE_READY_MOVE;
}
