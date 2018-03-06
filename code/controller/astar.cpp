#include "astar.h"
#include <cmath>
#include <set>
#include <queue>
#include <iostream>

using namespace std;

Astar::Astar() {
    grid a = makeTerrain();
    Vector2i start = {4, 0};
    Vector2i end = {0, 4};

    searchPath(a, start, end);

    for (Vector2i c: path) {
        cout << c.x_comp << " " << c.y_comp << endl;
    }
}

double Astar::manhattanDist(Vector2i cur, Vector2i dest) {
    return abs(dest.x_comp - cur.x_comp) + abs(dest.y_comp - cur.y_comp);
}

bool Astar::isValid(int x, int y, int maxRow, int maxCol) {
    return (x >= 0) && (x < maxRow) && (y >= 0) && (y < maxCol);
}

vector<Vector2i> Astar::getNeighbours(Vector2i c, int maxRow, int maxCol) {
    vector<Vector2i> neighbours;
    int cur_x = c.x_comp;
    int cur_y = c.y_comp;
    if (isValid(cur_x-1, cur_y, maxRow, maxCol)) {
        neighbours.push_back(Vector2i(cur_x-1, cur_y));
    }
    if (isValid(cur_x+1, cur_y, maxRow, maxCol)) {
        neighbours.push_back(Vector2i(cur_x+1, cur_y));
    }
    if (isValid(cur_x, cur_y-1, maxRow, maxCol)) {
        neighbours.push_back(Vector2i(cur_x, cur_y-1));
    }
    if (isValid(cur_x, cur_y+1, maxRow, maxCol)) {
        neighbours.push_back(Vector2i(cur_x, cur_y+1));
    }

    return neighbours;
}

Astar::grid Astar::makeTerrain() {
    grid a = {{1,   1, 100, 1},
              {1,   1, 100, 1},
              {100, 1, 1,   1},
              {1,   1, 100, 1}};
    return a;
}

void Astar::backtrack(Vector2i start, Vector2i dest, map<Vector2i, Vector2i> parent) {
    Vector2i cur = dest;
    while (cur != start) {
        path.push_back(cur);
        cur = parent[cur];
    }
    path.push_back(start);
}

void Astar::searchPath(Astar::grid terrain, Vector2i start, Vector2i dest) {
    map<Vector2i, Vector2i> parent;
    map<Vector2i, double> cost;
    set< pair<double, Vector2i> > frontier;

    frontier.insert(make_pair(0, start));
    parent[start] = start;
    cost[start] = 0;
    int maxRow = terrain.size();
    int maxCol = terrain[0].size();

    while(!frontier.empty()) {
        associatedCost a = *frontier.begin();
        Vector2i cur = a.second;

        if (cur == dest) break;

        for (Vector2i next: getNeighbours(cur, maxRow, maxCol)) {
            double new_cost = cost[cur] + terrain[next.x_comp][next.y_comp];
            if (cost.find(next) == cost.end() // We have not encountered this location
                    || new_cost < cost[next]) { // New cost is less than previous costs
                cost[next] = new_cost;
                double priority = new_cost + manhattanDist(next, dest);
                frontier.insert(make_pair(priority, next));
                parent[next] = cur;
            }
        }
    }

    backtrack(start, dest, parent);
}

