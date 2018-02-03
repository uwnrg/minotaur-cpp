#include <opencv2/opencv.hpp>

#include "tracker.h"
#include "../uwnrg.h"

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
            WHEN_DEBUG(qDebug() << "Using GOTURN tracker");
            m_tracker = cv::TrackerGOTURN::create();
            break;
        default:
            break;
    }
}

TrackerModifier::~TrackerModifier() = default;

void TrackerModifier::forwardKeyEvent(int key) {
    WHEN_DEBUG(qDebug() << "Key event received");
    if (key == Qt::Key_A) {
        WHEN_DEBUG(qDebug() << "Switching to First Scan");
        m_state = State::FIRST_SCAN;
    } else if (key == Qt::Key_S) {
        WHEN_DEBUG(qDebug() << "Resetting tracker");
        m_state = State::UNINITIALIZED;
        m_bounding_box = {};
    }
}

void TrackerModifier::modify(cv::Mat &img) {
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
    cv::rectangle(img, m_bounding_box, cv::Scalar(255, 0, 0));
}
