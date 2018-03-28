#ifndef MINOTAUR_CPP_COMPSTATE_H
#define MINOTAUR_CPP_COMPSTATE_H

#include <memory>

#include <opencv2/core/types.hpp>

#include <QObject>

#include "../utility/array2d.h"
#include "../utility/vector2d.h"
#include "../camera/statusbox.h"

#include "procedure.h"

class MainWindow;

double acquisition_r(const cv::Rect2d &rect, double calibrated_area);

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

    const path2d<double> &get_path() const;

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

    StatusLabel *m_robot_loc_label;
    StatusLabel *m_object_loc_label;

    bool m_tracking_robot;
    bool m_tracking_object;
    bool m_acquire_walls;

    bool m_robot_box_fresh;
    bool m_object_box_fresh;

    cv::Rect2d m_robot_box;
    cv::Rect2d m_object_box;
    cv::Rect2d m_target_box;

    std::shared_ptr<wall_arr> m_walls;

    int m_object_type;

    double m_robot_calibrated_area;
    double m_object_calibrated_area;

    double m_acquisition_r_sigma;

    path2d<double> m_path;

    std::unique_ptr<Procedure> m_procedure;
};

Q_DECLARE_METATYPE(std::shared_ptr<CompetitionState::wall_arr>);

#endif //MINOTAUR_CPP_COMPSTATE_H
