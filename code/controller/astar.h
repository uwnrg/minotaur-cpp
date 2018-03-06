#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include <map>
#include <stack>
#include "../utility/vector2i.h"

class Astar {
/*
public:
    struct Coord {
        int x;
        int y;
    };
    */

private:
    typedef typename std::vector< std::vector<int> > grid;
    typedef typename std::pair<double, Vector2i> associatedCost;

    std::vector<Vector2i> path;

    double manhattanDist(Vector2i, Vector2i);
    bool isValid(int, int, int, int);

    std::vector<Vector2i> getNeighbours(Vector2i, int, int);
    grid makeTerrain();

    void backtrack(Vector2i, Vector2i, std::map<Vector2i, Vector2i>);

public:
    Astar();
    void searchPath(grid terrain, Vector2i cur, Vector2i dest);
};

/*
bool operator == (Astar::Coord a, Astar::Coord b) {
    return a.x == b.x && a.y == b.y;
}

bool operator != (Astar::Coord a, Astar::Coord b) {
    return !(a == b);
}

bool operator < (Astar::Coord a, Astar::Coord b) {
    return std::tie(a.x, a.y) < std::tie(b.x, b.y);
}
 */

#endif
