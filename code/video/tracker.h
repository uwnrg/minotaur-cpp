#ifndef MINOTAUR_CPP_TRACKER_H
#define MINOTAUR_CPP_TRACKER_H
#ifndef TRACKER_OFF

#include "modify.h"

#include <opencv2/tracking.hpp>

class QVBoxLayout;
class QPushButton;

class TrackerModifier : public VideoModifier {
public:
    TrackerModifier();

    void modify(cv::UMat &img) override;

    void forwardKeyEvent(int key) override;

    void register_actions(const std::vector<ActionButton *> &action_btns, ActionBox *box) override;

    int num_buttons() const;

protected:
    Q_SLOT void beginTracking();

    Q_SLOT void stopTracking();

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

#endif
#endif //MINOTAUR_CPP_TRACKER_H
