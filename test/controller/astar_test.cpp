#include <gtest/gtest.h>

#include <code/controller/astar.h>

TEST(direct_movement, find_path) {
    array2d<int> a = {{1,   1, -1, 1},
                     {1,   1, -1, 1},
                     {-1,  1, 1,  1},
                     {1,   1, -1, 1}};

    std::vector<vector2i> path;
    nrg::search_path(a, {3, 0}, {0, 3}, path);

    vector2i p1 = {3, 0};
    vector2i p2 = {3, 1};
    vector2i p3 = {2, 1};
    vector2i p4 = {2, 2};
    vector2i p5 = {2, 3};
    vector2i p6 = {1, 3};
    vector2i p7 = {0, 3};

    ASSERT_EQ(path.at(0), p1);
    ASSERT_EQ(path.at(1), p2);
    ASSERT_EQ(path.at(2), p3);
    ASSERT_EQ(path.at(3), p4);
    ASSERT_EQ(path.at(4), p5);
    ASSERT_EQ(path.at(5), p6);
    ASSERT_EQ(path.at(6), p7);
}

