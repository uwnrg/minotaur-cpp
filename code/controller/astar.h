#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include <map>
#include <stack>

class Astar {
private:
    typedef typename std::vector< std::vector<int> > grid;
    typedef typename std::pair<double, Coord> associatedCost;

    std::vector<Coord> path;

    double manhattanDist(Coord, Coord);
    bool isValid(int, int, int, int);

    std::vector<Coord> getNeighbours(Coord, int, int);
    grid makeTerrain();

    void backtrack(Coord, Coord, std::map<Coord, Coord>);

public:
    struct Coord {
        int x;
        int y;
    };

    Astar();
    void searchPath(grid terrain, Coord cur, Coord dest);
};


bool operator == (Astar::Coord a, Astar::Coord b) {
    return a.x == b.x && a.y == b.y;
}

bool operator != (Astar::Coord a, Astar::Coord b) {
    return !(a == b);
}

bool operator < (Astar::Coord a, Astar::Coord b) {
    return std::tie(a.x, a.y) < std::tie(b.x, b.y);
}

#endif
