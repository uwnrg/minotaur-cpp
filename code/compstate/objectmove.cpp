#include "compstate.h"
#include "objectmove.h"
#include "parammanager.h"
#include "procedure.h"

#include "../camera/statusbox.h"
#include "../camera/statuslabel.h"
#include "../gui/mainwindow.h"
#include "../gui/global.h"
#include "../utility/logger.h"
#include "../utility/rect.h"

#include <QBasicTimer>
#include <QTimerEvent>

static QString align_text(double align_err) {
    QString text;
    text.sprintf("Align Err:  %.1f", align_err);
    return text;
}

static QString target_text(double target_err) {
    QString text;
    text.sprintf("Target Err: %.1f", target_err);
    return text;
}

class ObjectMove::Impl {
public:
    Impl(int t_dir,
        double t_target,
        double t_norm_base,
        double t_norm_dev,
        std::weak_ptr<Controller> sol);

    nrg::dir dir;
    double target;
    double norm_base;
    double norm_dev;
    Procedure delegate;
    QBasicTimer timer;

    void do_move(double delta);
    void correct(double delta);

    /**
     * Whether the object is at or exceeded the target value.
     *
     * @param obj_loc the object's current center location
     * @return true if the object is past the target
     */
    bool beyond_target(const vector2d &obj_loc) const;
    /**
     * Whether the robot is on the correct side of the object
     * for pushing in the desired direction.
     *
     * @param rob_loc the robot's center location
     * @param obj_loc the object's center location
     * @return true if the robot is on the correct side
     */
    bool robot_correct_side(const vector2d &rob_loc, const vector2d &obj_loc) const;
    /**
     * Whether the object's location is beyond the acceptable
     * normal deviation value.
     *
     * @param obj_loc the object's center location
     * @return true if the object has exceeded the norm
     */
    bool exceeded_norm(const vector2d &obj_loc) const;
    /**
     * Determine the robot's alignment error the object's location
     * in the axis perpendicular to the axis of movement. I.e. if
     * the object is to be moved horizontally, this returns the
     * difference in y values of the robot and object.
     *
     * @param rob_loc the robot's center location
     * @param obj_loc the object's center location
     * @return the difference in alignment
     */
    double alignment_err(const vector2d &rob_loc, const vector2d &obj_loc) const;
    /**
     * @param obj_loc the object's center location
     * @return distance to the target location
     */
    double target_err(const vector2d &obj_loc) const;
};

ObjectMove::Impl::Impl(
    int t_dir,
    double t_target,
    double t_norm_base,
    double t_norm_dev,
    std::weak_ptr<Controller> sol) :
    dir(static_cast<nrg::dir>(t_dir)),
    target(t_target),
    norm_base(t_norm_base),
    norm_dev(t_norm_dev),
    delegate(std::move(sol), {}) {}

ObjectMove::ObjectMove(
    std::weak_ptr<Controller> sol,
    int dir,
    double target,
    double norm_base,
    double norm_dev
) :
    m_impl(std::make_unique<Impl>(dir, target, norm_base, norm_dev, sol)),
    m_sol(sol),
    m_done(false),
    m_stop(Stop::OKAY) {
    if (auto lp = Main::get()->status_box().lock()) {
        m_align_label = lp->add_label(align_text(0));
        m_target_label = lp->add_label(target_text(0));
    }
}

ObjectMove::~ObjectMove() {
    if (auto lp = Main::get()->status_box().lock()) {
        lp->remove_label(m_align_label);
        lp->remove_label(m_target_label);
    }
}

bool ObjectMove::is_done() const {
    return m_done;
}

ObjectMove::Stop ObjectMove::get_stop() const {
    return m_stop;
}

void ObjectMove::start() {
    m_impl->timer.start(g_pm->timer_reg, this);
}

void ObjectMove::stop() {
    m_impl->timer.stop();
}

void ObjectMove::timerEvent(QTimerEvent *ev) {
    if (ev->timerId() == m_impl->timer.timerId()) {
        movement_loop();
    }
}

