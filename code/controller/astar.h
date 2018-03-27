#ifndef ASTAR_H
#define ASTAR_H

#include <vector>
#include <map>
#include <stack>
#include "../utility/array2d.h"

class Astar {
public:
    struct Coord {
        int x;
        int y;
    };

    typedef typename std::pair<double, Coord> associated_cost;

    Astar();

    void searchPath(array2d<int> &terrain, Coord cur, Coord dest);

    std::vector<Coord> get_path();

private:
    std::vector<Coord> m_path;

    double manhattan_dist(Coord cur, Coord dest);

    bool is_valid(int x, int y, int max_row, int max_col);

    std::vector<Coord> get_neighbors(Coord c, int max_row, int max_col);

    void backtrack(
        const Coord &start,
        const Coord &dest,
        const std::map<Coord, Coord> &parent
    );
};

bool operator==(Astar::Coord a, Astar::Coord b);

bool operator!=(Astar::Coord a, Astar::Coord b);

bool operator<(Astar::Coord a, Astar::Coord b);

#endif
