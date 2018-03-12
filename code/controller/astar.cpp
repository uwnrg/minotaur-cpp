#include "astar.h"
#include <cmath>
#include <set>
#include <queue>
#include <iostream>
#include <algorithm>
#include <iterator>

using namespace std;

Astar::Astar() {}

double Astar::manhattanDist(Astar::Coord cur, Astar::Coord dest) {
    return abs(dest.x - cur.x) + abs(dest.y - cur.y);
}

bool Astar::isValid(int x, int y, int maxRow, int maxCol) {
    return (x >= 0) && (x < maxRow) && (y >= 0) && (y < maxCol);
}

vector<Astar::Coord> Astar::getNeighbours(Astar::Coord c, int maxRow, int maxCol) {
    vector<Astar::Coord> neighbours;
    int cur_x = c.x;
    int cur_y = c.y;
    if (isValid(cur_x-1, cur_y, maxRow, maxCol)) {
        neighbours.push_back(Astar::Coord{cur_x-1, cur_y});
    }
    if (isValid(cur_x+1, cur_y, maxRow, maxCol)) {
        neighbours.push_back(Astar::Coord{cur_x+1, cur_y});
    }
    if (isValid(cur_x, cur_y-1, maxRow, maxCol)) {
        neighbours.push_back(Astar::Coord{cur_x, cur_y-1});
    }
    if (isValid(cur_x, cur_y+1, maxRow, maxCol)) {
        neighbours.push_back(Astar::Coord{cur_x, cur_y+1});
    }

    return neighbours;
}

void Astar::backtrack(Astar::Coord start, Astar::Coord dest, map<Astar::Coord, Astar::Coord> parent) {
    Astar::Coord cur = dest;
    while (cur != start) {
        path.push_back(cur);
        cur = parent[cur];
    }
    path.push_back(start);
    reverse(begin(path), end(path));
}

void Astar::searchPath(array2d<int>& terrain, Astar::Coord start, Astar::Coord dest) {
    // idea for using maps to keep track of path information came from redblobgames.com
    map<Astar::Coord, Astar::Coord> parent;
    map<Astar::Coord, double> cost;
    set< associatedCost > openset;

    // initialize base cases
    openset.insert(make_pair(0, start));
    parent[start] = start;
    cost[start] = 0;

    int maxRow = terrain.x();
    int maxCol = terrain.y();

    while(!openset.empty()) {
        // element returned by begin() C++ set should be lowest
        Coord cur = (*openset.begin()).second;
        // remove from open list
        openset.erase(openset.begin());

        if (cur == dest) break;

        for (Coord next: getNeighbours(cur, maxRow, maxCol)) {
            // check for obstacles
            if (terrain[next.x][next.y] != -1) {
                double newCost = cost[cur] + terrain[next.x][next.y];
                // We have not encountered this location or new cost is less than previous costs
                if (!cost.count(next) || newCost < cost[next]) {
                    // update cost
                    cost[next] = newCost;
                    // move into openset with estimated cost
                    openset.insert(make_pair(newCost + manhattanDist(next, dest), next));
                    // update parent
                    parent[next] = cur;
                }
            }
        }
    }

    backtrack(start, dest, parent);
}

vector<Astar::Coord> Astar::getPath() {
    return path;
}

bool operator == (Astar::Coord a, Astar::Coord b) {
    return a.x == b.x && a.y == b.y;
}

bool operator != (Astar::Coord a, Astar::Coord b) {
    return !(a == b);
}

bool operator < (Astar::Coord a, Astar::Coord b) {
    return std::tie(a.x, a.y) < std::tie(b.x, b.y);
}