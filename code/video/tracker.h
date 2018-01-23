#ifndef MINOTAUR_CPP_TRACKER_H
#define MINOTAUR_CPP_TRACKER_H

#include "modify.h"

//#include <opencv2/tracking.hpp>

class TrackerModifier : public VideoModifier {
public:
    TrackerModifier();

    ~TrackerModifier();

    void modify(cv::Mat *img) override;

private:
    //cv::Ptr<cv::Tracker> m_tracker;

    cv::Rect2d m_bounding_box;

    bool m_need_init = true;
};

#endif //MINOTAUR_CPP_TRACKER_H
