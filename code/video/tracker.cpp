#ifndef TRACKER_OFF

#include <opencv2/opencv.hpp>
#include <QDialog>
#include <QPushButton>

#include "tracker.h"
#include "../gui/global.h"
#include "../compstate/compstate.h"

#ifndef NDEBUG

#include <QDebug>

#endif

// CMake will try to find goturn.caffemodel and goturn.prototxt, which need
// to be added separately. If these are found, the GOTURN tracker model
// will be used instead of the MIL tracker.
#ifdef GOTURN_FOUND
#define TRACKER_TYPE Type::GOTURN
#else
#define TRACKER_TYPE Type::KCF
#endif

__tracker::__tracker() :
    m_bounding_box(),
    m_type(TRACKER_TYPE),
    m_state(State::UNINITIALIZED) {
    reset_tracker();
}

void __tracker::reset_tracker() {
    m_mutex.lock();
    // At this point MIL seems to be the best performing tracker
    // Accuracy is more important than performance so long as
    // framerate remains above at least 12
    switch (m_type) {
        case Type::BOOSTING:
            m_tracker = cv::TrackerBoosting::create();
            break;
        case Type::MIL:
            m_tracker = cv::TrackerMIL::create();
            break;
        case Type::KCF:
            m_tracker = cv::TrackerKCF::create();
            break;
        case Type::TLD:
            m_tracker = cv::TrackerTLD::create();
            break;
        case Type::MEDIAN_FLOW:
            m_tracker = cv::TrackerMedianFlow::create();
            break;
        case Type::GOTURN:
#ifndef NDEBUG
            qDebug() << "Using GOTURN tracker";
#endif
            m_tracker = cv::TrackerGOTURN::create();
            break;
        default:
            break;
    }
    m_mutex.unlock();
}

void __tracker::begin_tracking() {
    if (m_state == State::UNINITIALIZED) {
        m_state = State::FIRST_SCAN;
    }
}

void __tracker::stop_tracking() {
    if (m_state != State::UNINITIALIZED) {
        reset_tracker();
        m_state = State::UNINITIALIZED;
        m_bounding_box = {};
    }
}

void __tracker::update_track(cv::UMat &img) {
    if (m_state == State::FAILED) {
        reset_tracker();
        if (m_tracker->init(img, m_bounding_box)) {
            m_state = State::TRACKING;
        }
        return;
    }
    if (m_state != State::UNINITIALIZED) {
        m_mutex.lock();
        if (m_state == State::TRACKING) {
            if (!m_tracker->update(img, m_bounding_box)) {
                m_state = State::FAILED;
            }
        } else if (m_state == State::FIRST_SCAN) {
            m_bounding_box = cv::selectROI(img);
            if (m_tracker->init(img, m_bounding_box)) {
                m_state = State::TRACKING;
            } else {
                m_state = State::FAILED;
            }
        }
        m_mutex.unlock();
        Q_EMIT target_box(m_bounding_box);
    }
}

void __tracker::draw_bounding_box(cv::UMat &img) {
    if (m_state == State::TRACKING) {
        cv::rectangle(img, m_bounding_box.tl(), m_bounding_box.br(), cv::Scalar(255, 0, 0));
    }
}

__tracker::State __tracker::state() const {
    return m_state;
}

TrackerModifier::TrackerModifier() :
    m_robot_tracker(),
    m_object_tracker() {
    CompetitionState *state = &Main::get()->state();
    connect(&m_robot_tracker, &__tracker::target_box, state, &CompetitionState::acquire_robot_box);
    connect(&m_object_tracker, &__tracker::target_box, state, &CompetitionState::acquire_object_box);
}

void TrackerModifier::traverse() {
    if (m_robot_tracker.state() == __tracker::TRACKING) {
        Main::get()->state().begin_traversal();
    }
}

void TrackerModifier::move_object() {
    if (
        m_robot_tracker.state() == __tracker::TRACKING &&
        m_object_tracker.state() == __tracker::TRACKING
    ) {
        Main::get()->state().begin_object_move();
    }
}

void TrackerModifier::register_actions(ActionBox *box) {
    ActionButton *traverse_button = box->add_action("Traverse");
    ActionButton *object_move_button = box->add_action("Move Object");
    ActionButton *select_robot_roi = box->add_action("Select Robot ROI");
    ActionButton *select_object_roi = box->add_action("Select Object ROI");
    ActionButton *clear_robot_roi = box->add_action("Clear Robot ROI");
    ActionButton *clear_object_roi = box->add_action("Clear Object ROI");
    ActionButton *stop_button = box->add_action("Stop Object")
    connect(traverse_button, &QPushButton::clicked, this, &TrackerModifier::traverse);
    connect(object_move_button, &QPushButton::clicked, this, &TrackerModifier::move_object);
    connect(select_robot_roi, &QPushButton::clicked, &m_robot_tracker, &__tracker::begin_tracking);
    connect(select_object_roi, &QPushButton::clicked, &m_object_tracker, &__tracker::begin_tracking);
    connect(clear_robot_roi, &QPushButton::clicked, &m_robot_tracker, &__tracker::stop_tracking);
    connect(clear_object_roi, &QPushButton::clicked, &m_object_tracker, &__tracker::stop_tracking);
    box->set_actions();
}

void TrackerModifier::modify(cv::UMat &img) {
    m_robot_tracker.update_track(img);
    m_object_tracker.update_track(img);
    m_robot_tracker.draw_bounding_box(img);
    m_object_tracker.draw_bounding_box(img);
}

#endif