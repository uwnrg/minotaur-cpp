#ifndef MINOTAUR_CPP_TRACKER_H
#define MINOTAUR_CPP_TRACKER_H

#include "modify.h"

#include <opencv2/tracking

class TrackerModifier : public VideoModifier {
public:
    void modify(cv::Mat *img) override;

private:
    cv::Ptr<cv::Tracker> m_tracker;
};

#endif //MINOTAUR_CPP_TRACKER_H
