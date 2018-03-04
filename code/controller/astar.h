#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include <map>
#include <stack>

struct Coord {
    int x;
    int y;
    Coord(int x, int y): x(x), y(y) {}
};

bool operator == (Coord a, Coord b) {
    return a.x == b.x && a.y == b.y;
}

bool operator != (Coord a, Coord b) {
    return !(a == b);
}

class Astar {
private:
    typedef typename std::vector< std::vector<int> > Grid;
    typedef typename std::pair<double, Coord> associatedCost;

    std::vector<Coord> path;

    double manhattanDist(Coord, Coord);
    bool isValid(int, int, int, int);

    std::vector<Coord> getNeighbours(Coord, int, int);
    Grid makeTerrain();

    void backtrack(Coord, Coord, std::map<Coord, Coord>);

public:
    Astar();
    void searchPath(Grid terrain, Coord cur, Coord dest);

};

#endif
