#include "astar.h"
#include <cmath>
#include <set>
#include <queue>
#include <iostream>
#include <algorithm>
#include <iterator>

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
    // Idea for using maps to keep track of path information came from redblobgames.com
    map<Astar::Coord, Astar::Coord> parent;
    map<Astar::Coord, double> cost;
    set<associated_cost> open_set;

    // initialize base cases
    open_set.insert(make_pair(0, start));
    parent[start] = start;
    cost[start] = 0;

    auto max_row = static_cast<int>(terrain.x());
    auto max_col = static_cast<int>(terrain.y());

    while (!open_set.empty()) {
        // element returned by begin() C++ set should be lowest
        Coord cur = (*open_set.begin()).second;
        // remove from open list
        open_set.erase(open_set.begin());

        if (cur == dest) { break; }

        for (Coord next: get_neighbors(cur, max_row, max_col)) {
            // check for obstacles
            if (terrain[next.x][next.y] != -1) {
                double new_cost = cost[cur] + terrain[next.x][next.y];
                // We have not encountered this location or new cost is less than previous costs
                if (!cost.count(next) || new_cost < cost[next]) {
                    // update cost
                    cost[next] = new_cost;
                    // move into openset with estimated cost
                    open_set.insert(make_pair(new_cost + manhattan_dist(next, dest), next));
                    // update parent
                    parent[next] = cur;
                }
            }
        }
    }

    backtrack(start, dest, parent);
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