#ifndef MINOTAUR_CPP_OBJECTSTRATEGY_H
#define MINOTAUR_CPP_OBJECTSTRATEGY_H

#include "../controller/controller.h"
#include "../utility/vector.h"
#include "../utility/rect.h"

#include <QObject>
#include <QTimer>

class ObjectStrategy : public QObject {
Q_OBJECT

public:
    ObjectStrategy(std::weak_ptr<Controller> sol, const vector2d &target);

    void start();

    void stop();

private:
    void timerEvent(QTimerEvent *ev) override;

    void movement_loop();

private:
    vector2d m_target;
    std::weak_ptr<Controller> m_sol;
    QBasicTimer m_timer;
};

#endif //MINOTAUR_CPP_OBJECTSTRATEGY_H
