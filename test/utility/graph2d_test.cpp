#include <gtest/gtest.h>

#include <code/utility/graph2d.h>

TEST(graph2d, astar_finds_path) {
    nrg::graph2d<double> g;
    auto tl = g.add_node({0, 0});   // 0
    auto tr = g.add_node({20, 0});  // 1
    auto bl = g.add_node({0, 10});  // 2
    auto br = g.add_node({20, 10}); // 3

    auto tm = g.add_node({10, 0});  // 4
    auto lm = g.add_node({0, 5});   // 5
    auto rm = g.add_node({20, 5});  // 6
    auto bm = g.add_node({10, 10}); // 7

    g.connect(tl, tm);
    g.connect(tm, tr);
    g.connect(tr, rm);
    g.connect(rm, br);
    g.connect(br, bm);
    g.connect(bm, bl);
    g.connect(bl, lm);
    g.connect(lm, tl);

    auto start = g.add_node({8, 12}); // 8

    g.connect(start, bm);
    g.connect(start, bl);
    g.connect(start, br);

    std::vector<nrg::node2d<double>> path = g.astar(start, tm);
    std::size_t expected_traverse[] = {start.id(), bl.id(), lm.id(), tl.id(), tm.id()};
    ASSERT_EQ(5, path.size());
    for (int i = 0; i < 5; ++i) {
        ASSERT_EQ(expected_traverse[i], path.at(i).id());
    }
}
