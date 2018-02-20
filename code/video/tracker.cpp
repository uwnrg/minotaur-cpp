#include <opencv2/opencv.hpp>

#include "tracker.h"
#ifndef NDEBUG
#include <QDebug>
#endif

// CMake will try to find goturn.caffemodel and goturn.prototxt, which need
// to be added separtaely. If these are found, the GOTURN tracker model
// will be used instead of the MIL tracker.
#ifdef GOTURN_FOUND
#define TRACKER_TYPE Type::GOTURN
#else
#define TRACKER_TYPE Type::MIL
#endif

TrackerModifier::TrackerModifier()
    : m_bounding_box(287, 23, 100, 100),
      m_type(TRACKER_TYPE),
      m_state(State::UNINITIALIZED) {
    reset_tracker();
}

TrackerModifier::~TrackerModifier() = default;

void TrackerModifier::reset_tracker() {
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
}

void TrackerModifier::forwardKeyEvent(int key) {
#ifndef NDEBUG
    qDebug() << "Key event received";
#endif
    if (key == Qt::Key_A) {
#ifndef NDEBUG
        qDebug() << "Switching to First Scan";
#endif
        m_state = State::FIRST_SCAN;
    } else if (key == Qt::Key_S) {
#ifndef NDEBUG
        qDebug() << "Resetting tracker";
#endif
        reset_tracker();
        m_state = State::UNINITIALIZED;
        m_bounding_box = {};
    }
}

void TrackerModifier::modify(cv::UMat &img) {
    if (m_state == State::UNINITIALIZED) {
        return;
    }
    if (m_state == State::TRACKING) {
        m_tracker->update(img, m_bounding_box);
    } else if (m_state == State::FIRST_SCAN) {
        m_bounding_box = cv::selectROI(img);
        m_tracker->init(img, m_bounding_box);
        m_state = State::TRACKING;
    }
    cv::rectangle(img, m_bounding_box.br(), m_bounding_box.tl(), cv::Scalar(255, 0, 0));
}
