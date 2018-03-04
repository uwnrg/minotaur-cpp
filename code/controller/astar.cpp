#include "astar.h"
#include <cmath>
#include <unordered_map>
#include <map>
#include <set>
#include <queue>

using namespace std;

Astar::Astar() {

}

double Astar::manhattanDist(Coord cur, Coord dest) {
    return abs(dest.x - cur.x) + abs(dest.y - cur.y);
}

bool Astar::isValid(int x, int y, int maxRow, int maxCol) {
    return (x >= 0) && (x <maxRow) && (y >= 0) && (y < maxCol);
}

vector<Coord> Astar::getNeighbours(Coord c, int maxRow, int maxCol) {
    vector<Coord> neighbours;
    int cur_x = c.x;
    int cur_y = c.y;
    if (isValid(cur_x-1, cur_y, maxRow, maxCol)) {
        neighbours.push_back(Coord(cur_x-1, cur_y));
    }
    if (isValid(cur_x+1, cur_y, maxRow, maxCol)) {
        neighbours.push_back(Coord(cur_x+1, cur_y));
    }
    if (isValid(cur_x, cur_y-1, maxRow, maxCol)) {
        neighbours.push_back(Coord(cur_x, cur_y-1));
    }
    if (isValid(cur_x, cur_y+1, maxRow, maxCol)) {
        neighbours.push_back(Coord(cur_x, cur_y+1));
    }

    return neighbours;
}

Grid Astar::makeTerrain() {
    Grid a = {{1,   1, 100, 1},
              {1,   1, 100, 1},
              {100, 1, 1,   1},
              {1,   1, 100, 1}};
    return a;
}

void Astar::backtrack(Coord start, Coord dest, map<Coord, Coord> parent) {
    Coord cur = dest;
    while (cur != start) {
        path.push_back(cur);
        cur = parent[cur];
    }
    path.push_back(start);
}

void Astar::searchPath(Grid terrain, Coord start, Coord dest) {
    map<Coord, Coord> parent;
    map<Coord, double> cost;
    set<associatedCost> frontier;

    frontier.put(start, 0);
    parent[start] = start;
    cost[start] = 0;
    int maxRow = terrain.size();
    int maxCol = terrain[0].size();

    while(!frontier.empty()) {
        associatedCost a = *frontier.begin();
        Coord cur = a.second();

        if (cur == dest) break;

        for (Coord next: getNeighbours(cur, maxRow, maxCol)) {
            double new_cost = cost[cur] + terrain[next.x][next.y];
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



