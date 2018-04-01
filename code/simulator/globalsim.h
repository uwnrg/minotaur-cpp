#ifndef MINOTAUR_CPP_GLOBAL_SIM_H
#define MINOTAUR_CPP_GLOBAL_SIM_H

#include <QObject>

#include "../utility/vector.h"

class GlobalSim : public QObject {
Q_OBJECT

public:
    enum Robot {
        WIDTH = 16,
        DELTA_PREF_MAX = 3,
        DELTA_PREF_MIN = 2,
        DELTA_ERR = 1,
        RESOLVE_PREF = 2,
        RESOLVE_ERR = 1
    };

    GlobalSim();

    Q_SLOT void robot_reset();
    Q_SLOT void robot_right();
    Q_SLOT void robot_left();
    Q_SLOT void robot_down();
    Q_SLOT void robot_up();

    vector2d &robot();
    vector2d &object();

private:
    double get_di() const;
    double get_dk() const;

    void check_collide();

    vector2d m_robot;
    vector2d m_object;
};

#endif

