#include "compstate.h"

CompetitionState::CompetitionState()
    : m_tracking_robot(false),
      m_tracking_object(false),
      m_acquire_walls(false),
      m_object_type(UNACQUIRED) {}

void CompetitionState::acquire_robot_box(std::shared_ptr<cv::Rect2d> &robot_box) {
    m_robot_box = robot_box;
}

void CompetitionState::acquire_object_box(std::shared_ptr<cv::Rect2d> &object_box) {
    m_object_box = object_box;
}

void CompetitionState::acquire_walls(std::shared_ptr<wall_arr> &walls) {
    m_walls = walls;
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