void ObjectMove::movement_loop() {
    CompetitionState &state = Main::get()->state();
    if (
        !state.is_robot_box_fresh() ||
        !state.is_robot_box_valid() ||
        !state.is_object_box_fresh() ||
        !state.is_object_box_valid()
        ) {
        return;
    }
    rect2d rob = state.get_robot_box(true);
    rect2d obj = state.get_object_box(true);
    vector2d obj_loc = obj.center();
    vector2d rob_loc = rob.center();
    // Check if the object has passed the target threshold
    // and that the robot is on the correct side of the object
    m_stop = Stop::OKAY;
    if (m_impl->beyond_target(obj_loc)) {
        m_stop = Stop::AT_TARGET;
    } else if (m_impl->exceeded_norm(obj_loc)) {
        m_stop = Stop::EXCEEDED_NORM;
    } else if (!m_impl->robot_correct_side(rob_loc, obj_loc)) {
        m_stop = Stop::WRONG_SIDE;
    }
    if (m_stop != Stop::OKAY) {
        m_impl->timer.stop();
        m_done = true;
        return;
    }
    // Make sure the robot is aligned for proper movement
    double align_err = m_impl->alignment_err(rob_loc, obj_loc);
    double tgt_err = m_impl->target_err(obj_loc);
    log() << "Align Err:  " << align_err;
    log() << "Target Err: " << tgt_err;
    m_align_label->setText(align_text(align_err));
    m_target_label->setText(target_text(tgt_err));
    if (fabs(align_err) > g_pm->objmove_algn_err) {
        // Correct for alignment
        m_impl->correct(align_err);
    } else {
        // Move in desired direction
        m_impl->do_move(tgt_err);
    }
}

void ObjectMove::Impl::do_move(double delta) {
    if (delta < 1) { delta = 1; }
    switch (dir) {
        case nrg::dir::RIGHT:
            delegate.move_right(delta);
            break;
        case nrg::dir::LEFT:
            delegate.move_left(delta);
            break;
        case nrg::dir::DOWN:
            delegate.move_down(delta);
            break;
        case nrg::dir::UP:
            delegate.move_up(delta);
            break;
        default:
            break;
    }
}

void ObjectMove::Impl::correct(double delta) {
    switch (dir) {
        case nrg::dir::DOWN:
        case nrg::dir::UP:
            if (delta > 0) { delegate.move_right(delta); }
            else if (delta < 0) { delegate.move_left(-delta); }
            break;
        case nrg::dir::RIGHT:
        case nrg::dir::LEFT:
            if (delta > 0) { delegate.move_down(delta); }
            else if (delta < 0) { delegate.move_up(-delta); }
            break;
        default:
            break;
    }
}

bool ObjectMove::Impl::beyond_target(const vector2d &obj_loc) const {
    switch (dir) {
        case nrg::dir::RIGHT:
            return obj_loc.x() >= target;
        case nrg::dir::LEFT:
            return obj_loc.x() <= target;
        case nrg::dir::DOWN:
            return obj_loc.y() >= target;
        case nrg::dir::UP:
            return obj_loc.y() <= target;
        default:
            return false;
    }
}

bool ObjectMove::Impl::robot_correct_side(const vector2d &rob_loc, const vector2d &obj_loc) const {
    switch (dir) {
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

bool ObjectMove::Impl::exceeded_norm(const vector2d &obj_loc) const {
    double delta;
    switch (dir) {
        case nrg::dir::RIGHT:
        case nrg::dir::LEFT:
            delta = obj_loc.y() - norm_base;
            break;
        case nrg::dir::DOWN:
        case nrg::dir::UP:
            delta = obj_loc.x() - norm_base;
            break;
        default:
            delta = 0;
            break;
    }
    return fabs(delta) > norm_dev;
}

double ObjectMove::Impl::alignment_err(const vector2d &rob_loc, const vector2d &obj_loc) const {
    switch (dir) {
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

double ObjectMove::Impl::target_err(const vector2d &obj_loc) const {
    switch (dir) {
        case nrg::dir::RIGHT:
            return target - obj_loc.x();
        case nrg::dir::LEFT:
            return obj_loc.x() - target;
        case nrg::dir::DOWN:
            return target - obj_loc.y();
        case nrg::dir::UP:
            return obj_loc.y() - target;
        default:
            return 0;
    }
}
