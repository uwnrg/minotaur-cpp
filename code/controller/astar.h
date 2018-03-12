#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include <map>
#include <stack>

class Astar {
public:
    struct Coord {
        int x;
        int y;
    };;

    typedef typename std::vector< std::vector<int> > grid;
    typedef typename std::pair<double, Coord> associatedCost;

    Astar();
    void searchPath(grid terrain, Coord cur, Coord dest);
    std::vector<Coord> getPath();
private:
    std::vector<Coord> path;

    double manhattanDist(Coord, Coord);
    bool isValid(int, int, int, int);

    std::vector<Coord> getNeighbours(Coord, int, int);

    void backtrack(Coord, Coord, std::map<Coord, Coord>);
};

bool operator == (Astar::Coord, Astar::Coord);

bool operator != (Astar::Coord, Astar::Coord);

bool operator < (Astar::Coord, Astar::Coord);

#endif
