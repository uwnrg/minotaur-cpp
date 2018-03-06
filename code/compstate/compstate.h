#ifndef MINOTAUR_CPP_COMPSTATE_H
#define MINOTAUR_CPP_COMPSTATE_H

#include <memory>

#include <opencv2/core/types.hpp>

#include <QObject>

#include "../utility/array2d.h"

Q_DECLARE_METATYPE(std::shared_ptr<cv::Rect2d>);

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

    CompetitionState();

    Q_SIGNAL void request_robot_box();
    Q_SIGNAL void request_object_box();

    Q_SLOT void acquire_robot_box(const cv::Rect2d &robot_box);
    Q_SLOT void acquire_object_box(const cv::Rect2d &object_box);
    Q_SLOT void acquire_target_box(const cv::Rect2d &target_box);
    Q_SLOT void acquire_walls(std::shared_ptr<wall_arr> &walls);

    cv::Rect2d &get_robot_box();
    cv::Rect2d &get_object_box();
    cv::Rect2d &get_target_box();
    const cv::Rect2d &get_robot_box() const;
    const cv::Rect2d &get_object_box() const;
    const cv::Rect2d &get_target_box() const;

    bool is_tracking_robot() const;
    void set_tracking_robot(bool tracking_robot);

    bool is_tracking_object() const;
    void set_tracking_object(bool tracking_object);

    int object_type() const;
    void set_object_type(int object_type);

private:
    bool m_tracking_robot;
    bool m_tracking_object;
    bool m_acquire_walls;

    cv::Rect2d m_robot_box;
    cv::Rect2d m_object_box;
    cv::Rect2d m_target_box;

    std::shared_ptr<wall_arr> m_walls;

    int m_object_type;
};

Q_DECLARE_METATYPE(std::shared_ptr<CompetitionState::wall_arr>);

#endif //MINOTAUR_CPP_COMPSTATE_H
