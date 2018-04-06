#ifndef MINOTAUR_CPP_COMPSTATE_H
#define MINOTAUR_CPP_COMPSTATE_H

#include <memory>

#include <opencv2/core/types.hpp>

#include <QObject>

#include "../utility/array2d.h"
#include "../utility/vector.h"
#include "../camera/statusbox.h"

#include "procedure.h"
#include "objectprocedure.h"

class MainWindow;

/**
 * Determine the likelihood that a bounding box actually contains the
 * object or robot that is tracked, based on the squareness of the rectangle
 * and its closeness to the calibrated area.
 *
 * @param rect            bounding box rectangle
 * @param calibrated_area the expected area of the object or robot
 * @return a value representing accuracy
 */
double acquisition_r(const cv::Rect2d &rect, double calibrated_area);

/**
 * This competition state object is held global and is used as the middleman
 * for communication between various object instances that contribute to
 * running the robot and object at competition time.
 *
 * The global instance is held in the MainWindow.
 */
class CompetitionState : public QObject {
Q_OBJECT

public:
    typedef bool wall_t;
    typedef array2d<wall_t, int> wall_arr;

    enum ObjectType {
        CIRCLE,
        TRIANGLE,
        SQUARE,
        UNACQUIRED
    };

    enum {
        wall_is_bool = std::is_same<wall_t, bool>::value,
        wall_is_int = std::is_same<wall_t, int>::value
    };

    enum {
        wall_x = 60,
        wall_y = 30
    };

    explicit CompetitionState(MainWindow *parent);

    Q_SIGNAL void request_robot_box();
    Q_SIGNAL void request_object_box();

    Q_SLOT void acquire_robot_box(const cv::Rect2d &robot_box);
    Q_SLOT void acquire_object_box(const cv::Rect2d &object_box);
    Q_SLOT void acquire_target_box(const cv::Rect2d &target_box);
    Q_SLOT void acquire_walls(std::shared_ptr<wall_arr> &walls);

    Q_SLOT void set_robot_calibrated_area(double robot_calibrated_area);
    Q_SLOT void set_object_calibrated_area(double object_calibrated_area);

    Q_SLOT void clear_path();
    Q_SLOT void append_path(double x, double y);

    Q_SLOT void begin_traversal();
    Q_SLOT void halt_traversal();

    Q_SLOT void begin_object_move();
    Q_SLOT void halt_object_move();

    const path2d &get_path() const;

    cv::Rect2d &get_robot_box(bool consume = false);
    cv::Rect2d &get_object_box(bool consume = false);
    cv::Rect2d &get_target_box();

    bool is_tracking_robot() const;
    void set_tracking_robot(bool tracking_robot);

    bool is_tracking_object() const;
    void set_tracking_object(bool tracking_object);

    int object_type() const;
    void set_object_type(int object_type);

    bool is_robot_box_fresh() const;
    bool is_object_box_fresh() const;

    bool is_robot_box_valid() const;
    bool is_object_box_valid() const;

private:
    MainWindow *m_parent;

    // Status labels to display the robot and object positions
    // based on rectangles posted to the object
    StatusLabel *m_robot_loc_label;
    StatusLabel *m_object_loc_label;

    bool m_tracking_robot;
    bool m_tracking_object;
    bool m_acquire_walls;

    // Whether a received bounding box for the robot and object
    // from the trackers has been consumed by a procedure
    bool m_robot_box_fresh;
    bool m_object_box_fresh;

    // Contained rectangles for the robot and object boxes
    cv::Rect2d m_robot_box;
    cv::Rect2d m_object_box;
    cv::Rect2d m_target_box;

    std::shared_ptr<wall_arr> m_walls;

    int m_object_type;

    // Stored calibrated area values for the object and robot
    double m_robot_calibrated_area;
    double m_object_calibrated_area;

    /**
     * The accuracy threshold to determine whether the robot or
     * object bounding box is accurate
     */
    double m_acquisition_r_sigma;

    /**
     * The desired robot or object traversal path. Object that seek
     * to update this should modify it directly through access functions.
     */
    path2d m_path;

    /**
     * Stored procedure instance for traversing the path.
     */
    std::unique_ptr<Procedure> m_procedure;
    /**
     * Stored object procedure instance for moving the object along the path.
     */
    std::unique_ptr<ObjectProcedure> m_object_procedure;
};

Q_DECLARE_METATYPE(std::shared_ptr<CompetitionState::wall_arr>);

#endif //MINOTAUR_CPP_COMPSTATE_H
