#include "objectprocedure.h"
#include "compstate.h"
#include "../utility/algorithm.h"
#include "../gui/mainwindow.h"

#include <QTimerEvent>

#ifndef NDEBUG
#include <cassert>
#endif

#define DELTA_THRES 2.0

ObjectProcedure::move_node
ObjectProcedure::delta_to_move_node(double base, double target, bool ver) {
#ifndef NDEBUG
    assert(fabs(target - base) >= DELTA_THRES);
#endif
    move_node node{};
    node.base = base;
    node.target = target;
    node.dir = target > base ? nrg::dir::RIGHT : nrg::dir::LEFT;
    node.dir = ver ? rotate_cw(node.dir) : node.dir;
    return node;
}

std::vector<ObjectProcedure::move_node>
ObjectProcedure::path_to_move_nodes(const path2d &path) {
    // Rectangular movement only, and prefer horizontal first
    std::vector<move_node> move_nodes; // at most two per point
    std::size_t max = path.size() - 1;
    for (std::size_t i = 0; i < max; ++i) {
        const vector2d &prev = path[i];
        const vector2d &next = path[i + 1];
        if (fabs(next.x() - prev.x()) >= DELTA_THRES) {
            move_nodes.push_back(delta_to_move_node(prev.x(), next.x(), false));
        }
        if (fabs(next.y() - prev.y()) >= DELTA_THRES) {
            move_nodes.push_back(delta_to_move_node(prev.y(), next.y(), true));
        }
    }
    return move_nodes; // move constructor
}

ObjectProcedure::ObjectProcedure(std::weak_ptr<Controller> sol, const path2d &path) :
    m_sol(std::move(sol)),
    m_path(path),
    m_done(false),
    m_start(false),
    m_index(0) {}

void ObjectProcedure::start() {
    m_timer.start(50, this);
}

void ObjectProcedure::stop() {
    m_timer.stop();
}

void ObjectProcedure::timerEvent(QTimerEvent *ev) {
    if (ev->timerId() == m_timer.timerId()) {
        movement_loop();
    }
}

bool ObjectProcedure::is_done() {
    return m_done;
}

void ObjectProcedure::movement_loop() {
    CompetitionState &state = Main::get()->state();
    if (!state.is_robot_box_fresh() || !state.is_robot_box_valid()) {
        return;
    }
    // Initialization
    if (!m_start) {
        m_start = true;
        path2d path;
        vector2d robot_loc = rect2d(state.get_robot_box(true)).center();
        path.push_back(robot_loc);
        path.insert(path.end(), m_path.begin(), m_path.end());
        m_path = std::move(path);
        m_move_nodes = path_to_move_nodes(m_path);
    }
    // If there is no active object movement
    if (!m_object_line) {
        // If we have exhausted the move list, we are done
        if (m_index == m_move_nodes.size()) {
            m_timer.stop();
            m_done = true;
            return;
        }
        // Grab the next move node
        const move_node &next = m_move_nodes[m_index];
        m_object_line = std::make_unique<ObjectLine>(m_sol, next.dir, next.target, next.base);
        // Start moving along this line
        m_object_line->start();
    } else if (m_object_line->is_done()) {
        // Completed traversing line so increment the index and invalidate
        m_object_line.reset();
        ++m_index;
    }
}
