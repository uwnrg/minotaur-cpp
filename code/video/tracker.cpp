#ifndef TRACKER_OFF

#include <opencv2/opencv.hpp>
#include <QDialog>
#include <QPushButton>
#include <QVBoxLayout>

#include "tracker.h"
#include "../utility/utility.h"

#ifndef NDEBUG

#include <QDebug>

#endif

// CMake will try to find goturn.caffemodel and goturn.prototxt, which need
// to be added separately. If these are found, the GOTURN tracker model
// will be used instead of the MIL tracker.
#ifdef GOTURN_FOUND
#define TRACKER_TYPE Type::GOTURN
#else
#define TRACKER_TYPE Type::MIL
#endif

TrackerModifier::TrackerModifier()
    : m_bounding_box(),
      m_type(TRACKER_TYPE),
      m_state(State::UNINITIALIZED) {
    reset_tracker();
}

void TrackerModifier::reset_tracker() {
    m_mutex.lock();
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

void TrackerModifier::beginTracking() {
    if (m_state == State::UNINITIALIZED) {
        m_state = State::FIRST_SCAN;
    }
}

void TrackerModifier::stopTracking() {
    if (m_state != State::UNINITIALIZED) {
        reset_tracker();
        m_state = State::UNINITIALIZED;
        m_bounding_box = {};
    }
}

void TrackerModifier::modify(cv::UMat &img) {
    if (m_state != State::UNINITIALIZED) {
        m_mutex.lock();
        if (m_state == State::TRACKING) {
            m_tracker->update(img, m_bounding_box);
        } else if (m_state == State::FIRST_SCAN) {
            m_bounding_box = cv::selectROI(img);
            m_tracker->init(img, m_bounding_box);
            m_state = State::TRACKING;
        }
        m_mutex.unlock();
        cv::rectangle(img, m_bounding_box.br(), m_bounding_box.tl(), cv::Scalar(255, 0, 0));
    }
}

void TrackerModifier::register_actions(ActionBox *box) {
    ActionButton *start_button = box->add_action("Select ROI");
    ActionButton *clear_button = box->add_action("Clear ROI");
    connect(start_button, &ActionButton::clicked, this, &TrackerModifier::beginTracking);
    connect(clear_button, &ActionButton::clicked, this, &TrackerModifier::stopTracking);
    box->set_actions();
}

#endif