#include "tracker.h"

#include <opencv2/core/ocl.hpp>
#include <opencv/cv.hpp>

TrackerModifier::TrackerModifier() {
    //m_tracker = cv::TrackerBoosting::create();
}

TrackerModifier::~TrackerModifier() {
    //m_tracker.release();
}

void TrackerModifier::modify(cv::Mat *img) {
    /*if (m_need_init) {
        m_bounding_box = {287, 23, 86, 320};
        m_bounding_box = cv::selectROI(*img, false);
        cv::rectangle(*img, m_bounding_box, cv::Scalar(255, 0, 0), 2, 1);
        m_tracker->init(*img, m_bounding_box);
        m_need_init = false;
        return;
    }
    bool track_ok = m_tracker->update(*img, m_bounding_box);
    if (track_ok) {
        cv::rectangle(*img, m_bounding_box, cv::Scalar(255, 0, 0), 2, 1);
    }*/
}
