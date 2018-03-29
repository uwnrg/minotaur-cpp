#ifndef MINOTAUR_CPP_GLOBAL_SIM_H
#define MINOTAUR_CPP_GLOBAL_SIM_H

#include <QObject>

#include "../utility/vector.h"

class GlobalSim : public QObject {
Q_OBJECT

public:
    enum Robot {
        WIDTH = 15,
        DELTA_PREF_MAX = 10,
        DELTA_PREF_MIN = 8,
        DELTA_ERR = 3
    };

    GlobalSim();

    Q_SLOT void robot_reset();
    Q_SLOT void robot_right();
    Q_SLOT void robot_left();
    Q_SLOT void robot_down();
    Q_SLOT void robot_up();

    vector2d &robot();

private:
    double get_di() const;
    double get_dk() const;

    vector2d m_robot;
};

#endif

