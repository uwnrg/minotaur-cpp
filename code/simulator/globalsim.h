#ifndef MINOTAUR_CPP_GLOBAL_SIM_H
#define MINOTAUR_CPP_GLOBAL_SIM_H

#include <QObject>

#include "../utility/vector2d.h"

class GlobalSim : public QObject {
Q_OBJECT

public:
    enum Robot {
        WIDTH = 15,
        DELTA_PREF_MAX = 10,
        DELTA_PREF_MIN = 5,
        DELTA_ERR = 4
    };

    GlobalSim();

    Q_SLOT void robot_reset();
    Q_SLOT void robot_right();
    Q_SLOT void robot_left();
    Q_SLOT void robot_down();
    Q_SLOT void robot_up();

private:
    double get_di() const;
    double get_dk() const;

    vector2d<double> m_robot;
};

#endif

