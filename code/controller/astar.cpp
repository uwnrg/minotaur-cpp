#include "astar.h"

#include "../utility/vector.h"

#include <cmath>
#include <set>
#include <queue>
#include <iostream>
#include <algorithm>
#include <iterator>

struct associated_cost {
    associated_cost(double t_cost, const vector2i &t_coord) :
        cost(t_cost),
        coord(t_coord) {}

    double cost;
    vector2i coord;

    bool operator==(const associated_cost &o) const {
        return cost == o.cost;
    }

    bool operator!=(const associated_cost &o) const {
        return cost != o.cost;
    }

    bool operator<(const associated_cost &o) const {
        return cost < o.cost;
    }
};

struct vector_less {
    bool operator()(const vector2i &a, const vector2i &b) const {
        return std::tie(a.x(), a.y()) < std::tie(b.x(), b.y());
    }
};

static double manhattan_dist(
    const vector2i &cur,
    const vector2i &dest
) {
    return abs(dest.x() - cur.x()) + abs(dest.y() - cur.y());
}

static bool is_valid(
    int x, int y,
    int max_row, int max_col
) {
    return (x >= 0) && (x < max_row) && (y >= 0) && (y < max_col);
}

static void get_neighbors(
    const vector2i &c,
    int max_row, int max_col,
    std::vector<vector2i> &neighbors
) {
    int cur_x = c.x();
    int cur_y = c.y();
    if (is_valid(cur_x - 1, cur_y, max_row, max_col)) {
        neighbors.emplace_back(cur_x - 1, cur_y);
    }
    if (is_valid(cur_x + 1, cur_y, max_row, max_col)) {
        neighbors.emplace_back(cur_x + 1, cur_y);
    }
    if (is_valid(cur_x, cur_y - 1, max_row, max_col)) {
        neighbors.emplace_back(cur_x, cur_y - 1);
    }
    if (is_valid(cur_x, cur_y + 1, max_row, max_col)) {
        neighbors.emplace_back(cur_x, cur_y + 1);
    }
}

static void backtrack(
    const vector2i &start,
    const vector2i &dest,
    const std::map<vector2i, vector2i, vector_less> &parent,
    std::vector<vector2i> &path
) {
    vector2i cur = dest;
    while (cur != start) {
        path.push_back(cur);
        cur = parent.at(cur);
    }
    path.push_back(start);
    std::reverse(std::begin(path), std::end(path));
}

void search_path1(
    array2d<int> &terrain,
    const vector2i &start,
    const vector2i &dest,
    std::vector<vector2i> &path
) {
    std::map<vector2i, vector2i, vector_less> parent;
    std::map<vector2i, double, vector_less> cost;
    std::set<associated_cost> open_set;

    open_set.emplace(0, start);
    parent[start] = start;
    cost[start] = 0;

    int max_row = static_cast<int>(terrain.x());
    int max_col = static_cast<int>(terrain.y());

    while (!open_set.empty()) {
        vector2i cur = open_set.begin()->coord;
        open_set.erase(open_set.begin());
        if (cur == dest) { break; }

        std::vector<vector2i> neighbors;
        get_neighbors(cur, max_row, max_col, neighbors);
        for (const vector2i &next : neighbors) {
            if (terrain[next.x()][next.y()] != -1) {
                double new_cost = cost[cur] + terrain[next.x()][next.y()];
                if (!cost.count(next) || new_cost < cost[next]) {
                    cost[next] = new_cost;
                    open_set.emplace(new_cost + manhattan_dist(next, dest), next);
                    parent[next] = cur;
                }
            }
        }
    }
    backtrack(start, dest, parent, path);
}

using namespace std;

Astar::Astar() = default;

double Astar::manhattan_dist(Astar::Coord cur, Astar::Coord dest) {
    return abs(dest.x - cur.x) + abs(dest.y - cur.y);
}

bool Astar::is_valid(int x, int y, int max_row, int max_col) {
    return (x >= 0) && (x < max_row) && (y >= 0) && (y < max_col);
}

vector<Astar::Coord> Astar::get_neighbors(Astar::Coord c, int max_row, int max_col) {
    vector<Astar::Coord> neighbours;
    int cur_x = c.x;
    int cur_y = c.y;
    if (is_valid(cur_x - 1, cur_y, max_row, max_col)) {
        neighbours.push_back(Astar::Coord{cur_x - 1, cur_y});
    }
    if (is_valid(cur_x + 1, cur_y, max_row, max_col)) {
        neighbours.push_back(Astar::Coord{cur_x + 1, cur_y});
    }
    if (is_valid(cur_x, cur_y - 1, max_row, max_col)) {
        neighbours.push_back(Astar::Coord{cur_x, cur_y - 1});
    }
    if (is_valid(cur_x, cur_y + 1, max_row, max_col)) {
        neighbours.push_back(Astar::Coord{cur_x, cur_y + 1});
    }

    return neighbours;
}

void Astar::backtrack(
    const Astar::Coord &start,
    const Astar::Coord &dest,
    const map<Astar::Coord, Astar::Coord> &parent
) {
    Astar::Coord cur = dest;
    while (cur != start) {
        m_path.push_back(cur);
        cur = parent.at(cur);
    }
    m_path.push_back(start);
    reverse(begin(m_path), end(m_path));
}

void Astar::search_path(array2d<int> &terrain, Astar::Coord start, Astar::Coord dest) {
    std::vector<vector2i> path;
    search_path1(terrain, {start.x, start.y}, {dest.x, dest.y}, path);
    for (auto it : path) {
        m_path.push_back(Astar::Coord{it.x(), it.y()});
    }
}

vector<Astar::Coord> Astar::get_path() {
    return m_path;
}

bool operator==(Astar::Coord a, Astar::Coord b) {
    return a.x == b.x && a.y == b.y;
}

bool operator!=(Astar::Coord a, Astar::Coord b) {
    return !(a == b);
}

bool operator<(Astar::Coord a, Astar::Coord b) {
    return std::tie(a.x, a.y) < std::tie(b.x, b.y);
}