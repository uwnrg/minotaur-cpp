#include "procedure.h"
#include "common.h"
#include "../controller/controller.h"
#include "../gui/mainwindow.h"

#define DIR_RIGHT "RIGHT"
#define DIR_LEFT  "LEFT"
#define DIR_DOWN  "DOWN"
#define DIR_UP    "UP"

QString err_text(double x, double y) {
    QString text;
    text.sprintf("Error: (%6.1f , %6.1f )", x, y);
    return text;
}

QString index_text(std::size_t index) {
    return "Index: " + QString::number(index);
}

QString perp_text(double err_x, double err_y, double norm_sq) {
    QString text;
    text.sprintf("PerpD: (%6.1f , %6.1f ) : %6.1f", err_x, err_y, norm_sq);
    return text;
}

Procedure::Procedure(
    std::weak_ptr<Controller> sol,
    const path2d &path,
    double loc_accept,
    double norm_dev
) :
    m_loc_accept(loc_accept),
    m_norm_dev(norm_dev),
    m_path(path),
    m_index(0),
    m_sol(std::move(sol)),
    m_done(false) {
    if (auto lp = Main::get()->status_box().lock()) {
        m_dir_label = lp->add_label("IDLE");
        m_err_label = lp->add_label(err_text(0, 0));
        m_index_label = lp->add_label(index_text(m_index));
        m_perp_label = lp->add_label(perp_text(0, 0, 0));
    }
}

Procedure::~Procedure() {
    if (auto lp = Main::get()->status_box().lock()) {
        lp->remove_label(m_dir_label);
        lp->remove_label(m_err_label);
        lp->remove_label(m_index_label);
        lp->remove_label(m_perp_label);
    }
}

bool Procedure::is_done() const {
    return m_done;
}

bool Procedure::is_stopped() const {
    return !m_timer.isActive();
}

void Procedure::start() {
    m_timer.start(200, this);
    m_initial = algo::rect_center(Main::get()->state().get_robot_box());
    Q_EMIT started();
}

void Procedure::stop() {
    m_timer.stop();
    Q_EMIT stopped();
}

void Procedure::timerEvent(QTimerEvent *ev) {
    if (ev->timerId() == m_timer.timerId()) {
        movement_loop();
    }
}

void Procedure::movement_loop() {
    // If the path has been traversed or solenoid expired, stop timer
    if (m_index == m_path.size() || m_sol.expired()) {
        m_timer.stop();
        m_done = true;
        Q_EMIT finished();
        return;
    }

    // If the box has not been updated, tracker has lost acquisition, skip this loop
    if (
        !Main::get()->state().is_robot_box_fresh() ||
        !Main::get()->state().is_robot_box_valid()
    ) { return; }

    // Acquire the current robot position
    vector2d center = algo::rect_center(Main::get()->state().get_robot_box(true));
    vector2d target = m_path[m_index];
    vector2d source = m_index > 0 ? m_path[m_index - 1] : m_initial;

    // Find differences in each axis
    double err_x = target.x() - center.x();
    double err_y = target.y() - center.y();
    m_err_label->setText(err_text(err_x, err_y));

    // If within acceptance range, move to next point
    if (hypot(err_x, err_y) < m_loc_accept) {
        ++m_index;
        return;
    }
    m_index_label->setText(index_text(m_index));

    // Calculate perpendicular distance to ensure the robot is straddling the line
    vector2d intersect = algo::perp_intersect(center, source, target);
    vector2d norm_diff = intersect - center;
    double norm_diff_sq = norm_diff.norm_sq();
    m_perp_label->setText(perp_text(norm_diff.x(), norm_diff.y(), norm_diff_sq));
    if (norm_diff_sq > m_norm_dev * m_norm_dev) {
        target = intersect;
        err_x = norm_diff.x();
        err_y = norm_diff.y();
    }

    // Attempt to reduce the error
    if (fabs(err_x) > fabs(err_y)) {
        double estimated_power = fabs(err_x);
        // Move in horizontal
        if (target.x() > center.x()) { move_right(estimated_power); }
        else { move_left(estimated_power); }
    } else {
        double estimated_power = fabs(err_y);
        // Move in vertical
        if (target.y() > center.y()) { move_down(estimated_power); }
        else { move_up(estimated_power); }
    }
}

void Procedure::move_right(double estimated_power) {
    // Right => +X
    if (m_dir_label) { m_dir_label->setText(DIR_RIGHT); }
    if (auto sol = m_sol.lock()) {
        sol->move({static_cast<int>(estimated_power), 0});
    }
}

void Procedure::move_left(double estimated_power) {
    // Left => -X
    if (m_dir_label) { m_dir_label->setText(DIR_LEFT); }
    if (auto sol = m_sol.lock()) {
        sol->move({-static_cast<int>(estimated_power), 0});
    }
}

void Procedure::move_up(double estimated_power) {
    // Up => -Y
    if (m_dir_label) { m_dir_label->setText(DIR_UP); }
    if (auto sol = m_sol.lock()) {
        sol->move({0, -static_cast<int>(estimated_power)});
    }
}

void Procedure::move_down(double estimated_power) {
    // Down => +Y
    if (m_dir_label) { m_dir_label->setText(DIR_DOWN); }
    if (auto sol = m_sol.lock()) {
        sol->move({0, static_cast<int>(estimated_power)});
    }
}
