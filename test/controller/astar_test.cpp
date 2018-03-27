#include <gtest/gtest.h>

#include <code/controller/astar.h>

using namespace std;

TEST(direct_movement, find_path) {
    array2d<int> a = {{1,   1, -1, 1},
                     {1,   1, -1, 1},
                     {-1,  1, 1,  1},
                     {1,   1, -1, 1}};

    Astar astar = Astar();
    astar.searchPath(a, Astar::Coord{3, 0}, Astar::Coord{0, 3});
    std::vector<Astar::Coord> generated = astar.get_path();

    Astar::Coord p1 = Astar::Coord{3, 0};
    Astar::Coord p2 = Astar::Coord{3, 1};
    Astar::Coord p3 = Astar::Coord{2, 1};
    Astar::Coord p4 = Astar::Coord{2, 2};
    Astar::Coord p5 = Astar::Coord{2, 3};
    Astar::Coord p6 = Astar::Coord{1, 3};
    Astar::Coord p7 = Astar::Coord{0, 3};

    ASSERT_EQ(generated.at(0), p1);
    ASSERT_EQ(generated.at(1), p2);
    ASSERT_EQ(generated.at(2), p3);
    ASSERT_EQ(generated.at(3), p4);
    ASSERT_EQ(generated.at(4), p5);
    ASSERT_EQ(generated.at(5), p6);
    ASSERT_EQ(generated.at(6), p7);
}

