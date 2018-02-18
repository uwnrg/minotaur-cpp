#ifndef MINOTAUR_CPP_TRACKER_H
#define MINOTAUR_CPP_TRACKER_H

#include "modify.h"

#include <opencv2/tracking.hpp>

class TrackerModifier : public VideoModifier {
public:
    TrackerModifier();

    ~TrackerModifier();

    void modify(cv::Mat &img) override;

    void forwardKeyEvent(int key) override;

private:
    void reset_tracker();

private:
    enum Type {
        BOOSTING,
        MIL,
        KCF,
        TLD,
        MEDIAN_FLOW,
        GOTURN
    };

    enum State {
        UNINITIALIZED,
        FIRST_SCAN,
        TRACKING
    };

    cv::Ptr<cv::Tracker> m_tracker;
    cv::Rect2d m_bounding_box;

    int m_type;
    int m_state;
};

#endif //MINOTAUR_CPP_TRACKER_H
