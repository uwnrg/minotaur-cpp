#ifndef MINOTAUR_CPP_TRACKER_H
#define MINOTAUR_CPP_TRACKER_H

#include "modify.h"

#include <opencv2/tracking.hpp>

class QVBoxLayout;
class QPushButton;

class TrackerModifier : public VideoModifier {
public:
    TrackerModifier();

    ~TrackerModifier();

    void modify(cv::UMat &img) override;

    void forwardKeyEvent(int key) override;

    void register_actions(ActionBox *box) override;

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

    std::unique_ptr<ButtonAction> m_select_roi_btn;
    std::unique_ptr<ButtonAction> m_clear_roi_btn;

    int m_type;
    int m_state;
};

#endif //MINOTAUR_CPP_TRACKER_H
