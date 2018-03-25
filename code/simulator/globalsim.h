#ifndef MINOTAUR_CPP_GLOBAL_SIM_H
#define MINOTAUR_CPP_GLOBAL_SIM_H

#include <QObject>

#include "../utility/vector2d.h"

class GlobalSim : public QObject {
Q_OBJECT

public:
    GlobalSim();

    Q_SLOT void robot_reset();
    Q_SLOT void robot_right();
    Q_SLOT void robot_left();
    Q_SLOT void robot_down();
    Q_SLOT void robot_up();

private:
    vector2d<double> m_robot;
};

#endif

