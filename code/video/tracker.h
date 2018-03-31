#ifndef MINOTAUR_CPP_TRACKER_H
#define MINOTAUR_CPP_TRACKER_H
#ifndef TRACKER_OFF

#include "modify.h"
#include "../compstate/procedure.h"
#include <opencv2/tracking.hpp>
#include <QMutex>

class QVBoxLayout;
class QPushButton;

class __tracker : public QObject {
Q_OBJECT

public:
    enum State {
        UNINITIALIZED,
        FIRST_SCAN,
        TRACKING,
        FAILED
    };

    enum Type {
        BOOSTING,
        MIL,
        KCF,
        TLD,
        MEDIAN_FLOW,
        GOTURN
    };

    __tracker();

    void update_track(cv::UMat &img);

    void draw_bounding_box(cv::UMat &img);

    State state() const;

    Q_SIGNAL void target_box(const cv::Rect2d &box);

    Q_SLOT void begin_tracking();

    Q_SLOT void stop_tracking();

private:
    void reset_tracker();

private:
    cv::Ptr<cv::Tracker> m_tracker;
    cv::Rect2d m_bounding_box;

    Type m_type;
    State m_state;

    /**
     * Class mutex instance used to prevent a scenario wherein
     * the class's cv::Tracker pointer is set to null while another
     * thread tries to use it, resulting in a segmentation fault.
     *
     * Might happen when clicking "Clear ROI", because reset_tracker() and
     * modify() are called in different threads.
     */
    QMutex m_mutex;
};

class TrackerModifier : public VideoModifier {
Q_OBJECT

public:
    TrackerModifier();

    void modify(cv::UMat &img) override;

    void register_actions(ActionBox *box) override;

protected:
    Q_SLOT void traverse();

    Q_SLOT void move_object();

private:
    __tracker m_robot_tracker;
    __tracker m_object_tracker;
};

#endif
#endif //MINOTAUR_CPP_TRACKER_H
