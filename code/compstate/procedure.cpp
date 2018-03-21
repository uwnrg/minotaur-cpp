#include "procedure.h"
#include "manager.h"
#include "../controller/solenoid.h"

#include <QTimerEvent>

#define DEFAULT_TARGET_LOC_ACCEPTANCE 3.0

cv::Point2d center(const cv::Rect2d &rect) {
    return {rect.x + rect.width / 2, rect.y + rect.height / 2};
}

Procedure::Procedure(std::weak_ptr<Solenoid> sol, path2d<double> &&path) :
    m_loc_accept(DEFAULT_TARGET_LOC_ACCEPTANCE),
    m_path(std::move(path)),
    m_index(0),
    m_sol(std::move(sol)) {}

void Procedure::timerEvent(QTimerEvent *ev) {
    if (ev->timerId() == m_timer.timerId()) {
        movement_loop();
    }
}

void Procedure::movement_loop() {
    // If the path has been traversed, stop timer
    if (m_index == m_path.size()) {
        m_timer.stop();
        return;
    }

    // If the box has not been updated, tracker has lost acquisition
    // or the pointer has expired stop the timer and loop
    if (
        !Manager::state().is_robot_box_fresh() ||
        !Manager::state().is_robot_box_valid() ||
        m_sol.expired()
    ) {
        m_timer.stop();
        return;
    }

    // Acquire the current robot position
    cv::Point2d l_center = center(Manager::state().get_robot_box(true));
    vector2d<double> target = m_path[m_index];

    // Find differences in each axis
    double err_x = target.x() - l_center.x;
    double err_y = target.y() - l_center.y;
    // If within acceptance range, move to next point
    if (hypot(err_x, err_y) < m_loc_accept) {
        ++m_index;
        return;
    }

    // Attempt to reduce the error
    if (err_x > err_y) {
        double estimated_power = fabs(err_x);
        // Move in horizontal
        if (target.x() > l_center.x) { move_right(estimated_power); }
        else { move_left(estimated_power); }
    } else {
        double estimated_power = fabs(err_y);
        // Move in vertical
        if (target.y() > l_center.y) { move_up(estimated_power); }
        else { move_down(estimated_power); }
    }
}

void Procedure::move_right(double estimated_power) {
    if (auto sol = m_sol.lock()) {
        sol->move({static_cast<int>(estimated_power), 0});
    }
}

void Procedure::move_left(double estimated_power) {
    if (auto sol = m_sol.lock()) {
        sol->move({-static_cast<int>(estimated_power), 0});
    }
}

void Procedure::move_up(double estimated_power) {
    if (auto sol = m_sol.lock()) {
        sol->move({0, static_cast<int>(estimated_power)});
    }
}

void Procedure::move_down(double estimated_power) {
    if (auto sol = m_sol.lock()) {
        sol->move({0, -static_cast<int>(estimated_power)});
    }
}
