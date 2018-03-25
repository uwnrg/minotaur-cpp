#include "compstate.h"
#include "../gui/mainwindow.h"

#define DEFAULT_CALIBRATED_AREA     400.0
#define DEFAULT_ACQUISITION_R_SIGMA 1.34

/**
 * Based off formula in
 * https://users.cs.cf.ac.uk/Paul.Rosin/resources/papers/squareness-JMIV-postprint.pdf
 *
 * @param rect
 * @param calibrated_area
 * @return
 */
double acquisition_r(const cv::Rect2d &rect, double calibrated_area) {
    double area = rect.width * rect.height;
    if (!area) { return 1000; }
    double t = rect.width > rect.height
               ? rect.width / rect.height
               : rect.height / rect.width;
    if (t <= 0.99) { return 1000; }
    return fabs(area - calibrated_area) / (area > calibrated_area ? area : calibrated_area) * t;
}

QString center_text(const cv::Rect2d &rect) {
    QString text;
    text.sprintf("Robot: (%6.1f , %6.1f )", rect.x + rect.width / 2, rect.y + rect.height / 2);
    return text;
}

CompetitionState::CompetitionState(MainWindow *parent) :
    m_parent(parent),
    m_tracking_robot(false),
    m_tracking_object(false),
    m_acquire_walls(false),
    m_object_type(UNACQUIRED),
    m_robot_calibrated_area(DEFAULT_CALIBRATED_AREA),
    m_object_calibrated_area(DEFAULT_CALIBRATED_AREA),
    m_acquisition_r_sigma(DEFAULT_ACQUISITION_R_SIGMA) {
    if (auto lp = parent->status_box().lock()) {
        m_robot_loc_label = lp->add_label(center_text(cv::Rect2d()));
    }
}

void CompetitionState::acquire_robot_box(const cv::Rect2d &robot_box) {
#ifndef NDEBUG
    assert(m_robot_loc_label != nullptr);
#endif
    m_robot_loc_label->setText(center_text(robot_box));
    m_robot_box = robot_box;
    m_robot_box_fresh = true;
}

void CompetitionState::acquire_object_box(const cv::Rect2d &object_box) {
    m_object_box = object_box;
    m_object_box_fresh = true;
}

void CompetitionState::acquire_target_box(const cv::Rect2d &target_box) {
    m_target_box = target_box;
}

void CompetitionState::acquire_walls(std::shared_ptr<wall_arr> &walls) {
    m_walls = walls;
}

void CompetitionState::set_robot_calibrated_area(double robot_calibrated_area) {
    m_robot_calibrated_area = robot_calibrated_area;
}

void CompetitionState::set_object_calibrated_area(double object_calibrated_area) {
    m_object_calibrated_area = object_calibrated_area;
}

bool CompetitionState::is_tracking_robot() const {
    return m_tracking_object;
}

bool CompetitionState::is_tracking_object() const {
    return m_tracking_object;
}

int CompetitionState::object_type() const {
    return m_object_type;
}

void CompetitionState::set_tracking_robot(bool tracking_robot) {
    m_tracking_robot = tracking_robot;
}

void CompetitionState::set_tracking_object(bool tracking_object) {
    m_tracking_object = tracking_object;
}

void CompetitionState::set_object_type(int object_type) {
    m_object_type = object_type;
}

cv::Rect2d &CompetitionState::get_robot_box(bool consume) {
    m_robot_box_fresh = m_robot_box_fresh && !consume;
    return m_robot_box;
}

cv::Rect2d &CompetitionState::get_object_box(bool consume) {
    m_object_box_fresh = m_object_box_fresh && !consume;
    return m_object_box;
}

cv::Rect2d &CompetitionState::get_target_box() {
    return m_target_box;
}

bool CompetitionState::is_robot_box_fresh() const {
    return m_robot_box_fresh;
}

bool CompetitionState::is_object_box_fresh() const {
    return m_object_box_fresh;
}

bool CompetitionState::is_robot_box_valid() const {
    return acquisition_r(m_robot_box, m_robot_calibrated_area) < m_acquisition_r_sigma;
}

bool CompetitionState::is_object_box_valid() const {
    return acquisition_r(m_object_box, m_object_calibrated_area) < m_acquisition_r_sigma;
}
