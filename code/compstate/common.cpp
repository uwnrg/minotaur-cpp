#include "common.h"
#include "../utility/algorithm.h"
#include "../utility/rect.h"
#include "../utility/vector.h"
#include "../utility/graph2d.h"

#ifndef NDEBUG
#include <cassert>
#endif

vector2d algo::rect_center(const cv::Rect2d &rect)  {
    return {rect.x + rect.width / 2, rect.y + rect.height / 2};
}

vector2d algo::perp_intersect(
    const vector2d &c,
    const vector2d &p0,
    const vector2d &p1
) {
    // Find slope of line and perpendicular line
    double dy1 = p1.y() - p0.y();
    double dx1 = p1.x() - p0.x();
    double m_l = dy1 / dx1;
    double m_r = -1 / m_l;
    // y - y0 = (x - x0) * m_l
    // y - yc = (x - xc) * m_r
    // y0 - yc = (x - xc) * m_r + (x0 - x) * m_l
    // y0 - yc + xc * m_r - x0 * m_l = x * (m_r - m_l)
    // Solve the x-intersect between the two lines
    double xi = (p0.y() - c.y() + c.x() * m_r - p0.x() * m_l) / (m_r - m_l);
    // y - y0 = (x - x0) * m_l
    // yi = (xi - x0) * m_l + y0
    // Compute the y-intersect using one of the equations
    double yi = (xi - p0.x()) * m_l + p0.y();
    return {xi, yi};
}


path2d algo::object_traverse_points(
    const rect2d &rob,
    const rect2d &obj
) {
    // Compute half-width and half-height
    double hw = 0.5 * rob.width();
    double hh = 0.5 * rob.height();
    // Traverse points are half-width and half-height from each
    // vertex and midpoint
    return {
        obj.tl() + vector2d(-hw, -hh),
        obj.mt() + vector2d(0, -hh),
        obj.tr() + vector2d(hw, -hh),
        obj.mr() + vector2d(hw, 0),
        obj.br() + vector2d(hw, hh),
        obj.mb() + vector2d(0, hh),
        obj.bl() + vector2d(-hw, hh),
        obj.ml() + vector2d(-hw, 0)
    }; // move constructor
}

path2d algo::robot_object_path(
    const rect2d &rob,
    const rect2d &obj,
    int side
) {
    path2d points =  algo::object_traverse_points(rob, obj);
#ifndef NDEBUG
    // There should be eight points and the bounding boxes
    // should not be colliding at this point
    assert(points.size() == 8);
    //assert(!algo::aabb_collide(rob, obj));
#endif
    nrg::graph2d<double> graph;
    std::vector<nrg::node2d<double>> nodes{
        graph.add_node(points[0]), // tl
        graph.add_node(points[1]), // mt
        graph.add_node(points[2]), // tr
        graph.add_node(points[3]), // mr
        graph.add_node(points[4]), // br
        graph.add_node(points[5]), // mb
        graph.add_node(points[6]), // bl
        graph.add_node(points[7])  // ml
    };
    // Connect the nodes around the object
    graph.connect(nodes[0], nodes[1]);
    graph.connect(nodes[1], nodes[2]);
    graph.connect(nodes[2], nodes[3]);
    graph.connect(nodes[3], nodes[4]);
    graph.connect(nodes[4], nodes[5]);
    graph.connect(nodes[5], nodes[6]);
    graph.connect(nodes[6], nodes[7]);
    graph.connect(nodes[7], nodes[0]);
    // Use swept collide to determine which nodes are
    // reachable by the robot, and there should always
    // be at least three
    nrg::node2d<double> start = graph.add_node(rob.center());
    for (std::size_t i = 0; i < 8; ++i) {
        if (!algo::swept_collide(rob, obj, points[i])) {
            graph.connect(start, nodes[i]);
        }
    }
#ifndef NDEBUG
    assert(graph.connections_of(start).size() >= 3);
#endif
    // Determine the target node based on the side
    // The side integer value should scale as
    // [0, 1, 2, 3] -> [0, 2, 4, 6] -> [1, 3, 5, 7]
    // to one of the m(t, r, b, l) points
    std::size_t target = static_cast<std::size_t>(side) * 2 + 1;
    std::vector<nrg::node2d<double>> node_path = graph.astar(start, nodes[target]);
    return nrg::graph2d<double>::to_path(node_path); // move constructor
}
