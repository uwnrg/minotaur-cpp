#ifndef MINOTAUR_CPP_OBJECTPROCEDURE_H
#define MINOTAUR_CPP_OBJECTPROCEDURE_H

#include "objectline.h"

#include <QBasicTimer>
#include <QObject>

class StatusLabel;

/**
 * This procedure combines ObjectLine procedures to move the object
 * in a series of rectangular paths.
 */
class ObjectProcedure : public QObject {
Q_OBJECT

public:
    struct move_node {
        double base;
        double target;
        nrg::dir dir;
    };

    explicit ObjectProcedure(std::weak_ptr<Controller> sol, const path2d &path);
    ~ObjectProcedure() override;

    void start();
    void stop();

    bool is_done();

private:
    static std::vector<move_node> path_to_move_nodes(const path2d &path);
    static move_node delta_to_move_node(double start, double delta, double base, bool ver);

    void timerEvent(QTimerEvent *ev) override;

    void movement_loop();

    std::weak_ptr<Controller> m_sol;
    std::vector<move_node> m_move_nodes;

    path2d m_path;
    vector2d m_initial;

    QBasicTimer m_timer;

    StatusLabel *m_index_label;

    bool m_done;
    bool m_start;
    std::size_t m_index;

    std::unique_ptr<ObjectLine> m_object_line;
};

#endif //MINOTAUR_CPP_OBJECTPROCEDURE_H
