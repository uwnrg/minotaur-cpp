#include "compstate.h"
#include "objectline.h"
#include "objectprocedure.h"
#include "parammanager.h"

#include "../camera/statusbox.h"
#include "../camera/statuslabel.h"
#include "../gui/mainwindow.h"
#include "../gui/global.h"
#include "../utility/algorithm.h"

#include <QBasicTimer>
#include <QTimerEvent>

struct move_node {
    double base;
    double target;
    nrg::dir dir;
};

static move_node delta_to_move_node(double start, double target, double base, bool ver) {
    // Create a move node
    move_node node{};
    node.base = base;
    node.target = target;
    node.dir = target > start ? nrg::dir::RIGHT : nrg::dir::LEFT;
    node.dir = ver ? rotate_cw(node.dir) : node.dir;
    return node;
}

static std::vector<move_node>
path_to_move_nodes(const path2d &path) {
    std::vector<move_node> move_nodes;
    // Translate general plane path to rectangular paths
    for (std::size_t i = 0; i < path.size() - 1; ++i) {
        const vector2d &prev = path[i];
        const vector2d &next = path[i + 1];
        move_nodes.push_back(delta_to_move_node(prev.x(), next.x(), prev.y(), false));
        move_nodes.push_back(delta_to_move_node(prev.y(), next.y(), next.x(), true));
    }
    return move_nodes;
}

class ObjectProcedure::Impl {
public:
    Impl(const path2d &t_path);

    path2d path;
    std::size_t index;
    vector2d initial;
    std::vector<move_node> move_nodes;
    QBasicTimer timer;
};

ObjectProcedure::Impl::Impl(const path2d &t_path) :
    path(t_path),
    index(0) {}

ObjectProcedure::ObjectProcedure(std::weak_ptr<Controller> sol, const path2d &path) :
    m_impl(std::make_unique<Impl>(path)),
    m_sol(std::move(sol)),
    m_done(false),
    m_start(false) {
    if (auto lp = Main::get()->status_box().lock()) {
        m_index_label = lp->add_label("Obj Index: 0");
    }
}

ObjectProcedure::~ObjectProcedure() {
    if (auto lp = Main::get()->status_box().lock()) {
        lp->remove_label(m_index_label);
    }
}

void ObjectProcedure::start() {
    m_impl->timer.start(g_pm->timer_fast, this);
}

void ObjectProcedure::stop() {
    m_impl->timer.stop();
    if (m_object_line != nullptr) {
        m_object_line->stop();
    }
}

void ObjectProcedure::timerEvent(QTimerEvent *ev) {
    if (ev->timerId() == m_impl->timer.timerId()) {
        movement_loop();
    }
}

bool ObjectProcedure::is_done() {
    return m_done;
}

void ObjectProcedure::movement_loop() {
    m_index_label->setText("Obj Index: " + QString::number(m_impl->index));
    CompetitionState &state = Main::get()->state();
    if (!state.is_robot_box_fresh() || !state.is_robot_box_valid()) {
        return;
    }
    // Initialization
    if (!m_start) {
        m_start = true;
        path2d path;
        vector2d object_loc = rect2d(state.get_object_box(true)).center();
        path.push_back(object_loc);
        path.insert(path.end(), m_impl->path.begin(), m_impl->path.end());
        m_impl->path = std::move(path);
        m_impl->move_nodes = path_to_move_nodes(m_impl->path);
    }
    // If there is no active object movement
    if (!m_object_line) {
        // If we have exhausted the move list, we are done
        if (m_impl->index == m_impl->move_nodes.size()) {
            m_impl->timer.stop();
            m_done = true;
            return;
        }
        // Grab the next move node
        const move_node &next = m_impl->move_nodes[m_impl->index];
        m_object_line = std::make_unique<ObjectLine>(m_sol, next.dir, next.target, next.base);
        // Start moving along this line
        m_object_line->start();
    } else if (m_object_line->is_done()) {
        // Completed traversing line so increment the index and invalidate
        m_object_line.reset();
        ++m_impl->index;
    }
}
