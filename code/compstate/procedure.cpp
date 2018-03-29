#include "procedure.h"
#include "../controller/controller.h"
#include "../gui/mainwindow.h"

#define DEFAULT_TARGET_LOC_ACCEPTANCE 3.0
#define DEFAULT_MAX_NORMAL_DEVIATION  15.0

#define DIR_RIGHT "RIGHT"
#define DIR_LEFT  "LEFT"
#define DIR_DOWN  "DOWN"
#define DIR_UP    "UP"

vector2d rect_center(const cv::Rect2d &rect) {
    return {rect.x + rect.width / 2, rect.y + rect.height / 2};
}

vector2d perp_intersect(
    const vector2d &c,
    const vector2d &p0,
    const vector2d &p1
) {
    double dy1 = p1.y() - p0.y();
    double dx1 = p1.x() - p0.x();
    double m_l = dy1 / dx1;
    double m_r = -1 / m_l;
    // y - y0 = (x - x0) * m_l
    // y - yc = (x - xc) * m_r
    // y0 - yc = (x - xc) * m_r + (x0 - x) * m_l
    // y0 - yc + xc * m_r - x0 * m_l = x * (m_r - m_l)
    double xi = (p0.y() - c.y() + c.x() * m_r - p0.x() * m_l) / (m_r - m_l);
    // y - y0 = (x - x0) * m_l
    // yi = (xi - x0) * m_l + y0
    double yi = (xi - p0.x()) * m_l + p0.y();
    return {xi, yi};
}

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

Procedure::Procedure(std::weak_ptr<Controller> sol, const path2d &path) :
    m_loc_accept(DEFAULT_TARGET_LOC_ACCEPTANCE),
    m_norm_dev(DEFAULT_MAX_NORMAL_DEVIATION),
    m_path(path),
    m_index(0),
    m_sol(std::move(sol)) {
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

void Procedure::start() {
    m_timer.start(200, this);
    m_initial = rect_center(Main::get()->state().get_robot_box());
}

void Procedure::stop() {
    m_timer.stop();
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
        return;
    }

    // If the box has not been updated, tracker has lost acquisition, skip this loop
    if (
        !Main::get()->state().is_robot_box_fresh() ||
        !Main::get()->state().is_robot_box_valid()
        ) { return; }

    // Acquire the current robot position
    vector2d center = rect_center(Main::get()->state().get_robot_box(true));
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
    vector2d intersect = perp_intersect(center, source, target);
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
    m_dir_label->setText(DIR_RIGHT);
    if (auto sol = m_sol.lock()) {
        sol->move({static_cast<int>(estimated_power), 0});
    }
}

void Procedure::move_left(double estimated_power) {
    // Left => -X
    m_dir_label->setText(DIR_LEFT);
    if (auto sol = m_sol.lock()) {
        sol->move({-static_cast<int>(estimated_power), 0});
    }
}

void Procedure::move_up(double estimated_power) {
    // Up => -Y
    m_dir_label->setText(DIR_UP);
    if (auto sol = m_sol.lock()) {
        sol->move({0, -static_cast<int>(estimated_power)});
    }
}

void Procedure::move_down(double estimated_power) {
    // Down => +Y
    m_dir_label->setText(DIR_DOWN);
    if (auto sol = m_sol.lock()) {
        sol->move({0, static_cast<int>(estimated_power)});
    }
}
